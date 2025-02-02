#include "../include/ft_malcolm.h"

static void	ft_handle_sigint(int sig)
{
	(void)sig;
	printf("\n");
	stop = 1;
}

static void	ft_handle_sigterm(int sig)
{
	(void)sig;
	stop = 1;
}

void	ft_set_signals(void)
{
	signal(SIGINT, ft_handle_sigint);
	signal(SIGTERM, ft_handle_sigterm);
}
