/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_send.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 23:49:40 by vileleu           #+#    #+#             */
/*   Updated: 2025/08/26 18:12:57 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scan.h"

uint16_t		scan_send(struct sockaddr_in *dst, t_list *port, uint8_t scan[SIZE_SCAN], uint16_t source) {
	t_scan_opt			scan_opt;
	struct sockaddr_in	src;
	uint16_t			copy_source = source;
	int					i = 0;
	
	bzero(&scan_opt, sizeof(scan_opt));
	bzero(&src, sizeof(src));
	scan_opt.packet = NULL;
	scan_opt.packet_size = 0;
	scan_opt.dst = dst;
	scan_opt.src = &src;
	scan_opt.port = port;

	scan_opt.dst->sin_family = AF_INET;
	if (get_local_sockaddr(scan_opt.src))
		return(0);
	while (scan[i] && i < SIZE_SCAN) {
		scan_opt.scan = scan[i++];
		if (scan_opt.scan == UDP)
			scan_opt.packet_size = sizeof(struct iphdr) + sizeof(struct udphdr);
		else
			scan_opt.packet_size = sizeof(struct iphdr) + sizeof(struct tcphdr);
		if (!(scan_opt.packet = malloc(scan_opt.packet_size))) {
			error_scan_errno("malloc");
			return (0);
		}
    	bzero(scan_opt.packet, scan_opt.packet_size);
		if (scan_opt.scan == UDP) {
			(void)scan;
		}
		else {
			if (send_tcp_packet(&scan_opt, &source)) {
				free(scan_opt.packet);
				return (0);
			}
		}
		free(scan_opt.packet);
	}
	return (source - copy_source);
}