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
#include "target.h"

#include <netinet/ip_icmp.h> //pour macOS

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

int check_host_availability(const char *ip_str) {
	int sockfd;
	struct sockaddr_in addr;
	struct icmp icmp_hdr;
	char packet[64];

	// ouvre un raw socket en mode ICMP
	if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
		perror("socket");
		return 0;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	if (inet_pton(AF_INET, ip_str, &addr.sin_addr) != 1) {
		fprintf(stderr, "Invalid IP address: %s\n", ip_str);
		close(sockfd);
		return 0;
	}

	// ICMP header
	memset(&icmp_hdr, 0, sizeof(icmp_hdr));
	icmp_hdr.icmp_type = ICMP_ECHO;
	icmp_hdr.icmp_code = 0;
	icmp_hdr.icmp_id = getpid() & 0xFFFF;
	icmp_hdr.icmp_seq = 1;

	// Payload (simple string)
	const char *payload = "ft_nmap_ping";
	memcpy(packet, &icmp_hdr, sizeof(icmp_hdr));
	strcpy(packet + sizeof(icmp_hdr), payload);

	int packet_size = sizeof(icmp_hdr) + strlen(payload);
	icmp_hdr.icmp_cksum = checksum(packet, packet_size);
	memcpy(packet, &icmp_hdr, sizeof(icmp_hdr));

	if (sendto(sockfd, packet, packet_size, 0,
	           (struct sockaddr *)&addr, sizeof(addr)) <= 0) {
		perror("sendto");
		close(sockfd);
		return 0;
	}

	struct timeval timeout = {1, 0};
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

	char recv_buf[1024];
	socklen_t addr_len = sizeof(addr);
	ssize_t bytes_received = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0,
	                                  (struct sockaddr *)&addr, &addr_len);
	close(sockfd);

	return (bytes_received > 0);
}


int resolve_target(t_opt *opt, t_target *target) {
    if (!opt || !opt->targets || !opt->targets->data)
        return (0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;

    // IP
    if (inet_pton(AF_INET, opt->targets->data, &addr.sin_addr) == 1) {
        strncpy(target->ip_str, opt->targets->data, INET_ADDRSTRLEN);
        target->ip_str[INET_ADDRSTRLEN - 1] = '\0';
        return (1);
    }

    //hostname
    struct addrinfo hints;
    struct addrinfo *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(opt->targets->data, NULL, &hints, &res) != 0)
        return (0);

    // Extrait et copie l'IP résolue
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    inet_ntop(AF_INET, &ipv4->sin_addr, target->ip_str, INET_ADDRSTRLEN);
    target->ip_str[INET_ADDRSTRLEN - 1] = '\0';

    freeaddrinfo(res);
    return (1);
}


int main(const int ac, const char **av) {
    t_opt *opt;

    if (!(opt = parsing(av, ac)))
        return (EXIT_FAILURE);
    print_opt(opt);

    t_list_addr *current = opt->targets;

    while (current) {
        t_target target;
        t_opt temp_opt = {0};

        temp_opt.targets = current;

        if (!resolve_target(&temp_opt, &target)) {
            fprintf(stderr, "Could not resolve target: %s\n", current->data);
        } else {
            if (!check_host_availability(target.ip_str)) {
                printf("Host %s is unreachable.\n", target.ip_str);
            } else {
                printf("Host %s is up.\n", target.ip_str);
            }
        }

        current = current->next;
    }

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
