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
    (void)arg;
    t_thread_data *data = (t_thread_data *)arg;
    // Exemple basique pour test
    printf("Thread started for IP %s\n", data->ip);
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


//list chaînée 
t_list *copy_ports(t_list *source_ports, int start_index, int nb_ports_to_copy) {
    t_list *copied_list_head = NULL;   // Tête de la nouvelle liste copiée
    t_list *copied_list_tail = NULL;   // Queue """
    int current_index = 0;             // Position actuelle dans la liste source
    
    //ex : start_index = 5 et ports_to_copy = 5, donc aller jusqu'a la place 10 
    while (source_ports && current_index < start_index + nb_ports_to_copy) { 
        if (current_index >= start_index) { //je copie  source_ports->data seulement lorsque j'arrive sur le debut de la sous-liste
            // Création d'un nouveau nœud
            t_list *new_port_node = malloc(sizeof(t_list));
            if (!new_port_node) {
                free_port_list(copied_list_head);
                exit(EXIT_FAILURE);
            }

            // creation du type a l'interiteur
            new_port_node->data = malloc(sizeof(uint16_t));
            if (!new_port_node->data) {
                free_port_list(copied_list_head);
                exit(EXIT_FAILURE);
            }
            *(new_port_node->data) = *(source_ports->data); //copier
            new_port_node->next = NULL;

            if (!copied_list_head) {
                copied_list_head = new_port_node;
            } else {
                copied_list_tail->next = new_port_node;
            }
            copied_list_tail = new_port_node; // Mise à jour du dernier élément
        }
        source_ports = source_ports->next;
        current_index++;
    }

    return copied_list_head;
}


t_thread_data *allocate_thread_data(t_opt *opt, t_list *all_ports, int total_ports) {
    int base_ports_per_thread = total_ports / opt->thread;
    int extra_ports = total_ports % opt->thread;

    // car ex : 2 / 10 = 0
    if (total_ports < opt->thread){
        printf("\nInsufficient ports per thread\n");
        exit(EXIT_FAILURE);
    }

    // debug a commenter
    printf("Répartition des ports :\n");
    printf("- %d ports par thread\n", base_ports_per_thread);
    printf("- %d threads auront 1 port supplémentaire (pour équilibrer)\n", extra_ports);

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
        threads_data[thread_index].ip = opt->targets->data; // Assignation de l'IP cible à scanner pour ce thread
        threads_data[thread_index].ports = copy_ports(all_ports, port_index, ports_for_this_thread);// Copie la sous-liste de ports pour ce thread, ex : -> 0 | 1 | 2 | 3 | 4 | 5 |
        memcpy(threads_data[thread_index].scan, opt->scan, sizeof(uint8_t) * 6);
        port_index += ports_for_this_thread;
        printf("Thread %d → %d port(s)\n", thread_index + 1, ports_for_this_thread);
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
