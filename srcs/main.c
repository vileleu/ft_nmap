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
#include "target.h"

int resolve_targets(t_opt *opt, t_target targets[], int *target_count) {
    if (!opt || !targets || !target_count)
        return 0;

		printf("------> IP parsing target = %s\n", targets[1].ip_str);

    int count = 0;
    for (uint32_t i = 0; i < opt->len_targets && count < MAX_TARGETS; i++) {
        struct in_addr addr;
        if (inet_pton(AF_INET, opt->targets[i], &addr) == 1) {
            strncpy(targets[count].ip_str, opt->targets[i], INET_ADDRSTRLEN);
            targets[count].ip_str[INET_ADDRSTRLEN - 1] = '\0';

            // affiche ici la cible ajoutée
            printf("------> IP parsing target = %s\n", targets[count].ip_str);

            count++;
        } else {
            fprintf(stderr, "IP invalide ignorée : %s\n", opt->targets[i]);
        }
    }
    *target_count = count;
    return (count > 0);
}


void	print_opt(const char *name, t_opt *opt) {
	printf("%s:\n", name);
	// port
	(opt->port.isranged ? printf("port: %d-%d\n", opt->port.min, opt->port.max) : printf("port: %d\n", opt->port.port));
	// thread
	printf("thread: %d\n", opt->thread);
}

int main(const int ac, const char **av) {
    t_opt *opt;
    t_target targets[MAX_TARGETS];
    int target_count = 0;

    if (!(opt = parsing(av, ac)))
        return 1;
    print_opt(av[0] + 2, opt);

    if (!resolve_targets(opt, targets, &target_count)) {
        fprintf(stderr, "Could not resolve targets.\n");
		printf("----------> IP main = %s\n", targets->ip_str);
		printf("----------> IP C main = %d\n", target_count);
        free(opt);
        return 1;
    }

    // for (int i = 0; i < target_count; i++) {
    //     if (!check_host_availability(targets[i].ip_str)) {
    //         printf("Host %s is unreachable.\n", targets[i].ip_str);
    //     } else {
    //         printf("Host %s is up.\n", targets[i].ip_str);
    //     }
    // }

    free(opt);
    return 0;
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
