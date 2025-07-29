/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_packets.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 11:28:52 by vileleu           #+#    #+#             */
/*   Updated: 2025/07/29 13:48:31 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nmap.h"

// SYN (open, close, filtered)
uint8_t     send_syn(uint16_t port) {
    struct tcphdr   packet;

    bzero(packet, sizeof(struct tcphdr));
    packet.syn = 1;
    return (EXIT_FAILURE);
}

// NULL (close, filtered, open|filtered)
uint8_t     send_null(uint16_t port) {
    struct tcphdr   packet;

    bzero(packet, sizeof(struct tcphdr));
    return (EXIT_FAILURE);
}

// ACK (filtered, unfiltered)
uint8_t     send_ack(uint16_t port) {
    struct tcphdr   packet;

    bzero(packet, sizeof(struct tcphdr));
    packet.ack = 1;
    return (EXIT_FAILURE);
}

// FIN (close, filtered, open|filtered)
uint8_t     send_fin(uint16_t port) {
    struct tcphdr   packet;

    bzero(packet, sizeof(struct tcphdr));
    packet.fin = 1;
    return (EXIT_FAILURE);
}

// XMAS (close, filtered, open|filtered)
uint8_t     send_xmas(uint16_t port) {
    struct tcphdr   packet;

    bzero(packet, sizeof(struct tcphdr));
    packet.fin = 1;
    packet.psh = 1;
    packet.urg = 1;
    return (EXIT_FAILURE);
}

// UDP (open, close, filtered, open|filtered)
uint8_t     send_udp(uint16_t port) {
    struct udphdr   packet;

    bzero(packet, sizeof(struct tcphdr));
	packet.source;
	packet.dest;
	packet.len;
	packet.check;
    return (EXIT_FAILURE);
}