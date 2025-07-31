/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:38:12 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/28 21:32:38 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

#include <netinet/ip_icmp.h> //pour macOS

// Calcule le checksum ICMP
unsigned short checksum(void *buf, int len) {
    unsigned short *ptr = buf;
    unsigned int sum = 0;

    for (; len > 1; len -= 2)
        sum += *ptr++;
    if (len == 1)
        sum += *(unsigned char*)ptr;

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

// Envoie le paquet ICMP et attend la réponse (avec timeout 1s)
int send_and_receive(int sockfd, const struct sockaddr_in *addr, const char *packet, int packet_len) {
    if (sendto(sockfd, packet, packet_len, 0, (const struct sockaddr *)addr, sizeof(*addr)) <= 0) {
        perror("sendto");
        return 0;
    }

    struct timeval timeout = {1, 0};
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    char recv_buf[1024];
    socklen_t addr_len = sizeof(*addr);
    ssize_t bytes_received = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0,
                                      (struct sockaddr *)addr, &addr_len);
    return (bytes_received > 0);
}


// Prépare le paquet ICMP Echo Request, retourne la taille du paquet prêt à être envoyer
int prepare_icmp_packet(char *packet, size_t packet_size) {
    struct icmp icmp_hdr = {0}; //Initialise en-tête ICMP a 0
    icmp_hdr.icmp_type = ICMP_ECHO;
    icmp_hdr.icmp_code = 0;
    icmp_hdr.icmp_id = getpid() & 0xFFFF;
    icmp_hdr.icmp_seq = 1;

	// vérifie que le buffer icmp_hdr est assez grand pour contenir l'en-tête
    if (packet_size < sizeof(icmp_hdr))
        return 0;

    memcpy(packet, &icmp_hdr, sizeof(icmp_hdr));

    int total_len = sizeof(icmp_hdr);
	// Calcule le checksum du paquet pour verif l'intégrité
    // l'insère dans l'en-tête ICMP du buffer
    ((struct icmp *)packet)->icmp_cksum = checksum(packet, total_len);

    return total_len;
}


// Init sockaddr_in à partir de l'ip en string current->data
int init_sockaddr_in(struct sockaddr_in *addr, const char *ip_str) {
    memset(addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    if (inet_pton(AF_INET, ip_str, &addr->sin_addr) != 1) {
        fprintf(stderr, "Invalid IP address: %s\n", ip_str);
        return 0;
    }
    return 1;
}


int check_host_availability(const char *ip_str) {
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
		perror("socket");
        return 0;
	}

    struct sockaddr_in addr;
    if (!init_sockaddr_in(&addr, ip_str)) {
        close(sockfd);
        return 0;
    }

    char packet[64];
    int packet_len = prepare_icmp_packet(packet, sizeof(packet));
    if (!packet_len) {
        close(sockfd);
        fprintf(stderr, "Packet buffer too small\n");
        return 0;
    }

    int result = send_and_receive(sockfd, &addr, packet, packet_len);
    close(sockfd);
    return result;
}


int main(const int ac, const char **av) {
    t_opt *opt;

    if (!(opt = parsing(av, ac)))
        return (EXIT_FAILURE);
    print_opt(opt);

    t_list_addr *current = opt->targets;

    while (current) {
        // current->data contient les IP
        if (!check_host_availability(current->data)) {
            printf("Host %s is unreachable.\n", current->data);
        } else {
            printf("Host %s is up.\n", current->data);
        }
        current = current->next;
    }

	init_scan_configuration(opt);

    free_opt(opt);
    return (EXIT_SUCCESS);
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
