#include "../include/cub3d.h"

void exit_with_error(char *message)
{
	ft_putendl_fd(RED, 2);
	ft_putendl_fd("ERROR", 2);
	ft_putendl_fd(RESET, 2);
	if (message)
		ft_putendl_fd(message, 2);
	exit(FAIL);
}

void exit_error_with_free(t_game *game, char *message)
{
	ft_putendl_fd(RED, 2);
	ft_putendl_fd("ERROR", 2);
	ft_putendl_fd(RESET, 2);
	if (message)
		ft_putendl_fd(message, 2);
	free_all_data(game);
	exit(FAIL);
}

int exit_event(t_game *game)
{
	ft_putendl_fd(GREEN, 0);
	ft_putendl_fd("EXIT CUB3D", 0);
	ft_putendl_fd(RESET, 0);
	free_all_data(game);
	exit(FAIL);
}