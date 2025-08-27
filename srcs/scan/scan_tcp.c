/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_tcp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 23:36:41 by vileleu           #+#    #+#             */
/*   Updated: 2025/08/26 17:48:15 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scan.h"

static void	fill_tcp_packet(t_scan_opt *scan_opt) {
	struct iphdr	*ip = (struct iphdr *)scan_opt->packet;
	struct tcphdr	*tcp = (struct tcphdr *)(scan_opt->packet + sizeof(struct iphdr));

	ip->ihl = 5;                                // ip header length (4 bytes / 1 word)
	ip->version = 4;                            // IPV4
	ip->tos = 0;                                // type of service
    ip->tot_len = htons(scan_opt->packet_size); // size of packet (IP + TCP)
    ip->id = htons(getpid());                   // id
    ip->frag_off = 0;                           // fragment offset (set to 0 for allow fragmentation)
    ip->ttl = 64;                               // ttl (default 64)
    ip->protocol = IPPROTO_TCP;                 // protocol is TCP/IP
    ip->check = 0;                              // checksum
    ip->saddr = scan_opt->src->sin_addr.s_addr; // addr source
    ip->daddr = scan_opt->dst->sin_addr.s_addr; // addr destination
	ip->check = get_checksum((uint16_t *)scan_opt->packet, scan_opt->packet_size);

	tcp->seq = htonl(0);                // sequence
    tcp->ack_seq = htonl(0);            // acknowledgment number (set to 0 for ignore)
    tcp->doff = 5;                      // tcp header length (4 bytes / 1 word)
    set_flag_scan(tcp, scan_opt->scan); // set scan flags (SYN,NULL,ACK,FIN,XMAS)
    tcp->window = htons(5840);          // TCP window size (default is 5840)
    tcp->check = 0;                     // checksum
    tcp->urg_ptr = 0;                   // urgent pointer (set to 0 for ignore)
}

static void	set_tcp_port(const unsigned char *packet, uint16_t *port, uint16_t *source) {
	struct iphdr	*ip = (struct iphdr *)packet;
	struct tcphdr	*tcp = (struct tcphdr *)(packet + sizeof(struct iphdr));
	t_psh			psh;
	
	tcp->source = htons(*source);  // port source
    tcp->dest = htons(*port);      // port destination
	tcp->check = 0;

	psh.src_addr = ip->saddr;
	psh.dst_addr = ip->daddr;
	psh.placeholder = 0;
	psh.protocol = IPPROTO_TCP;
	psh.tcp_length = htons(sizeof(struct tcphdr));
	memcpy(&psh.tcphdr, tcp, sizeof(struct tcphdr));
	tcp->check = get_checksum((uint16_t *)&psh, sizeof(psh)); // use struct t_psh for set up tcp checksum
}

uint8_t	send_tcp_packet(t_scan_opt *scan_opt, uint16_t *source) {
	t_list	*tmp = scan_opt->port;
	int32_t	sock = 0;
	int32_t	one = 1;

	fill_tcp_packet(scan_opt);
	if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0)
		return (error_scan_errno("socket"));
	if ((setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one))) < 0)
		return (error_scan_errno("setsockopt"));
	while (tmp) {
		printf("source = %u, htons(source) = %u\n", *source, htons(*source));
		scan_opt->dst->sin_port = *(tmp->data);
		set_tcp_port(scan_opt->packet, tmp->data, source);
		if ((sendto(sock, scan_opt->packet, scan_opt->packet_size, 0, (struct sockaddr *)scan_opt->dst, sizeof(*scan_opt->dst))) < 0)
			return (error_scan_errno("sendto"));
		tmp = tmp->next;
		(*source)++;
	}
	close(sock);
	printf("success!\n");
	return (EXIT_SUCCESS);
}
