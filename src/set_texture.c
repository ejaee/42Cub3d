#include "../include/cub3d.h"

t_texture	get_texture(t_game *g)
{
	t_texture	texture;

	if (g->side == 0)
	{
		if (g->stepx == -1)
			texture = g->tex[WE];
		else
			texture = g->tex[EA];
	}
	else
	{
		if (g->stepy == -1)
			texture = g->tex[NO];
		else
			texture = g->tex[SO];
	}
	return (texture);
}

void	cal_texture(t_game *g, t_texture wall_tex)
{
	if (g->side == 0)
		g->wallx = g->py + g->perpwalldist * g->raydiry;
	else
		g->wallx = g->px + g->perpwalldist * g->raydirx;
	g->wallx -= floor((g->wallx));
	g->texx = (int)(g->wallx * (double)(wall_tex.width));
	if (g->side == 0 && g->raydirx > 0)
		g->texx = wall_tex.width - g->texx - 1;
	if (g->side == 1 && g->raydiry < 0)
		g->texx = wall_tex.width - g->texx - 1;
	g->step = 1.0 * wall_tex.height / g->lineheight;
	g->texpos = (g->drawstart - SCREEN_HEIGHT / 2 + g->lineheight / 2);
	g->texpos *= g->step;
}
