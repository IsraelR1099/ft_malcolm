/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irifarac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:10:17 by irifarac          #+#    #+#             */
/*   Updated: 2025/02/15 14:37:12 by israel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_malcolm.h"
#include "../Libft/src/libft.h"

void	ft_get_gateway(char *gateway_ip)
{
	FILE			*fp;
	char			buf[256] = {0};
	char			iface[16] = {0};
	unsigned long	dest;
	unsigned long	gw;
	struct in_addr	gw_addr;

	fp = fopen("/proc/net/route", "r");
	if (fp == NULL)
	{
		perror("fopen");
		exit (1);
	}
	fgets(buf, sizeof(buf), fp);
	while (fgets(buf, sizeof(buf), fp))
	{
		if (sscanf(buf, "%15s %lx %lx", iface, &dest, &gw) == 3)
		{
			if (dest == 0)
			{
				gw_addr.s_addr = gw;
				ft_strncpy(gateway_ip, inet_ntoa(gw_addr), 16);
				fclose(fp);
				return ;
			}
		}
	}
	fprintf(stderr, "Could not find a gateway\n");
	fclose(fp);
}

void	ft_get_gateway_mac(char *ip, char *mac)
{
	FILE	*fp;
	char	buf[256] = {0};
	char	ip_addr[32] = {0};
	char	hw_type[16] = {0};
	char	flags[16] = {0};
	char	hw_addr[18] = {0};
	char	mask[32] = {0};
	char	dev[32] = {0};

	fp = fopen("/proc/net/arp", "r");
	if (fp == NULL)
	{
		perror("fopen");
		exit (1);
	}
	fgets(buf, sizeof(buf), fp);
	while (fgets(buf, sizeof(buf), fp))
	{
		if (sscanf(buf, "%31s %15s %15s %17s %31s %31s",
					ip_addr, hw_type, flags, hw_addr, mask, dev) == 6)
		{
			if (ft_strncmp(ip_addr, ip, sizeof(ip)) == 0)
			{
				ft_strncpy(mac, hw_addr, 17);
				fclose(fp);
				return ;
			}
			else
			{
				fprintf(stderr, "Could not find the MAC address for %s\n", ip);
				fclose(fp);
				exit (1);
			}
		}
	}
	fclose(fp);
	fprintf(stderr, "Could not find the MAC address for %s\n", ip);
}
