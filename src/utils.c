/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irifarac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:18:27 by irifarac          #+#    #+#             */
/*   Updated: 2025/02/10 13:32:59 by irifarac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_malcolm.h"
#include "../Libft/src/libft.h"

static void	ft_check_ip(t_info *info, struct ether_arp *arp_var)
{
	if (inet_pton(AF_INET, info->ip_src, &arp_var->arp_spa) != 1)
	{
		fprintf(stderr, "Invalid source IP address\n");
		exit (1);
	}
	if (inet_pton(AF_INET, info->ip_target, &arp_var->arp_tpa) != 1)
	{
		fprintf(stderr, "Invalid target IP address\n");
		exit (1);
	}
}

static void	ft_check_mac(t_info *info, unsigned char *src_mac, unsigned char *dst_mac)
{
	if (sscanf(info->mac_src, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
		&src_mac[0], &src_mac[1], &src_mac[2], &src_mac[3], &src_mac[4], &src_mac[5]) != MAC_ADDR_LEN)
	{
		fprintf(stderr, "Invalid source MAC address\n");
		exit (1);
	}
	if (sscanf(info->mac_target, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
		&dst_mac[0], &dst_mac[1], &dst_mac[2], &dst_mac[3], &dst_mac[4], &dst_mac[5]) != MAC_ADDR_LEN)
	{
		fprintf(stderr, "Invalid target MAC address\n");
		exit (1);
	}
}

static void	ft_find_interface_by_ip(char *dev, t_info *info)
{
	struct ifaddrs		*ifaddr;
	struct ifaddrs		*ifa;
	struct sockaddr_in	*addr;
	char				ip[INET_ADDRSTRLEN] = {0};
	int					src_len;

	if (getifaddrs(&ifaddr) == -1)
	{
		perror("getifaddrs");
		exit (1);
	}
	src_len = ft_strlen(info->ip_src);
	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	{
		if (ifa->ifa_addr == NULL ||
				ifa->ifa_addr->sa_family != AF_INET)
			continue ;
		addr = (struct sockaddr_in *)ifa->ifa_addr;
		inet_ntop(AF_INET, &addr->sin_addr, ip, INET_ADDRSTRLEN);
		if (ft_strncmp(ip, info->ip_src, src_len) == 0)
		{
			ft_memcpy(dev, ifa->ifa_name, ft_strlen(ifa->ifa_name));
			freeifaddrs(ifaddr);
			return ;
		}
	}
	printf(TC_RED "No interface found\n" TC_NRM);
	exit (1);
}

void	ft_usage(void)
{
	fprintf(stderr, "Usage: ./ft_malcolm [target_ip] [target_mac]");
	fprintf(stderr, " [gateway_ip] [gateway_mac]\n");
}

void	ft_check_errors(int argc)
{
	if (getuid() != 0)
	{
		fprintf(stderr, "You must be root to run this program\n");
		exit (1);
	}
	if (argc < 2)
	{
		ft_usage();
		exit (0);
	}
}

void	ft_init(t_info *info, char **argv)
{
	char	dev[IFNAMSIZ] = {0};

	info->ip_src = argv[1];
	info->mac_src = argv[2];
	info->ip_target = argv[3];
	info->mac_target = argv[4];
	ft_find_interface_by_ip(dev, info);
	ft_memcpy(info->dev, dev, ft_strlen(dev));
}

void	ft_check_syntax(t_info *info)
{
	struct ether_arp	*arp_var;
	char				buffer[42] = {0};
	unsigned char		src_mac[MAC_ADDR_LEN] = {0};
	unsigned char		dst_mac[MAC_ADDR_LEN] = {0};

	arp_var = (struct ether_arp *)(buffer + sizeof(struct ether_header));
	ft_check_ip(info, arp_var);
	ft_check_mac(info, src_mac, dst_mac);
}
