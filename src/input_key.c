/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_key.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcho <bcho@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 17:13:03 by bcho              #+#    #+#             */
/*   Updated: 2023/05/26 16:27:26 by bcho             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	deal_key(int key_code, t_game *game)
{
	if (key_code == KEY_ESC)
		exit_event(game);
	else if (key_code == KEY_W || key_code == KEY_UP)
		parallel_move(game, 0);
	else if (key_code == KEY_S || key_code == KEY_DOWN)
		parallel_move(game, M_PI);
	else if (key_code == KEY_A)
		parallel_move(game, M_PI_2);
	else if (key_code == KEY_D)
		parallel_move(game, -M_PI_2);
	else if (key_code == KEY_RIGHT)
		rotate_move(game, -R_UNIT * 0.1f);
	else if (key_code == KEY_LEFT)
		rotate_move(game, R_UNIT * 0.1f);
	return (0);
}

int	moveable(t_game *game, double nx, double ny)
{
	int	x;
	int	y;

	x = (int)nx;
	y = (int)ny;
	if (x < 0 || y < 0 || x >= game->map.row || y >= game->map.col)
		return (0);
	if (game->map.saved_map[x][y] > '0')
		return (0);
	else
		return (1);
}

void	parallel_move(t_game *g, double angle)
{
	double	y;
	double	x;

	y = (g->py);
	x = (g->px + (g->dirx * cos(angle) - g->diry * sin(angle)) * M_UNIT);
	if (moveable(g, x, y))
		g->px = x;
	y = (g->py + (g->dirx * sin(angle) + g->diry * cos(angle)) * M_UNIT);
	x = (g->px);
	if (moveable(g, x, y))
		g->py = y;
}

void	rotate_move(t_game *g, double angle)
{
	double	new_dirx;
	double	new_diry;
	double	new_planex;
	double	new_planey;

	new_dirx = g->dirx * cos(angle) - g->diry * sin(angle);
	new_diry = g->dirx * sin(angle) + g->diry * cos(angle);
	new_planex = g->planex * cos(angle) - g->planey * sin(angle);
	new_planey = g->planex * sin(angle) + g->planey * cos(angle);
	g->dirx = new_dirx;
	g->diry = new_diry;
	g->planex = new_planex;
	g->planey = new_planey;
}
