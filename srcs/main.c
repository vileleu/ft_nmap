/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:38:12 by vileleu           #+#    #+#             */
/*   Updated: 2025/10/20 20:37:14 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

static int run_scan(t_opt *opt, t_final_status *f_s) {
    t_list_addr *current = opt->targets;
    while (current) {
        if (!check_host_availability(current->data))
            printf("Host %s is unreachable.\n", current->data);
        else {
			if (create_final_status(&f_s, opt)) {
				free_final_status(f_s);
				free_opt(opt);
				return EXIT_FAILURE;
			}
            else {
                printf("\nHost %s is up.\n", current->data);
                struct timeval t_scan_start, t_scan_end;
                gettimeofday(&t_scan_start, NULL); //calcul temps du/des scans
                if (init_scan_configuration(opt, f_s) == 1) {
                    printf("Scan failed\n");
                    return 1;
                }
                gettimeofday(&t_scan_end, NULL);
                double total_sec = (t_scan_end.tv_sec - t_scan_start.tv_sec)
                                 + (t_scan_end.tv_usec - t_scan_start.tv_usec) / 1000000.0;
            
                printf("Scan took %.5f secs\n", total_sec);
			    print_conclusion(f_s);
			    f_s = NULL;
            }
        }
        current = current->next;
    }
    return EXIT_SUCCESS;
}

int main(const int ac, const char **av) {
    t_opt 			*opt;
	t_final_status	*final_status = NULL;

	if (getuid())
		return (error_all("Need privileges"));
    if (!(opt = parsing(av, ac)))
        return EXIT_FAILURE;
    print_opt(opt);
    srand(time(NULL));
    int scan_result = run_scan(opt, final_status);
    if (scan_result == 1) {
        free_opt(opt);             
        free_final_status(final_status);
        return EXIT_FAILURE;
    }
    free_opt(opt);
    return EXIT_SUCCESS;
}
