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



void *scan_thread(void *arg) {
    (void)arg;
    t_thread_data *data = (t_thread_data *)arg;

    // Exemple basique pour test
    printf("Thread lancé pour IP %s\n", data->ip);
    return NULL;
}

pthread_t *launch_threads(t_opt *opt, t_thread_data *threads_data) {
    pthread_t *threads = malloc(sizeof(pthread_t) * opt->thread);
    if (!threads)
        exit(EXIT_FAILURE);

    for (int i = 0; i < opt->thread; i++) {
        pthread_create(&threads[i], NULL, scan_thread, &threads_data[i]);
    }

    return threads;
}

void wait_for_threads(int thread_count, pthread_t *threads) {
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }
}

void cleanup_threads(int thread_count, pthread_t *threads, t_thread_data *threads_data) {
    for (int i = 0; i < thread_count; i++) {
        free_port_list(threads_data[i].ports);
    }
    free(threads_data);
    free(threads);
}

t_list *copy_ports(t_list *all_ports, int start, int count) {
    t_list *result = NULL;
    t_list *tail = NULL;
    int index = 0;

    while (all_ports && index < start + count) {
        if (index >= start) {
            t_list *new_node = malloc(sizeof(t_list));
            if (!new_node) {
                free_port_list(new_node);
                exit(EXIT_FAILURE);
            }
            new_node->data = malloc(sizeof(uint16_t));
            *(new_node->data) = *(all_ports->data);
            new_node->next = NULL;

            if (!result)
                result = new_node;
            else
                tail->next = new_node;
            tail = new_node;
        }
        all_ports = all_ports->next;
        index++;
    }

    return result;
}

t_thread_data *allocate_thread_data(t_opt *opt, t_list *all_ports, int total_ports) {
    //Répartition des ports par thread
    int ports_per_thread = total_ports / opt->thread; //opt->thread = nb thread
    int reste = total_ports % opt->thread;

    printf("Répartition des ports :\n");
    printf("- %d ports par thread\n", ports_per_thread);
    printf("- %d threads auront 1 port supplémentaire (pour équilibrer)\n", reste);

    t_thread_data *threads_data = malloc(sizeof(t_thread_data) * opt->thread); //qlloue dynamiquement un tableau contenant une structure t_thread_data par thread
    if (!threads_data) {
        free_port_list(threads_data);
        exit(EXIT_FAILURE);
    }
    int port_index = 0;
    for (int i = 0; i < opt->thread; i++) {
        int count = ports_per_thread;
        if (i < reste)
            count += 1;
        threads_data[i].ip = opt->targets->data; //associe à chaque thread l’IP cible à scanner 
        threads_data[i].ports = copy_ports(all_ports, port_index, count); //copie une portion de la liste de ports pour ce thread
        memcpy(threads_data[i].scan, opt->scan, sizeof(uint8_t) * 6);
        port_index += count;
        printf("Thread %d → %d port(s)\n", i + 1, count);
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
    if (!port)
        return 0;

    if (port->isranged) //retourne le nombre total d’éléments dans la plage
        return (port->max - port->min + 1);

    if (port->islist) { //compter les éléments de la liste chaînée
        int count = 0;
        t_list *tmp = port->list;
        while (tmp) {
            count++;
            tmp = tmp->next;
        }
        return count;
    }

    return 0;
}

void init_scan_configuration(t_opt *opt) {
    int total_ports = get_total_ports(&opt->port);
    t_list *all_ports = prepare_all_ports(&opt->port);
    t_thread_data *threads_data = allocate_thread_data(opt, all_ports, total_ports);
    pthread_t *threads = launch_threads(opt, threads_data);

    wait_for_threads(opt->thread, threads);
    cleanup_threads(opt->thread, threads, threads_data);

    if (opt->port.isranged)
        free_port_list(all_ports);
}


// int get_total_ports(t_port *port) {
//     if (!port)
//         return 0;

//     if (port->isranged)
//         return (port->max - port->min + 1);

//     if (port->islist) {
//         int count = 0;
//         t_list *tmp = port->list;
//         while (tmp) {
//             count++;
//             tmp = tmp->next;
//         }
//         return count;
//     }

//     return 0; // aucun port défini
// }


// void init_scan_configuration(t_opt *opt) {

//     // Calculer le nombre total de ports à scanner pour répartir le scan entre les threads
//     int total_ports = get_total_ports(&opt->port);
//     printf("Nombre total de ports à scanner : %d\n", total_ports);

//     // Répartition des ports entre les threads
//     int ports_per_thread = total_ports / opt->thread;
//     int reste = total_ports % opt->thread;
//     printf("Répartition des ports :\n");
//     printf("- %d ports par thread\n", ports_per_thread);
//     printf("- %d threads auront 1 port supplémentaire (pour équilibrer)\n", reste);
//     for (int i = 0; i < opt->thread; i++) {
//         int ports_for_this_thread = ports_per_thread + (i < reste ? 1 : 0);
//         printf("Thread %d → %d port(s)\n", i + 1, ports_for_this_thread);
//     }
    
//     // Allocation d’un tableau t_thread_data et lancement des threads avec cette structure
// }

