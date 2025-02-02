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
