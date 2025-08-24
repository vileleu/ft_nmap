/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_tcp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 23:36:41 by vileleu           #+#    #+#             */
/*   Updated: 2025/08/14 16:42:30 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scan.h"

void	fill_tcp_packet(unsigned char *packet, size_t packet_size, struct sockaddr_in *src, struct sockaddr_in *dst, uint16_t port, uint8_t scan) {
	struct iphdr	*ip = (struct iphdr *)packet;
	struct tcphdr	*tcp = (struct tcphdr *)(packet + sizeof(struct iphdr));
	t_psh			psh;

	ip->ihl = 5;                      // ip header length (4 bytes / 1 word)
	ip->version = 4;                  // IPV4
	ip->tos = 0;                      // type of service
    ip->tot_len = htons(packet_size); // size of packet (IP + TCP)
    ip->id = htons(getpid());         // id
    ip->frag_off = 0;                 // fragment offset (set to 0 for allow fragmentation)
    ip->ttl = 64;                     // ttl (default 64)
    ip->protocol = IPPROTO_TCP;       // protocol is TCP/IP
    ip->check = 0;                    // checksum
    ip->saddr = src->sin_addr.s_addr;  // addr source
    ip->daddr = dst->sin_addr.s_addr;  // addr destination
	ip->check = get_checksum((uint16_t *)packet, packet_size);

	tcp->source = htons(1234);        // port source
    tcp->dest = htons(port);          // port destination
    tcp->seq = htonl(0);              // sequence
    tcp->ack_seq = htonl(0);          // acknowledgment number (set to 0 for ignore)
    tcp->doff = 5;                    // tcp header length (4 bytes / 1 word)
    set_flag_scan(tcp, scan);         // SYN on
    tcp->window = htons(5840);        // TCP window size (default is 5840)
    tcp->check = 0;                   // checksum
    tcp->urg_ptr = 0;                 // urgent pointer (set to 0 for ignore)

	psh.src_addr = ip->saddr;
	psh.dst_addr = ip->daddr;
	psh.placeholder = 0;
	psh.protocol = IPPROTO_TCP;
	psh.tcp_length = htons(sizeof(struct tcphdr));
	memcpy(&psh.tcphdr, tcp, sizeof(struct tcphdr));
	tcp->check = get_checksum((uint16_t *)&psh, sizeof(psh)); // use struct t_psh for set up tcp checksum
}

uint8_t	send_tcp_packet(const unsigned char *packet, size_t packet_size, struct sockaddr_in *dst) {
	int32_t	sock = 0;
	int32_t	one = 1;

	if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0)
		return (error_scan_errno("socket"));
	if ((setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one))) < 0)
		return (error_scan_errno("setsockopt"));
	if ((sendto(sock, packet, packet_size, 0, (struct sockaddr *)dst, sizeof(*dst))) < 0)
		return (error_scan_errno("sendto"));
	close(sock);
	printf("success!\n");
	return (EXIT_SUCCESS);
}
