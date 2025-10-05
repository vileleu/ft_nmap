#include "ft_nmap.h"

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

static int	prepare_timestamp_packet(char *packet, size_t packet_size) {
    struct icmp icmp_hdr = {0};
    icmp_hdr.icmp_type = ICMP_TSTAMP;
    icmp_hdr.icmp_code = 0;
    icmp_hdr.icmp_id = getpid() & 0xFFFF;
    icmp_hdr.icmp_seq = 0;
    if (packet_size < sizeof(icmp_hdr))
        return 0;
    memcpy(packet, &icmp_hdr, sizeof(icmp_hdr));
    int total_len = sizeof(icmp_hdr);
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


uint8_t tcp_ping(const char *ip_str) {
    t_list *only443 = NULL;
    t_list *only80 = NULL;
    uint16_t source = get_source_port();
    uint8_t scan[SIZE_SCAN] = {0};
    struct sockaddr_in dst;

    if (!init_sockaddr_in(&dst, ip_str))
        return 0;

    // allocation port 443
    if (!(only443 = malloc(sizeof(t_list))))
        return (error_all_errno("malloc"));
    if (!(only443->data = malloc(sizeof(uint16_t))))
        return (error_all_errno("malloc"));
    *(uint16_t *)(only443->data) = 443;
    only443->next = NULL;

    // allocation port 80
    if (!(only80 = malloc(sizeof(t_list))))
        return (error_all_errno("malloc"));
    if (!(only80->data = malloc(sizeof(uint16_t))))
        return (error_all_errno("malloc"));
    *(uint16_t *)(only80->data) = 80;
    only80->next = NULL;

    // envoi SYN sur 443
    scan[0] = SYN;
    scan_send(&dst, only443, source, scan);

    // envoi ACK sur 80
    scan[0] = ACK;
    scan_send(&dst, only80, source, scan);

    // libération mémoire
    free(only443->data);
    free(only443);
    free(only80->data);
    free(only80);

    return 1;
}


int icmp_ping(const char *ip_str) {
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

	packet_len = prepare_timestamp_packet(packet, sizeof(packet));
	if (!packet_len) {
        close(sockfd);
        fprintf(stderr, "Packet buffer too small\n");
        return 0;
    }
	send_and_receive(sockfd, &addr, packet, packet_len);
    close(sockfd);
    return result;
}


int check_host_availability(const char *ip_str) {
    if (icmp_ping(ip_str) == 0) {
        return 0;
    }
    if (tcp_ping(ip_str)) {
        return 1;
    }
    return 0;
}