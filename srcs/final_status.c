/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:56:52 by vileleu           #+#    #+#             */
/*   Updated: 2025/10/05 17:56:40 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

void	free_final_status(t_final_status *final_status) {
	t_final_status *tmp = NULL;

	while (final_status) {
		tmp = final_status;
		final_status = final_status->next;
		free(tmp);
		tmp = NULL;
	}
}

static char *print_status(const t_port_status status) {
	if (status.open)
		return ("open");
	else if (status.closed)
		return ("closed");
	else if (status.filtered)
		return ("filtered");
	else if (status.open_filtered)
		return ("open_filtered");
	else if (status.unfiltered)
		return ("unfiltered");
	else
		return (NULL);
}

static char	*print_scan(const uint8_t scan) {
	switch (scan) {
		case SYN:
			return ("SYN");
		case NUL:
			return ("NULL");
		case ACK:
			return ("ACK");
		case FIN:
			return ("FIN");
		case XMAS:
			return ("XMAS");
		case UDP:
			return ("UDP");
		default:
			return (NULL);
	}
}

static char *print_service(const uint16_t port) {
	struct servent	*service = NULL;
	
	service = getservbyport(htons(port), NULL);
    if (service)
        return (service->s_name);
	else
        return ("Unassigned");
}

static char*	print_end(uint8_t conclusion) {
	if (conclusion == OPEN)
		return ("open");
	else if (conclusion == CLOSED)
		return ("closed");
	else if (conclusion == FILTERED)
		return ("filtered");
	else if (conclusion == UNFILTERED)
		return ("unfiltered");
	else
		return ("filtered");
}

void	print_conclusion(t_final_status *final_status) {
	t_final_status	*tmp = final_status;
	char			print_s[21] = "";
	char			print_a[110] = "";
	uint8_t			i = 0;

	printf("Open ports:\n");
	printf("%-7s%-15s%-108s%-10s\n", "Port", "Service Name", "Results", "Conclusion");
	printf("--------------------------------------------------------------------------------------------------------------------------------------------\n");
	while (tmp) {
		if (tmp->conclusion == OPEN) {
			i = 0;
			printf("%-7u", tmp->port);
			printf("%-15s", print_service(tmp->port));
			bzero(print_a, 110);
			while (i < SIZE_SCAN && tmp->scan[i]) {
				sprintf(print_s, (i ? " %s(%s)" : "%s(%s)"), print_scan(tmp->scan[i]), print_status(tmp->status[i]));
				memcpy(print_a + strlen(print_a), print_s, strlen(print_s));
				i++;
			}
			printf("%-108s", print_a);
			printf("%-10s", print_end(tmp->conclusion));
			printf("\n");	
		}
		tmp = tmp->next;
	}
	printf("\n");
	tmp = final_status;
	printf("Closed/Filtered/Unfiltered ports:\n");
	printf("%-7s%-15s%-108s%-10s\n", "Port", "Service Name", "Results", "Conclusion");
	printf("--------------------------------------------------------------------------------------------------------------------------------------------\n");
	while (tmp) {
		if (tmp->conclusion != OPEN) {
			i = 0;
			printf("%-7u", tmp->port);
			printf("%-15s", print_service(tmp->port));
			bzero(print_a, 110);
			while (i < SIZE_SCAN && tmp->scan[i]) {
				sprintf(print_s, (i ? " %s(%s)" : "%s(%s)"), print_scan(tmp->scan[i]), print_status(tmp->status[i]));
				memcpy(print_a + strlen(print_a), print_s, strlen(print_s));
				i++;
			}
			printf("%-108s", print_a);
			printf("%-10s", print_end(tmp->conclusion));
			printf("\n");	
		}
		tmp = tmp->next;
	}
	free_final_status(final_status);
}

static uint8_t	add_final_status(t_final_status **final_status, uint16_t port) {
	t_final_status	*tmp = *final_status;

	if (!tmp) {
		if (!((*final_status) = malloc(sizeof(t_final_status))))
			return (error_all_errno("malloc"));
		bzero((*final_status), sizeof(t_final_status));
		(*final_status)->port = port;
		(*final_status)->next = NULL;
		return (EXIT_SUCCESS);
	}
	while (tmp->next)
		tmp = tmp->next;
	if (!(tmp->next = malloc(sizeof(t_final_status))))
		return (error_all_errno("malloc"));
	bzero(tmp->next, sizeof(t_final_status));
	tmp->next->port = port;
	tmp->next->next = NULL;
	return (EXIT_SUCCESS);
}

uint8_t	create_final_status(t_final_status **final_status, t_opt *opt) {
	t_list		*tmp_l = NULL;
	uint16_t	tmp_u = 0;

	if (opt->port.isranged) {
		tmp_u = opt->port.min;
		while (tmp_u <= opt->port.max) {
			if (add_final_status(final_status, tmp_u))
				return (EXIT_FAILURE);
			tmp_u++;
		}
	}
	else if (opt->port.islist) {
		tmp_l = opt->port.list;
		while (tmp_l) {
			if (add_final_status(final_status, *(tmp_l->data)))
				return (EXIT_FAILURE);
			tmp_l = tmp_l->next;
		}
	}
	else {
		if (add_final_status(final_status, opt->port.port))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
