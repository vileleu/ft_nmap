/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_send.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 23:49:40 by vileleu           #+#    #+#             */
/*   Updated: 2025/09/01 18:13:08 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scan.h"

uint8_t		scan_send(struct sockaddr_in *dst, t_list *ports, uint16_t source, uint8_t scan[SIZE_SCAN]) {
	t_scan_opt			scan_opt;
	struct sockaddr_in	src;
	int					i = 0;

	bzero(&scan_opt, sizeof(scan_opt));
	bzero(&src, sizeof(src));
	scan_opt.packet = NULL;
	scan_opt.packet_size = 0;
	scan_opt.dst = dst;
	scan_opt.src = &src;
	scan_opt.port = ports;

	scan_opt.dst->sin_family = AF_INET;
	if (get_local_sockaddr(scan_opt.src))
		return (EXIT_FAILURE);
	while (i < SIZE_SCAN && scan[i]) {
		scan_opt.scan = scan[i++];
		if (scan_opt.scan == UDP)
			scan_opt.packet_size = sizeof(struct iphdr) + sizeof(struct udphdr);
		else
			scan_opt.packet_size = sizeof(struct iphdr) + sizeof(struct tcphdr);
		if (!(scan_opt.packet = malloc(scan_opt.packet_size))) {
			error_scan_errno("malloc");
			return (EXIT_FAILURE);
		}
    	bzero(scan_opt.packet, scan_opt.packet_size);
		if (scan_opt.scan == UDP) {
			if (send_udp_packet(&scan_opt, &source)) {
				free(scan_opt.packet);
				return (EXIT_FAILURE);
			}
		}
		else {
			if (send_tcp_packet(&scan_opt, &source)) {
				free(scan_opt.packet);
				return (EXIT_FAILURE);
			}
		}
		free(scan_opt.packet);
	}
	return (EXIT_SUCCESS);
}