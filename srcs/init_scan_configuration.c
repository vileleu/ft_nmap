#include "ft_nmap.h"

int get_total_ports(t_port *port) {
    if (!port)
        return 0;

    if (port->isranged)
        return (port->max - port->min + 1);

    if (port->islist) {
        int count = 0;
        t_list *tmp = port->list;
        while (tmp) {
            count++;
            tmp = tmp->next;
        }
        return count;
    }

    return 0; // aucun port défini
}


void init_scan_configuration(t_opt *opt) {

    // Calculer le nombre total de ports à scanner pour répartir le scan entre les threads
    int total_ports = get_total_ports(&opt->port);
    //printf("Nombre total de ports à scanner : %d\n", total_ports);

    // Répartition des ports entre les threads
    int ports_per_thread = total_ports / opt->thread;
    int reste = total_ports % opt->thread;
    // printf("Répartition des ports :\n");
    // printf("- %d ports par thread\n", ports_per_thread);
    // printf("- %d threads auront 1 port supplémentaire (pour équilibrer)\n", reste);
    // for (int i = 0; i < opt->thread; i++) {
    //     int ports_for_this_thread = ports_per_thread + (i < reste ? 1 : 0);
    //     printf("Thread %d → %d port(s)\n", i + 1, ports_for_this_thread);
    // }
    
    // Allocation d’un tableau t_thread_data et lancement des threads avec cette structure
}

