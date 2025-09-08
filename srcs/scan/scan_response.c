/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_response.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vileleu <vileleu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 02:35:53 by vileleu           #+#    #+#             */
/*   Updated: 2025/09/01 18:17:55 by vileleu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scan.h"

static void	print_status(const t_port_status status) {
	if (status.open)
		printf("open");
	else if (status.closed)
		printf("closed");
	else if (status.filtered)
		printf("filtered");
	else if (status.open_filtered)
		printf("open_filtered");
	else if (status.unfiltered)
		printf("unfiltered");
	else
		return;
}

void	print_conclusion(t_list_result *result) {
	while (result) {
		printf("port source = %u, port destination = %u: ", result->source, result->dest);
		print_status(result->status);
		printf("\n");
		result = result->next;
	}
}

void	get_response_unreach(const unsigned char *packet, t_list_result *result) {
	const struct iphdr			*ip = (struct iphdr *)packet;
	const struct icmphdr		*icmp = (struct icmphdr *)(packet + (ip->ihl * 4));
	const struct iphdr			*myip = (struct iphdr *)(packet + (ip->ihl * 4 + sizeof(struct icmphdr)));
	const struct tcphdr			*tcp = (myip->protocol == IPPROTO_TCP ? (struct tcphdr *)(packet + (sizeof(ip->ihl * 4) + sizeof(struct icmphdr) + myip->ihl * 4)) : NULL);
	const struct udphdr			*udp = (myip->protocol == IPPROTO_UDP ? (struct udphdr *)(packet + (sizeof(ip->ihl * 4) + sizeof(struct icmphdr) + myip->ihl * 4)) : NULL);
	uint16_t					source = 0;
	
	source = (myip->protocol == IPPROTO_TCP ? ntohs(tcp->source) : source);
	source = (myip->protocol == IPPROTO_UDP ? ntohs(udp->uh_sport) : source);
	while (result) {
		if (result->source == source)
			break;
		result = result->next;
	}
	if (!result)
		return;
	bzero(&result->status, sizeof(t_port_status));
	if (result->scan == UDP) {
		if (icmp->code == 3)
			result->status.closed = 1;
		else if (icmp->code == 1 || icmp->code == 2 || icmp->code == 9 || icmp->code == 10 || icmp->code == 13)
			result->status.filtered = 1;
		else
			result->status.open_filtered = 1;
	}
	else {
		if (icmp->code == 1 || icmp->code == 2 || icmp->code == 3 || icmp->code == 9 || icmp->code == 10 || icmp->code == 13)
			result->status.filtered = 1;
		else {
			if (result->scan == SYN || result->scan == ACK)
				result->status.filtered = 1;
			else
				result->status.open_filtered = 1;
		}
	}
}

void	get_response(const unsigned char *packet, t_list_result *result) {
	const struct iphdr			*ip = (struct iphdr *)packet;
	const struct tcphdr			*tcp = (ip->protocol == IPPROTO_TCP ? (struct tcphdr *)(packet + (ip->ihl * 4)) : NULL);

	bzero(&result->status, sizeof(t_port_status));
	switch(result->scan) {
		case SYN:
			if (tcp->syn && tcp->ack)
				result->status.open = 1;
			else if (tcp->rst)
				result->status.closed = 1;
			else
				result->status.filtered = 1;
			return;
		case NUL:
			if (tcp->rst)
				result->status.closed = 1;
			else
				result->status.open_filtered = 1;
			return;
		case ACK:
			if (tcp->rst)
				result->status.unfiltered = 1;
			else
				result->status.filtered = 1;
			return;
		case FIN:
			if (tcp->rst)
				result->status.closed = 1;
			else
				result->status.open_filtered = 1;
			return;
		case XMAS:
			if (tcp->rst)
				result->status.closed = 1;
			else
				result->status.open_filtered = 1;
			return;
		case UDP:
			result->status.open = 1;
			return;
		default:
			return;
	}
}