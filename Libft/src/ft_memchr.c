/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irifarac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 14:01:19 by irifarac          #+#    #+#             */
/*   Updated: 2025/01/27 12:48:01 by irifarac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int ch, size_t count)
{
	const char		*new_str;
	unsigned int	position;

	new_str = (const char *)str;
	position = 0;
	if (count == 0)
		return (0);
	while (position < count)
	{
		if (new_str[position] == (char)ch)
			return ((void *)((const char *)str + position));
		position++;
	}
	return (0);
}
