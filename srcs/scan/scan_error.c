/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 00:09:42 by vileleu           #+#    #+#             */
/*   Updated: 2025/08/26 23:04:05 by vileleu          ###   ########.fr       */
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

uint8_t	error_scan_pcap(pcap_if_t *list_if, pcap_t *handle, const char *msg, const char *name, const char *des) {
	fprintf(stderr, "%s", msg);
	(name ? fprintf(stderr, ": %s", name) : (void)0);
	(des ? fprintf(stderr, ": %s", des) : (void)0);
	fprintf(stderr, "\n");
	if (list_if)
		pcap_freealldevs(list_if);
	if (handle)
		pcap_close(handle);
	return (EXIT_FAILURE);
}