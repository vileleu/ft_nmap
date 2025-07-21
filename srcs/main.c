/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:38:12 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/17 13:38:48 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

int	main() {
	printf("ft_nmap\n");
	return (0);
}

/*Pseudo-code global pour ft_nmap
MAIN()
{
    parse_arguments(argc, argv);                    // --ip, --ports, --file, --scan, --speedup
    resolve_targets();                              // Lire IP ou fichier, stocker les IPs
    
    init_scan_configuration();                      // Ports à scanner, types de scans, nombre de threads
    load_services_database();                       // Pour associer les ports à des services (optionnel)

    start_timer();                                  // Pour mesurer le temps du scan

    foreach target_ip in targets {
        create_threads_for_target(target_ip);       // Crée des threads selon --speedup
    }

    wait_for_threads_to_finish();

    stop_timer();
    print_results();
}*/