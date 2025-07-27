
#ifndef TARGET_H
# define TARGET_H

# include <netinet/in.h> // pour struct in_addr
# include <arpa/inet.h>  // pour INET_ADDRSTRLEN

//#define MAX_TARGETS 250 // car limite de 250 threads max, et 1 thread par IP

typedef struct s_target {
	char ip_str[INET_ADDRSTRLEN]; 
	struct in_addr ip_addr;       // Adresse IP binaire (pour envoi de paquets)
} t_target;

#endif
