/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 23:36:56 by vileleu           #+#    #+#             */
/*   Updated: 2025/09/01 15:57:41 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scan.h"

void			*free_list_result(t_list_result *list) {
	t_list_result *tmp = list;
	
	while (list) {
		tmp = list;
		list = list->next;
		free(tmp);
		tmp = NULL;
	}
	return (NULL);
}

void			*free_pcap_data(t_pcap_data *p_data) {
	if (p_data->l_result)
		free_list_result(p_data->l_result);
	if (p_data->filter_on)
		pcap_freecode(&p_data->filter);
	if (p_data->handle)
		pcap_close(p_data->handle);
	free(p_data);
	return (NULL);
}

uint8_t			get_local_sockaddr(struct sockaddr_in *local) {
	struct sockaddr_in	tmp;
	socklen_t 			locallen = sizeof(*local);
	int 				sock = 0;

	bzero(&tmp, sizeof(tmp));
	tmp.sin_family = AF_INET;
	tmp.sin_port = htons(53); // default DNS port is 53
	if ((inet_pton(AF_INET, "8.8.8.8", &tmp.sin_addr)) < 0)
		return (error_scan_errno("inet_pton"));
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		return (error_scan_errno("socket"));
	if ((connect(sock, (const struct sockaddr *)&tmp, sizeof(tmp))) < 0)
		return (error_scan_errno("connect"));
	if ((getsockname(sock, (struct sockaddr *)local, &locallen)) < 0)
		return (error_scan_errno("getsockname"));
	close(sock);
	return (EXIT_SUCCESS);
}

void			set_flag_scan(struct tcphdr *tcp, uint8_t scan) {
	switch (scan) {
		case SYN:
			tcp->syn = 1;
			break;
		case ACK:
			tcp->ack = 1;
			break;
		case FIN:
			tcp->fin = 1;
			break;
		case XMAS:
			tcp->fin = 1;
			tcp->psh = 1;
			tcp->urg = 1;
			break;
		default:
			break;
		// if scan == NULL, alls flags are set to 0
	}
}

unsigned short	get_checksum(unsigned short *packet, size_t size) {
	uint32_t	checksum = 0;
	
	while (size > 1) {
		checksum += *(packet++);
		size -= 2;
	}
	if (size == 1)
		checksum += *((unsigned char *)packet);
	checksum = (checksum >> 16) + (checksum & 0xffff);
	return (~checksum);
}
