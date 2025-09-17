/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_receive.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 10:45:54 by vileleu           #+#    #+#             */
/*   Updated: 2025/09/17 17:06:01 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scan.h"

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
