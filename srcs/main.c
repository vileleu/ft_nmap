/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:38:12 by vileleu           #+#    #+#             */
/*   Updated: 2025/08/14 16:13:49 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

int	main(const int ac, const char **av) {
	t_opt		*opt;

	if (!(opt = parsing(av, ac)))
		return (EXIT_FAILURE);
	print_opt(opt);
	if (send_scan(&opt->targets->addr, 3000, ACK)) {
		free_opt(opt);
		return (EXIT_FAILURE);
	}
	free_opt(opt);
	return (EXIT_SUCCESS);
}