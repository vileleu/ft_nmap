/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 00:06:09 by vileleu           #+#    #+#             */
/*   Updated: 2025/10/06 19:31:33 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCAN_H
#define SCAN_H

#include "ft_nmap.h"

#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <sys/socket.h>

typedef struct	s_scan_opt {
	unsigned char		*packet;
	size_t				packet_size;
	struct sockaddr_in	*src;
	struct sockaddr_in	*dst;
	t_list				*port;
	uint8_t				scan;
	uint16_t			seq;
}					t_scan_opt;

typedef struct	s_psh_tcp {
	uint32_t		src_addr;
	uint32_t		dst_addr;
	uint8_t			placeholder;
	uint8_t			protocol;
	uint16_t		tcp_length;
	struct tcphdr	tcphdr;
}				t_psh_tcp;

typedef struct	s_psh_udp {
	uint32_t		src_addr;
	uint32_t		dst_addr;
	uint8_t			placeholder;
	uint8_t			protocol;
	uint16_t		udp_length;
	struct udphdr	udphdr;
}				t_psh_udp;

/*
** SCAN FUNCTIONS
*/

uint8_t			send_tcp_packet(t_scan_opt *scan_opt, uint16_t *source);
uint8_t			send_udp_packet(t_scan_opt *scan_opt, uint16_t *source);

void			get_response_unreach(const unsigned char *packet, t_list_result *result);
void			get_response(const unsigned char *packet, t_list_result *result);

void			write_conclusion(t_list_result *result, t_final_status *final_status);

/*
** SCAN UTILS FUNCTIONS
*/

uint8_t			get_local_sockaddr(struct sockaddr_in *local);
void			set_flag_scan(struct tcphdr *tcp, uint8_t scan);
unsigned short	get_checksum(unsigned short *packet, size_t size);

void			*free_list_result(t_list_result *list);

/*
** SCAN ERROR FUNCTIONS
*/

uint8_t			error_scan(const char *msg);
uint8_t			error_scan_errno(const char *msg);
void			*error_scan_pcap(t_pcap_data *p_data, const char *msg, const char *des);
#endif
