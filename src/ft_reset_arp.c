#include "../include/ft_malcolm.h"
#include "../Libft/src/libft.h"

static void	ft_set_hdrs(struct ether_header *eth, struct ether_arp *arp, u_char *src_mac, char *ip_target)
{
	char	gateway_ip[16] = {0};

	ft_get_gateway(gateway_ip);
	ft_memset(eth->ether_dhost, 0xff, ETH_ALEN);
	ft_memcpy(eth->ether_shost, src_mac, ETH_ALEN);
	eth->ether_type = htons(ETH_P_ARP);

	arp->arp_hrd = htons(ARPHRD_ETHER);
	arp->arp_pro = htons(ETH_P_IP);
	arp->arp_hln = ETH_ALEN;
	arp->arp_pln = 4;
	arp->arp_op = htons(ARPOP_REPLY);

	ft_memcpy(arp->arp_sha, src_mac, ETH_ALEN);
	if (inet_pton(AF_INET, gateway_ip, arp->arp_spa) <= 0)
	{
		perror("inet_pton()");
		exit (1);
	}
	ft_memset(arp->arp_tha, 0, ETH_ALEN);
	if (inet_pton(AF_INET, ip_target, arp->arp_tpa) <= 0)
	{
		perror("inet_pton()");
		exit (1);
	}
}

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
