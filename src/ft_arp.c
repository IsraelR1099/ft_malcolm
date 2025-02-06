#include "../include/ft_malcolm.h"

static void	ft_recv(int sock, char *recv_buffer, size_t buf_size)
{
	struct sockaddr_ll	recv_device;
	socklen_t			recv_device_len;
	int					len;

	recv_device_len = sizeof(recv_device);
	len = recvfrom(
			sock, recv_buffer, buf_size, 0,
			(struct sockaddr *)&recv_device,
			&recv_device_len);
	if (len < 0)
	{
		perror("recvfrom error: ");
		return ;
	}
	printf("Received packet: %d bytes\n", len);
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
	printf("ARP Spoofing is started\n");
	while (!stop)
	{
		ft_recv(sock, recv_buffer, sizeof(recv_buffer));
		recv_eth = (struct ether_header *)recv_buffer;
		recv_arp = (struct ether_arp *)(recv_buffer + sizeof(struct ether_header));
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
		if (ntohs(recv_eth->ether_type) == ETH_P_ARP &&
				ntohs(recv_arp->ea_hdr.ar_op) == ARPOP_REQUEST)
		{
			printf("Receivde ARP request from %s\n", inet_ntoa(*(struct in_addr *)&recv_arp->arp_spa));
		}
		sleep(1);
	}
	printf(TC_RED "ARP Spoofing is stopped\n" TC_NRM);
	close(sock);
}
