/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_scan_configuration.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 19:32:58 by vileleu           #+#    #+#             */
/*   Updated: 2025/09/08 16:58:33 by vileleu          ###   ########.fr       */
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

void	print_list_result(t_list_result *list) {
	while (list) {
		printf("port = %u, scan = %u\n", list->source, list->scan);
		list = list->next;
	}
}

void *scan_thread(void *arg) {
    t_thread_data *data = (t_thread_data *)arg;
	t_pcap_data	*p_data;

	if (!(p_data = init_pcap_data(data->ip, data->ports, data->source, data->count, data->scan)))
		return (NULL);

	if (scan_send(&data->addr, data->ports, data->source, data->scan)) {
		free_pcap_data(p_data);
		return NULL;
	}
	scan_receive(p_data, data->final_status);
	free_pcap_data(p_data);
    return (void *)1;
}

pthread_t *launch_threads(t_opt *opt, t_thread_data *threads_data) {
    pthread_t *threads = malloc(sizeof(pthread_t) * opt->thread);
    if (!threads) {
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < opt->thread; i++) {
        int res = pthread_create(&threads[i], NULL, scan_thread, &threads_data[i]); //scan_thread = appelle la fonction avce tous les scans
        if (res != 0) {
            for (int j = 0; j < i; j++)
                pthread_join(threads[j], NULL);
            free(threads);
            exit(EXIT_FAILURE);
        }
    }

    return threads;
}

// debug
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

t_thread_data *allocate_thread_data(t_opt *opt, t_list *all_ports, int total_ports, t_final_status *f_s) {
    if (opt->thread <= 0) {
        opt->thread = 1;
    }
	
 	// car ex : 2 / 10 = 0
    if (opt->thread > total_ports) { // ajustement pour eviter une erreur
        opt->thread = total_ports;
    }

    int base_ports_per_thread = total_ports / opt->thread;
    int extra_ports = total_ports % opt->thread;
    uint16_t	source = get_source_port();
	uint8_t		total_scan = get_total_scan(opt->scan);
	uint16_t	total_send = 0;

    // debug
    // printf("Répartition des ports :\n");
    // printf("- %d ports par thread\n", base_ports_per_thread);
    // printf("- %d thread(s) avec port supplémentaire (pour équilibrer)\n", extra_ports);

    t_thread_data *threads_data = malloc(sizeof(t_thread_data) * opt->thread);
    if (!threads_data) {
        return NULL;
    }

    // Répartition des ports par thread
    int port_index = 0; //ex : -> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10
    for (int thread_index = 0; thread_index < opt->thread; thread_index++) {
        int ports_for_this_thread = base_ports_per_thread;
        if (thread_index < extra_ports)  // Les premiers threads dans extra_ports obtiennent un port supplémentaire
            ports_for_this_thread += 1;

        //remplissage du thread
        threads_data[thread_index].addr = opt->targets->addr;
		threads_data[thread_index].ip = (const char *)opt->targets->data; // Assignation de l'IP cible à scanner pour ce thread
        threads_data[thread_index].ports = copy_ports(all_ports, port_index, ports_for_this_thread);// Copie la sous-liste de ports pour ce thread, ex : -> 0 | 1 | 2 | 3 | 4 | 5 |
		threads_data[thread_index].source = source;
		total_send = ports_for_this_thread * total_scan;
		threads_data[thread_index].count = total_send;
		threads_data[thread_index].final_status = f_s;
		source += total_send;
		memcpy(threads_data[thread_index].scan, opt->scan, sizeof(uint8_t) * SIZE_SCAN);
        port_index += ports_for_this_thread;
        printf("Thread %d → %d port(s)\n", thread_index + 1, ports_for_this_thread);
        //debug //print_port_list(threads_data[thread_index].ports);

    }

    return threads_data;
}


// liste chainee intervalle
t_list *create_list_from_range(uint16_t min, uint16_t max) {
    t_list *head = NULL;
    t_list *last_node = NULL;

    for (uint16_t port = min; port <= max; port++) {
        t_list *new_node = malloc(sizeof(t_list));
        if (!new_node) {
            free_port_list(head);
            exit(EXIT_FAILURE);
        }
        new_node->data = malloc(sizeof(uint16_t)); // Le champ data pointe vers un entier (uint16_t) contenant la valeur du port courant
        if (!new_node->data) {
            free_port_list(head);
            free(new_node);
            exit(EXIT_FAILURE);
        }
        *(new_node->data) = port; //On copie la valeur du port dans ce champ
        new_node->next = NULL;

        // Si c’est le premier nœud, on initialise head.
        //Sinon, on chaîne ce nouveau nœud à la fin de la liste
        if (!head)
            head = new_node;
        else
            last_node->next = new_node;
        last_node = new_node;
    }

    return head;
}


t_list *prepare_all_ports(t_port *port, int total_ports) {
    if (port->isranged)
        return create_list_from_range(port->min, port->max);

    if (port->islist)
        return port->list;

    if (total_ports == 1) { //cree une liste chainee de 1 -> cas particulier
        t_list *list = malloc(sizeof(t_list));
        if (!list) {
            free(list);
            exit(EXIT_FAILURE);
        }
        list->data = malloc(sizeof(uint16_t));
        if (!list->data) {
            free(list);
            exit(EXIT_FAILURE);
        }
        *(list->data) = port->port;
        list->next = NULL;
        return list;
    }

    return NULL;
}

int get_total_ports(t_port *port) {

    if (port->isranged) { // plage
        return (port->max - port->min + 1);
    }

    if (port->islist) { // liste chaînée
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

int init_scan_configuration(t_opt *opt, t_final_status *f_s) {
    int total_ports = get_total_ports(&opt->port);
    if (total_ports <= 0) {
        return 1; // aucun port a scanner
    }

    t_list *all_ports = prepare_all_ports(&opt->port, total_ports);
    if (!all_ports) {
        return 1; // pas de liste cree
    }

    t_thread_data *threads_data = allocate_thread_data(opt, all_ports, total_ports, f_s);
    if (!threads_data) {
        printf("Error: Failed to allocate thread data.\n");
        if (opt->port.isranged || total_ports == 1)
            free_port_list(all_ports);
        return 1;
    }
    pthread_t *threads = launch_threads(opt, threads_data);
    
    wait_for_threads(opt->thread, threads);
    cleanup_threads(opt->thread, threads, threads_data);

    if (opt->port.isranged || total_ports == 1) {
        free_port_list(all_ports);
    }

    return 0;
}
