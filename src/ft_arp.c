#include "../include/ft_malcolm.h"

static void	ft_recv(int sock, char *recv_buffer)
{
	struct sockaddr_ll	recv_device;
	socklen_t			recv_device_len;
	int					len;

	recv_device_len = sizeof(recv_device);
	len = recvfrom(
			sock, recv_buffer, sizeof(recv_buffer), 0,
			(struct sockaddr *)&recv_device,
			&recv_device_len);
	if (len < 0)
	{
		perror("recvfrom");
		return ;
	}

}

void	ft_set_arp_spoof(t_info info)
{
	struct sockaddr_ll	device;
	struct ether_header	*recv_eth;
	struct ether_arp	*recv_arp;
	int					sock;
	char				buffer[42] = {0};
	char				recv_buffer[42] = {0};

	sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (sock < 0)
	{
		perror("socket");
		exit (1);
	}
	ft_set_device(&info, &device);
	while (!stop)
	{
		ft_recv(sock, recv_buffer);
		recv_eth = (struct ether_header *)recv_buffer;
		recv_arp = (struct ether_arp *)(recv_buffer + sizeof(struct ether_header));
		if (ntohs(recv_eth->ether_type) == ETH_P_ARP &&
				ntohs(recv_arp->ea_hdr.ar_op) == ARPOP_REQUEST)
		{
			printf("Receivde ARP request from %s\n", inet_ntoa(*(struct in_addr *)&recv_arp->arp_spa));
		}
	}
	printf(TC_RED "ARP Spoofing is stopped\n" TC_NRM);
	close(sock);
}
