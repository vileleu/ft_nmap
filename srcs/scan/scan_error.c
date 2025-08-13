/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 00:09:42 by vileleu           #+#    #+#             */
/*   Updated: 2025/08/14 00:18:09 by vileleu          ###   ########.fr       */
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