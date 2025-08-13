/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_send.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 23:49:40 by vileleu           #+#    #+#             */
/*   Updated: 2025/08/14 00:31:34 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scan.h"

uint8_t			send_scan(struct sockaddr_in *dst, uint16_t port, uint8_t scan) {
	unsigned char		*packet = NULL;
	size_t				packet_size = sizeof(struct iphdr) + sizeof(struct tcphdr);
	struct sockaddr_in	src;

	dst->sin_family = AF_INET;
	dst->sin_port = port;
	if (get_local_sockaddr(&src))
		return(EXIT_FAILURE);
	if (!(packet = malloc(packet_size)))
		return (error_scan_errno("malloc"));
    bzero(packet, packet_size);
	if (scan == UDP) {
		(void)scan;
	}
	else {
		fill_tcp_packet(packet, packet_size, &src, dst, port, scan);
		if (send_tcp_packet(packet, packet_size, dst)) {
			free(packet);
			return (EXIT_FAILURE);
		}
	}
	free(packet);
	return (EXIT_SUCCESS);
}