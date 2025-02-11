#include "../include/ft_malcolm.h"
#include "../Libft/src/libft.h"

void	ft_reset_arp(t_info *info)
{
	char				buff[42] = {0};
	struct ether_header	*eth;
	struct ether_arp	*arp;
	struct sockaddr_ll	dev;
	u_char				source_mac[MAC_ADDR_LEN] = {0};
	int					sockfd;
	int					interval;

	eth = (struct ether_header *)buff;
	arp = (struct ether_arp *)(buff + sizeof(struct ether_header));
	interval = 0;
	sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (sockfd < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	if (sscanf(info->gateway_mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
		&source_mac[0], &source_mac[1], &source_mac[2],
		&source_mac[3], &source_mac[4], &source_mac[5]) != MAC_ADDR_LEN)
	{
		fprintf(stderr, "Invalid MAC address\n");
		exit(EXIT_FAILURE);
	}
	ft_memset(&dev, 0, sizeof(struct sockaddr_ll));
	dev.sll_family = AF_PACKET;
	dev.sll_ifindex = if_nametoindex(info->dev);
	dev.sll_halen = ETH_ALEN;
	while (interval < 15)
	{
		ft_set_hdrs(eth, arp, source_mac, info->ip_target);
		printf(TC_GRN "Restoring ARP cache of %s\n" TC_NRM, info->ip_target);
		if (sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&dev, sizeof(struct sockaddr_ll)) < 0)
		{
			perror("sendto");
			exit(EXIT_FAILURE);
		}
		sleep(1);
		interval++;
	}
	close(sockfd);
}
