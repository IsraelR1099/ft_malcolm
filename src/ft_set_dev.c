/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_set_dev.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irifarac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:18:13 by irifarac          #+#    #+#             */
/*   Updated: 2025/02/07 13:18:15 by irifarac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_malcolm.h"
#include "../Libft/src/libft.h"

void	ft_set_device(t_info *info, struct sockaddr_ll *dev)
{
	ft_memset(dev, 0, sizeof(dev));
	dev->sll_family = AF_PACKET;
	dev->sll_ifindex = if_nametoindex(info->dev);
	if (dev->sll_ifindex == 0)
	{
		perror("if_nametoindex");
		exit (1);
	}
	dev->sll_halen = ETH_ALEN;
}
