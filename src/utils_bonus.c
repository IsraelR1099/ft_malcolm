/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irifarac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:10:17 by irifarac          #+#    #+#             */
/*   Updated: 2025/02/14 17:51:09 by israel           ###   ########.fr       */
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
