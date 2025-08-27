/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_receive.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 10:45:54 by vileleu           #+#    #+#             */
/*   Updated: 2025/08/26 23:03:43 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scan.h"

static void	print_packet_info(const unsigned char *packet, const struct pcap_pkthdr packet_header) {
    printf("Packet capture length: %d\n", packet_header.caplen);
    printf("Packet total length %d\n", packet_header.len);
	printf("packet = |%s|\n", packet);
    return ;
}

uint8_t			scan_receive(uint16_t source, uint16_t count) {
	pcap_if_t			*list_if = NULL;
	pcap_if_t			*tmp = NULL;
    char				error_buffer[PCAP_ERRBUF_SIZE];
	pcap_t				*handle = NULL;
    int					timeout_limit = 10000; // ms
    const unsigned char	*packet = NULL;
    struct pcap_pkthdr	packet_header;

	(void)source;
	(void)count;
	// search device (ex: eth0)
    if ((pcap_findalldevs(&list_if, error_buffer)) < 0)
		return (error_scan_pcap(list_if, handle, "Error finding device", NULL, error_buffer));
	tmp = list_if;
	while (tmp) {
		printf("name = %s, description = %s\n", tmp->name, tmp->description);
		tmp = tmp->next;
	}
    // open device for live capture
    if (!(handle = pcap_open_live(list_if->name, BUFSIZ, 1, timeout_limit, error_buffer)))
		return (error_scan_pcap(list_if, handle, "Could not open device", list_if->name, error_buffer));
	if (!(packet = pcap_next(handle, &packet_header)))
        return (error_scan_pcap(list_if, handle, "No packet found", NULL, NULL));
	print_packet_info(packet, packet_header);
	pcap_close(handle);
	pcap_freealldevs(list_if);
	/*
	// pcap loop
    pcap_loop(handle, 0, packet_handler, NULL);
	*/
	return (EXIT_SUCCESS);
}