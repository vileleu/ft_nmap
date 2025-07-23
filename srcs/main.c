/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:38:12 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/23 02:28:54 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

void	print_opt(const char *name, t_opt *opt) {
	printf("%s:\n", name);
	// port
	(opt->port.isranged ? printf("port: %d-%d\n", opt->port.min, opt->port.max) : printf("port: %d\n", opt->port.port));
	// thread
	printf("thread: %d\n", opt->thread);
}

int	main(const int ac, const char **av) {
	t_opt		*opt;

	if (!(opt = parsing(av, ac)))
		return (1);
	print_opt(av[0] + 2, opt);
	free(opt);
	return (0);
}