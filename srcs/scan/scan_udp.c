/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_udp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 23:50:07 by vileleu           #+#    #+#             */
/*   Updated: 2025/08/31 17:02:04 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scan.h"

static void	fill_udp_packet(t_scan_opt *scan_opt) {
	struct iphdr	*ip = (struct iphdr *)scan_opt->packet;
	struct udphdr	*udp = (struct udphdr *)(scan_opt->packet + sizeof(struct iphdr));

	ip->ihl = (sizeof(struct iphdr) / 4);        // ip header length (4 bytes / 1 word)
	ip->version = 4;                             // IPV4
	ip->tos = 0;                                 // type of service
    ip->tot_len = htons(scan_opt->packet_size);  // size of packet (IP + UDP)
    ip->id = htons(getpid());                    // id
    ip->frag_off = 0;                            // fragment offset (set to 0 for allow fragmentation)
    ip->ttl = 64;                                // ttl (default 64)
    ip->protocol = IPPROTO_UDP;                  // protocol is UDP/IP
    ip->check = 0;                               // checksum
    ip->saddr = scan_opt->src->sin_addr.s_addr;  // addr source
    ip->daddr = scan_opt->dst->sin_addr.s_addr;  // addr destination
	ip->check = get_checksum((uint16_t *)scan_opt->packet, scan_opt->packet_size);

	udp->uh_ulen = htons(sizeof(struct udphdr)); // udp length
	udp->uh_sum = 0;                             // checksum
}

static void	set_udp_port(const unsigned char *packet, uint16_t *port, uint16_t *source) {
	struct iphdr	*ip = (struct iphdr *)packet;
	struct udphdr	*udp = (struct udphdr *)(packet + sizeof(struct iphdr));
	t_psh_udp		psh;
	
	udp->uh_sport = htons(*source);    // port source
    udp->uh_dport = htons(*port);      // port destination
	udp->uh_sum = 0;

	psh.src_addr = ip->saddr;
	psh.dst_addr = ip->daddr;
	psh.placeholder = 0;
	psh.protocol = IPPROTO_UDP;
	psh.udp_length = udp->uh_ulen;
	memcpy(&psh.udphdr, udp, sizeof(struct udphdr));
	udp->check = get_checksum((uint16_t *)&psh, sizeof(psh)); // use struct t_psh_udp for set up udp checksum
}

uint8_t		send_udp_packet(t_scan_opt *scan_opt, uint16_t *source) {
	t_list	*tmp = scan_opt->port;
	int32_t	sock = 0;
	int32_t	one = 1;

	fill_udp_packet(scan_opt);
	if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0)
		return (error_scan_errno("socket"));
	if ((setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one))) < 0)
		return (error_scan_errno("setsockopt"));
	while (tmp) {
		printf("send udp packet: source = %u\n", *source);
		scan_opt->dst->sin_port = *(tmp->data);
		set_udp_port(scan_opt->packet, tmp->data, source);
		if ((sendto(sock, scan_opt->packet, scan_opt->packet_size, 0, (struct sockaddr *)scan_opt->dst, sizeof(*scan_opt->dst))) < 0)
			return (error_scan_errno("sendto"));
		tmp = tmp->next;
		(*source)++;
	}
	close(sock);
	return (EXIT_SUCCESS);
}