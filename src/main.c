/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irifarac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 10:14:11 by irifarac          #+#    #+#             */
/*   Updated: 2025/02/01 22:55:02 by israel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_malcolm.h"
#include "../Libft/src/libft.h"

volatile sig_atomic_t	stop = 0;

int	main(int argc, char **argv)
{
	t_info	info;

	ft_memset(&info, 0, sizeof(t_info));
	ft_check_errors(argc);
	ft_init(&info, argv);
	ft_check_syntax(&info);
	ft_set_signals();
	ft_set_arp_spoof(info);
	printf("ip_src: %s\n", info.ip_src);
	printf("mac_src: %s\n", info.mac_src);
	printf("ip_target: %s\n", info.ip_target);
	printf("mac_target: %s\n", info.mac_target);
	return (0);
}
