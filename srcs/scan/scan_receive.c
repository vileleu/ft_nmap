/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_receive.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 10:45:54 by vileleu           #+#    #+#             */
/*   Updated: 2025/09/08 18:41:40 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scan.h"

static t_list_result	*init_list_result(t_list *ports, uint16_t source, uint8_t scan[SIZE_SCAN]) {
	t_list_result	*list = NULL;
	t_list_result	*tmp = NULL;
	t_list			*tmp_ports = NULL;
	uint8_t			i = 0;

	while (i < SIZE_SCAN && scan[i]) {
		tmp_ports = ports;
		while (tmp_ports) {
			if (!list) {
				if (!(list = malloc(sizeof(t_list_result)))) {
					error_scan_errno("malloc");
					return (free_list_result(list));
				}
				bzero(list, sizeof(t_list_result));
				list->source = source++;
				list->dest = *(tmp_ports->data);
				list->scan = scan[i];
				if (list->scan == SYN || list->scan == ACK)
					list->status.filtered = 1;
				else
					list->status.open_filtered = 1;
				list->next = NULL;
				tmp = list;
			}
			else {
				if (!(tmp->next = malloc(sizeof(t_list_result)))) {
					error_scan_errno("malloc");
					return (free_list_result(list));
				}
				bzero(tmp->next, sizeof(t_list_result));
				tmp->next->source = source++;
				tmp->next->dest = *(tmp_ports->data);
				tmp->next->scan = scan[i];
				if (tmp->next->scan == SYN || tmp->next->scan == ACK)
					tmp->next->status.filtered = 1;
				else
					tmp->next->status.open_filtered = 1;
				tmp->next->next = NULL;
				tmp = tmp->next;
			}
			tmp_ports = tmp_ports->next;
		}
		i++;
	}
	return (list);
}

t_pcap_data		*init_pcap_data(const char *ip, t_list *ports, uint16_t source, uint16_t count, uint8_t scan[SIZE_SCAN]) {
	t_pcap_data			*p_data = NULL;
	pcap_if_t			*list_if = NULL;
    char				error_buffer[PCAP_ERRBUF_SIZE];
    char				filter_exp[100];
	bpf_u_int32			ip_bpf = 0;
	const struct timeval		*tv_pointer = NULL;

	if (!(p_data = malloc(sizeof(t_pcap_data)))) {
			error_scan_errno("malloc");
			return (NULL);
	}
	bzero(p_data, sizeof(t_pcap_data));
	p_data->handle = NULL;
	p_data->l_result = NULL;
	if (!(p_data->l_result = init_list_result(ports, source, scan)))
		return (free_pcap_data(p_data));
	// create our string filter (like tcpdump)
	sprintf(filter_exp, "(tcp or udp or (icmp[0] = 3)) and src %s and dst portrange %u-%u", ip, source, source + count - 1);
	printf("filter = %s\n", filter_exp);
	// search device (ex: eth0)
    if (pcap_findalldevs(&list_if, error_buffer) < 0)
		return (error_scan_pcap(p_data, "pcap_findalldevs", error_buffer));
    // open device for live capture
	// can use pcap_open_live(list_if->name, BUFSIZ, 1, 1000, error_buffer) but 64 is enought for header IP + TCP/UDP
    p_data->handle = pcap_open_live(list_if->name, 64, 1, 1000, error_buffer);
	pcap_freealldevs(list_if);
	if (!p_data->handle)
		return (error_scan_pcap(p_data, "pcap_open_live", error_buffer));
	// compile filter
    if (pcap_compile(p_data->handle, &p_data->filter, filter_exp, 0, ip_bpf) < 0)
		return (error_scan_pcap(p_data, "pcap_compile", pcap_geterr(p_data->handle)));
	p_data->filter_on = 1;
	// set filter
    if (pcap_setfilter(p_data->handle, &p_data->filter) < 0)
		return (error_scan_pcap(p_data, "pcap_setfilter", pcap_geterr(p_data->handle)));
	// get fd of pcap for read buffer of packet in transit
	if ((p_data->fd = pcap_get_selectable_fd(p_data->handle)) < 0)
		return (error_scan_pcap(p_data, "pcap_get_selectable_fd", pcap_geterr(p_data->handle)));
	// on windows pcap give a special timeout for select
	if ((tv_pointer = pcap_get_required_select_timeout(p_data->handle)))
		p_data->tv = *tv_pointer; //p_data->tv = *tv_pointer;
	else {
		p_data->tv.tv_sec = 3;
		p_data->tv.tv_usec = 0;
	}
	p_data->nb_packet = count;
	return (p_data);
}

static void	packet_handler(unsigned char *arg, const struct pcap_pkthdr *packet_header, const unsigned char *packet) {
	t_list_result				*l_result = (t_list_result *)arg;
	const struct iphdr			*ip = NULL;
	const struct tcphdr			*tcp = NULL;
	const struct udphdr			*udp = NULL;
	uint16_t					packet_port_dst = 0;

	(void)packet_header;
	// packet_header->len = size of full packet (don't need it)
    // printf("Packet capture length: %d\n", packet_header->caplen);
	// skip ethernet header
	packet = packet + sizeof(struct ether_header);
	ip = (struct iphdr *)(packet);
	if (ip->protocol == IPPROTO_ICMP)
		return (get_response_unreach(packet, l_result));
	else if (ip->protocol == IPPROTO_TCP) {
		tcp = (struct tcphdr *)(packet + (ip->ihl * 4));
		packet_port_dst = ntohs(tcp->dest);
	}
	else if (ip->protocol == IPPROTO_UDP) {
		udp = (struct udphdr *)(packet + (ip->ihl * 4));
		packet_port_dst = ntohs(udp->uh_dport);
	}
	else
		return;
	while (l_result) {
		if (l_result->source == packet_port_dst)
			return (get_response(packet, l_result));
		l_result = l_result->next;
	}
    return;
}

uint8_t			scan_receive(t_pcap_data *p_data, t_final_status *final_status) {
	fd_set 			fds;
	struct timeval	tv;
	int 			ret = 0;
	int				n = 0;

	while (p_data->nb_packet > 0) {
		FD_ZERO(&fds);
		FD_SET(p_data->fd, &fds);
		tv = p_data->tv;
		if ((ret = select(p_data->fd + 1, &fds, NULL, NULL, &tv)) > 0) {
		    n = pcap_dispatch(p_data->handle, p_data->nb_packet, packet_handler, (unsigned char *)p_data->l_result);
			p_data->nb_packet -= n;
		}
		else if (!ret)
			break;
		else {
		    return (error_scan_errno("select"));
		}
	}
	write_conclusion(p_data->l_result, final_status);
	return (EXIT_SUCCESS);
}
