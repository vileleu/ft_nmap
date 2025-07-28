/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 23:07:01 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/28 23:07:47 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

void	free_list(t_list *list) {
	t_list	*tmp = NULL;

	while (list) {
		tmp = list;
		list = list->next;
		free(tmp->data);
		free(tmp);
	}
}

void	free_list_addr(t_list_addr *list) {
	t_list_addr	*tmp = NULL;

	while (list) {
		tmp = list;
		list = list->next;
		free(tmp->data);
		free(tmp);
	}
}

void    free_opt(t_opt *opt) {
    free_list_addr(opt->targets);
	if (opt->port.islist)
		free_list(opt->port.list);
    free(opt);
}

void	print_opt(t_opt *opt) {
	t_list_addr	*tmp_addr = NULL;
	t_list		*tmp = NULL;
	uint8_t	i = 0;

	// ip/host
	printf("Scan Configurations\nTarget Ip-Address : ");
	tmp_addr = opt->targets;
	while (tmp_addr) {
		printf("%s", (char *)tmp_addr->data);
		tmp_addr = tmp_addr->next;
		if (tmp_addr)
			printf(" ");
	}
	printf("\n");
	// port
	printf("No of Ports to scan : ");
	if (opt->port.isranged)
		printf("%d-%d", opt->port.min, opt->port.max);
	else if (opt->port.islist) {
		tmp = opt->port.list;
		while (tmp) {
			printf("%u", (*(uint16_t *)tmp->data));
			tmp = tmp->next;
			if (tmp)
				printf(" ");
		}
	}
	else
		printf("%d", opt->port.port);
	printf("\n");
	//scan
	printf("Scans to be performed : ");
	while (i < 6) {
		switch (opt->scan[i]) {
			case 1:
				printf("SYN");
				break;
			case 2:
				printf("NULL");
				break;
			case 3:
				printf("ACK");
				break;
			case 4:
				printf("FIN");
				break;
			case 5:
				printf("XMAS");
				break;
			case 6:
				printf("UDP");
				break;
		}
		if (i++ < 6)
			printf(" ");
	}
	printf("\n");
	// thread
	printf("No of threads : %d\n", opt->thread);
}