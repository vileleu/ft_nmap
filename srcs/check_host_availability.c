#include "ft_nmap.h"

// additionne tous les octets du paquet 2 par 2 (en mots de 16 bits) 
// pour creer un code de 16 bits dans l’en-tete qui permet de 
// detecter toute alteration du paquet pendant le transport
unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (; len > 1; len -= 2) {
        sum += *buf++;
    }
    if (len == 1) {
        sum += *(unsigned char *)buf;
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum; // inverse tous les bits pour produire le checksum final
    return result;
}


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


int prepare_icmp_packet(char *packet, size_t packet_size) {
    struct icmp icmp_hdr = {0}; // initialise
    icmp_hdr.icmp_type = ICMP_ECHO; // ECHO request
    icmp_hdr.icmp_code = 0;         // ECHO request = 0
    icmp_hdr.icmp_id = getpid() & 0xFFFF; // identifiant : prend le PID du processus sur 16 bits car icmp_id = 16bits
    icmp_hdr.icmp_seq = 1;          // numero de sequence pour savoir a quel ping la request est liee


    if (packet_size < sizeof(icmp_hdr))
        return 0;

    memcpy(packet, &icmp_hdr, sizeof(icmp_hdr));

    int total_len = sizeof(icmp_hdr);
    ((struct icmp *)packet)->icmp_cksum = checksum(packet, total_len);

    return total_len;
}


int init_sockaddr_in(struct sockaddr_in *addr, const char *ip_str) {
    memset(addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    if (inet_pton(AF_INET, ip_str, &addr->sin_addr) == 1) {
        return 1;
    }

    struct addrinfo hints, *res = NULL;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(ip_str, NULL, &hints, &res) != 0 || res == NULL) {
        fprintf(stderr, "Failed to resolve: %s\n", ip_str);
        return 0;
    }

    memcpy(&addr->sin_addr, &((struct sockaddr_in *)res->ai_addr)->sin_addr, sizeof(struct in_addr));
    freeaddrinfo(res);

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
    close(sockfd);
    return result;
}


int check_host_availability(const char *ip_str) {
    if (icmp_ping(ip_str)) {
        return 1;
    }

    return 0;
}
