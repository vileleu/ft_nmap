/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:38:12 by vileleu           #+#    #+#             */
/*   Updated: 2025/09/08 17:26:58 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

static int run_scan(t_opt *opt, t_final_status *f_s) {
    t_list_addr *current = opt->targets;

    while (current) {
        // current->data contient les IP
        if (!check_host_availability(current->data)) {
            printf("Host %s is unreachable.\n", current->data);
        } else {
            printf("Host %s is up.\n", current->data);
            init_scan_configuration(opt, f_s);
        }
        current = current->next;
    }
    return 0;
}

int main(const int ac, const char **av) {
    t_opt 			*opt = parsing(av, ac);
	t_final_status	*final_status = NULL;

    if (!opt)
        return EXIT_FAILURE;
    print_opt(opt);
	if (create_final_status(&final_status, opt)) {
		free_final_status(final_status);
		free_opt(opt);
		return EXIT_FAILURE;
	}
    srand(time(NULL));
    run_scan(opt, final_status);
	print_conclusion(final_status);
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
