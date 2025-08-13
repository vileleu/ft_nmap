/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 00:06:09 by vileleu           #+#    #+#             */
/*   Updated: 2025/08/14 00:29:16 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCAN_H
#define SCAN_H

#include "ft_nmap.h"

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>

typedef struct	s_psh {
	u_int32_t		src_addr;
	u_int32_t		dst_addr;
	u_int8_t		placeholder;
	u_int8_t		protocol;
	u_int16_t		tcp_length;
	struct tcphdr	tcphdr;
}				t_psh;

/*
** SCAN FUNCTIONS
*/

void			fill_tcp_packet(unsigned char *packet, size_t packet_size, struct sockaddr_in *src, struct sockaddr_in *dst, uint16_t port, uint8_t scan);
uint8_t			send_tcp_packet(const unsigned char *packet, size_t packet_size, struct sockaddr_in *dst);

/*
** SCAN UTILS FUNCTIONS
*/

uint8_t			get_local_sockaddr(struct sockaddr_in *local);
void			set_flag_scan(struct tcphdr *tcp, uint8_t scan);
unsigned short	get_checksum(unsigned short *packet, size_t size);

/*
** SCAN ERROR FUNCTIONS
*/

uint8_t			error_scan(const char *msg);
uint8_t			error_scan_errno(const char *msg);

#endif
