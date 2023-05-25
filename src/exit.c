/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcho <bcho@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 17:12:28 by bcho              #+#    #+#             */
/*   Updated: 2023/05/25 17:12:30 by bcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	exit_with_error(char *message)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd("ERROR: ", 2);
	ft_putstr_fd(RESET, 2);
	if (message)
		ft_putendl_fd(message, 2);
	exit(FAIL);
}

void	exit_error_with_free(t_game *game, char *message)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd("ERROR: ", 2);
	ft_putstr_fd(RESET, 2);
	if (message)
		ft_putendl_fd(message, 2);
	free_all_data(game, 0);
	exit(FAIL);
}

int	exit_event(t_game *game)
{
	ft_putendl_fd(GREEN, 0);
	ft_putendl_fd("EXIT CUB3D", 0);
	ft_putendl_fd(RESET, 0);
	free_all_data(game, 0);
	exit(FAIL);
}
