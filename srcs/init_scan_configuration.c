/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_scan_configuration.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 19:32:58 by vileleu           #+#    #+#             */
/*   Updated: 2025/08/26 21:45:45 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

void free_port_list(t_list *list) {
    t_list *tmp;
    while (list) {
        tmp = list;
        list = list->next;
        free(tmp->data);
        free(tmp);
    }
}

void cleanup_threads(int thread_count, pthread_t *threads, t_thread_data *threads_data) {
    for (int i = 0; i < thread_count; i++) {
        free_port_list(threads_data[i].ports);
    }
    free(threads_data);
    free(threads);
}

void wait_for_threads(int thread_count, pthread_t *threads) {
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }
}

void *scan_thread(void *arg) {
    t_thread_data *data = (t_thread_data *)arg;
	uint16_t	count = 0;
    // Exemple basique pour test
    //printf("Thread started for IP %s\n", data->ip);
	if (!(count = scan_send(&data->addr, data->ports, data->scan, data->source)))
		return NULL;
	if (scan_receive(data->source, count))
		return NULL;
	//(void)data;
    return NULL;
}

pthread_t *launch_threads(t_opt *opt, t_thread_data *threads_data) {
    // On malloc pour créer dynamiquement un tableau qui stockera les identifiants (`pthread_t`)
    // de tous les threads pour les wait
    pthread_t *threads = malloc(sizeof(pthread_t) * opt->thread);
    if (!threads) {
        //free(threads);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < opt->thread; i++) {
        pthread_create(&threads[i], NULL, scan_thread, &threads_data[i]); //scan_thread = appelle la fonction avce tous les scans
    }

    return threads;
}

// void print_port_list(t_list *ports) {
//     t_list *tmp = ports;
//     while (tmp) {
//         printf("------------> %d", *(uint16_t *)tmp->data);
//         tmp = tmp->next;
//     }
//     printf("\n");
// }


//list chaînée 
t_list *copy_ports(t_list *all_ports, int start, int count) {
    for (int i = 0; i < start && all_ports; i++)
        all_ports = all_ports->next;

    t_list *result = NULL;
    t_list *tail = NULL;

    for (int i = 0; i < count && all_ports; i++) {
        t_list *new_node = malloc(sizeof(t_list));
        if (!new_node) {
            free_port_list(result);
            exit(EXIT_FAILURE);
        }
        new_node->data = malloc(sizeof(uint16_t));
        if (!new_node->data) {
            free_port_list(result);
            exit(EXIT_FAILURE);
        }
        *(new_node->data) = *(all_ports->data);
        new_node->next = NULL;

        if (!result)
            result = new_node;
        else
            tail->next = new_node;
        tail = new_node;

        all_ports = all_ports->next;
    }

    return result;
}

uint8_t			get_total_scan(uint8_t scan[SIZE_SCAN]) {
	uint8_t		i = 0;

	while (scan[i] && i < SIZE_SCAN)
		i++;
	return (i);
}

uint16_t		get_source_port() {
    return (uint16_t)(MIN_PORT_SOURCE + (rand() % (MAX_PORT_SOURCE - MIN_PORT_SOURCE + 1)));
}

t_thread_data *allocate_thread_data(t_opt *opt, t_list *all_ports, int total_ports) {
    int base_ports_per_thread = total_ports / opt->thread;
    int extra_ports = total_ports % opt->thread;
	uint16_t	source = get_source_port();
	uint8_t		total_scan = get_total_scan(opt->scan);

    // car ex : 2 / 10 = 0
    if (total_ports < opt->thread || !opt->thread) {
        printf("\nInsufficient ports per thread\n");
        exit(EXIT_FAILURE);
    }

    int base_ports_per_thread = total_ports / opt->thread;
    int extra_ports = total_ports % opt->thread;

    // debug a commenter
    printf("Répartition des ports :\n");
    printf("- %d ports par thread\n", base_ports_per_thread);
    printf("- %d thread(s) avec port supplémentaire (pour équilibrer)\n", extra_ports);

    t_thread_data *threads_data = malloc(sizeof(t_thread_data) * opt->thread);
    if (!threads_data) {
        //free(threads_data);
        exit(EXIT_FAILURE);
    }

    // Répartition des ports par thread
    int port_index = 0; //ex : -> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10
    for (int thread_index = 0; thread_index < opt->thread; thread_index++) {
        int ports_for_this_thread = base_ports_per_thread;
        if (thread_index < extra_ports)  // Les premiers threads dans extra_ports obtiennent un port supplémentaire
            ports_for_this_thread += 1;

        //remplissage du thread
        threads_data[thread_index].addr = opt->targets->addr; // Assignation de l'IP cible à scanner pour ce thread
        threads_data[thread_index].ports = copy_ports(all_ports, port_index, ports_for_this_thread);// Copie la sous-liste de ports pour ce thread, ex : -> 0 | 1 | 2 | 3 | 4 | 5 |
		threads_data[thread_index].source = source;
		source += base_ports_per_thread * total_scan;
		memcpy(threads_data[thread_index].scan, opt->scan, sizeof(uint8_t) * SIZE_SCAN);
        port_index += ports_for_this_thread;
        printf("Thread %d → %d port(s)\n", thread_index + 1, ports_for_this_thread);
        //debug //print_port_list(threads_data[thread_index].ports);

    }

    return threads_data;
}


// crée une liste chaînée de ports à partir de l'intervalle donné pour creer ls threads
t_list *create_list_from_range(uint16_t min, uint16_t max) {
    t_list *head = NULL;
    t_list *last_node = NULL;

    for (uint16_t port = min; port <= max; port++) {
        t_list *new_node = malloc(sizeof(t_list)); //alloue dynamiquement un nouveau nœud de la liste
        if (!new_node) {
            free_port_list(head);
            exit(EXIT_FAILURE);
        }
        new_node->data = malloc(sizeof(uint16_t)); // Le champ data pointe vers un entier (uint16_t) contenant la valeur du port courant
        if (!new_node) {
            free_port_list(head);
            free(new_node);
            exit(EXIT_FAILURE);
        }
        *(new_node->data) = port; //On copie la valeur du port dans ce champ
        new_node->next = NULL;

        // Si c’est le premier nœud, on initialise head.
        //Sinon, on chaîne ce nouveau nœud à la fin de la liste.
        if (!head)
            head = new_node;
        else
            last_node->next = new_node;
        last_node = new_node;
    }

    return head;
}

//preparer pour allocate_thread_data
t_list *prepare_all_ports(t_port *port) {
    if (port->isranged)
        return create_list_from_range(port->min, port->max);
    if (port->islist)
        return port->list;
    return NULL;
}

int get_total_ports(t_port *port) {

    if (port->isranged) {//retourne le nombre total d’éléments dans la plage
        return (port->max - port->min + 1);
    }

    if (port->islist) { //compter les éléments de la liste chaînée
        int count = 0;
        t_list *tmp = port->list;
        while (tmp) {
            count++;
            tmp = tmp->next;
        }
        return count;
    }
    else { // car 1 port ne rentre ni dans plage ni dans liste chaînée
        int nb_port = 1;
        return nb_port;
    }

    return 0;
}

void init_scan_configuration(t_opt *opt) {
    int total_ports = get_total_ports(&opt->port);
    t_list *all_ports = prepare_all_ports(&opt->port);
    t_thread_data *threads_data = allocate_thread_data(opt, all_ports, total_ports);
    pthread_t *threads = launch_threads(opt, threads_data); //lancement des threads

    wait_for_threads(opt->thread, threads);
    cleanup_threads(opt->thread, threads, threads_data);

    if (opt->port.isranged)
        free_port_list(all_ports);
}
