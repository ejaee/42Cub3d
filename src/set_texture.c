#include "../include/cub3d.h"

t_texture	get_texture(t_game *g)
{
	if (g->side == 0)
		return (g->tex[g->stepx == -1 ? WE : EA]);
	else
		return (g->tex[g->stepy == -1 ? NO : SO]);
}

void	cal_texture(t_game *g, t_texture wall_tex)
{
	if (g->side == 0)
		g->wallx = g->py + g->perpwalldist * g->raydiry;
	else
		g->wallx = g->px + g->perpwalldist * g->raydirx;
	g->wallx -= floor((g->wallx)); // x좌표 사각형 왼쪽위 니까 내림하는거 필요없이 map으로 갈겨도 상관 무
	g->texx = (int)(g->wallx * (double)(wall_tex.width));
	if (g->side == 0 && g->raydirx > 0)
		g->texx = wall_tex.width - g->texx - 1;
	if (g->side == 1 && g->raydiry < 0)
		g->texx = wall_tex.width - g->texx - 1;
	g->step = 1.0 * wall_tex.height / g->lineheight; // -> 세로줄 내려갈 때 시작하는지점.
	g->texpos = (g->drawstart - SCREEN_HEIGHT / 2 + g->lineheight / 2);
	g->texpos *= g->step; // 이거 모르겠다.
}
