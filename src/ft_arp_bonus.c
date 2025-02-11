#include "../include/ft_malcolm.h"
#include "../Libft/src/libft.h"

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
		if (ntohs(recv_eth->ether_type) == ETH_P_ARP &&
				ntohs(recv_arp->ea_hdr.ar_op) == ARPOP_REQUEST)
		{
			printf(TC_GRN"ARP Request received\n"TC_NRM);
			ft_memcpy(spa, inet_ntoa(*(struct in_addr *)&recv_arp->arp_spa), 16);
			if (ft_strncmp(spa, info.ip_target, ft_strlen(info.ip_target)) == 0)
			{
				printf(TC_GRN"ARP Reply sent\n"TC_NRM);
			}
		}
		sleep(1);
	}
}
