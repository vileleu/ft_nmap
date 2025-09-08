/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 00:09:42 by vileleu           #+#    #+#             */
/*   Updated: 2025/08/29 00:12:55 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scan.h"

uint8_t	error_scan(const char *msg) {
	fprintf(stderr, "%s: %s\n", "ft_nmap", msg);
	return (EXIT_FAILURE);
}

uint8_t	error_scan_errno(const char *msg) {
	fprintf(stderr, "%s: %s: %s\n", "ft_nmap", msg, strerror(errno));
	return (EXIT_FAILURE);
}

void	*error_scan_pcap(t_pcap_data *p_data, const char *msg, const char *des) {
	fprintf(stderr, "%s", msg);
	(des ? fprintf(stderr, ": %s", des) : (void)0);
	fprintf(stderr, "\n");
	free_pcap_data(p_data);
	return (NULL);
}