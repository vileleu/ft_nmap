/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_all.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:31:28 by vileleu           #+#    #+#             */
/*   Updated: 2025/09/08 16:31:59 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

uint8_t	error_all(const char *msg) {
	fprintf(stderr, "%s: %s\n", "ft_nmap", msg);
	return (EXIT_FAILURE);
}

uint8_t	error_all_errno(const char *msg) {
	fprintf(stderr, "%s: %s: %s\n", "ft_nmap", msg, strerror(errno));
	return (EXIT_FAILURE);
}
