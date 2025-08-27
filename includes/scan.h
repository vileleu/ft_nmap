/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 00:06:09 by vileleu           #+#    #+#             */
/*   Updated: 2025/08/26 22:54:27 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCAN_H
#define SCAN_H

#include "ft_nmap.h"

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <pcap.h>

typedef struct	s_scan_opt {
	unsigned char		*packet;
	size_t				packet_size;
	struct sockaddr_in	*src;
	struct sockaddr_in	*dst;
	t_list				*port;
	uint8_t				scan;
	uint16_t			seq;
}					t_scan_opt;

typedef struct	s_psh {
	uint32_t		src_addr;
	uint32_t		dst_addr;
	uint8_t			placeholder;
	uint8_t			protocol;
	uint16_t		tcp_length;
	struct tcphdr	tcphdr;
}				t_psh;

/*
** SCAN FUNCTIONS
*/

uint8_t			send_tcp_packet(t_scan_opt *scan_opt, uint16_t *source);

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
uint8_t			error_scan_pcap(pcap_if_t *list_if, pcap_t *handle, const char *msg, const char *name, const char *des);

#endif
