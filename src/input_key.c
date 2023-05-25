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
		rotate_move(game, -R_UNIT);
	else if (key_code == KEY_LEFT)
		rotate_move(game, R_UNIT);
	return (0);
}

int	moveable(t_game *game, double nx, double ny)
{
	int	x;
	int	y;

	x = (int)nx;
	y = (int)ny;
	if (x < 0 || y < 0 || x >= game->map.row || y >= game->map.col)
		return (1);
	if (game->map.saved_map[x][y] > '0')
		return (0);
	else
		return (1);
}

void	parallel_move(t_game *g, double angle)
{
	double	nx;
	double	ny;

	nx = g->px + (g->dirx * cos(angle) - g->diry * sin(angle)) * M_UNIT;
	ny = g->py + (g->dirx * sin(angle) + g->diry * cos(angle)) * M_UNIT;
	if (!moveable(g, nx, ny))
		return ;
	g->px = nx;
	g->py = ny;
}

void	rotate_move(t_game *g, double angle)
{
	double	cos_angle;
	double	sin_angle;
	double	new_dirx;
	double	new_diry;
	double	new_planex;
	double	new_planey;

	cos_angle = cos(angle);
	sin_angle = sin(angle);
	new_dirx = g->dirx * cos_angle - g->diry * sin_angle;
	new_diry = g->dirx * sin_angle + g->diry * cos_angle;
	new_planex = g->planex * cos_angle - g->planey * sin_angle;
	new_planey = g->planex * sin_angle + g->planey * cos_angle;
	g->dirx = new_dirx;
	g->diry = new_diry;
	g->planex = new_planex;
	g->planey = new_planey;
}
