#include "../include/ft_malcolm.h"

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
