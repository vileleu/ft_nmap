/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:38:12 by vileleu           #+#    #+#             */
/*   Updated: 2025/10/06 19:09:10 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

static int run_scan(t_opt *opt, t_final_status *f_s) {
    t_list_addr *current = opt->targets;

    while (current) {
        if (!check_host_availability(current->data)) {
            printf("Host %s is unreachable.\n", current->data);
            return EXIT_FAILURE;
        } 
        else {
			if (create_final_status(&f_s, opt)) {
				free_final_status(f_s);
				free_opt(opt);
				return EXIT_FAILURE;
			}
            printf("\nHost %s is up.\n", current->data);
            struct timeval t_scan_start, t_scan_end;
            gettimeofday(&t_scan_start, NULL); //calcul temps du/des scan
            init_scan_configuration(opt, f_s);
            gettimeofday(&t_scan_end, NULL);
            double total_sec = (t_scan_end.tv_sec - t_scan_start.tv_sec)
                             + (t_scan_end.tv_usec - t_scan_start.tv_usec) / 1000000.0;

            printf("Scan took %.5f secs\n", total_sec);
			print_conclusion(f_s);
			f_s = NULL;
        }
        current = current->next;
    }
    return EXIT_SUCCESS;
}

int main(const int ac, const char **av) {
    t_opt 			*opt = parsing(av, ac);
	t_final_status	*final_status = NULL;

    if (!opt)
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


// // Pseudo-code pour ft_nmap
// int main(int argc, char **argv) {
//     parse_arguments(argc, argv);                 // 1. --ip, --ports, --file, --scan, --speedup
//     resolve_targets();                           // 2. Récupère la ou les IPs à scanner
    
//     check_host_availability();                   // 3. Ping / SYN / ACK pour voir si les hôtes sont UP
//     //perform_reverse_dns();                       // 4. Résolution DNS inverse (facultatif mais utile)
    
//     init_scan_configuration();                   // 5. Configure ports, scan type, threads
//     load_services_database();                    // 6. Mappe les ports vers des services connus (optionnel)
    
//     start_timer();                               // 7. Démarre le chrono
    
//     foreach target_ip in targets {
//         create_threads_for_target(target_ip);    // 8. Lance les threads pour ce target
//     }

//     wait_for_threads_to_finish();                // 9. Attend la fin de tous les threads
    
//     stop_timer();                                // 10. Stop chrono
//     print_results();                             // 11. Affiche les résultats
//     return 0;
// }
