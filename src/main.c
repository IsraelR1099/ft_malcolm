/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irifarac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 10:14:11 by irifarac          #+#    #+#             */
/*   Updated: 2025/02/10 21:26:04 by israel           ###   ########.fr       */
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
#ifdef BONUS
		ft_set_arp_spoof_bonus(info);
	else
		ft_set_arp_spoof(info);
#endif
	return (0);
}
