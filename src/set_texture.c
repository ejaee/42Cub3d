#include "../include/cub3d.h"

t_texture getwalltexture(t_game *g)
{
	if (g->side == 0)
	{
		if (g->stepx == -1)
			return (g->tex[3]);
		else
			return (g->tex[4]);
	}
	else
	{
		if (g->stepy == -1)
			return (g->tex[1]);
		else
			return (g->tex[2]);
	}
}

void cal_texture(t_game *g, t_texture wall_tex)
{
	if (g->side == 0)
		g->wallx = g->py + g->perpwalldist * g->raydiry;
	else
		g->wallx = g->px + g->perpwalldist * g->raydirx;

	// printf("1 : wallx py perpwalldist raydiry ::  %f %f %f %f\n", g->py, g->perpwalldist, g->raydiry, g->wallx);
	//  floor 한거랑 mapx 한거 값 차이 찍어봐서 비교하자.
	g->wallx -= floor((g->wallx)); // x좌표 사각형 왼쪽위 니까 내림하는거 필요없이 map으로 갈겨도 상관 무
	// g->wallx -= g->mapx;					// 이거로 해도 무관하다 생각함. 확실하진 않노.
	// printf("2 : wallx is %f\n", g->wallx);

	g->texx = (int)(g->wallx * (double)(wall_tex.width));

	// printf("3 : texx is %d\n", g->texx);

	if (g->side == 0 && g->raydirx > 0)
		g->texx = wall_tex.width - g->texx - 1;
	if (g->side == 1 && g->raydiry < 0)
		g->texx = wall_tex.width - g->texx - 1;

	// printf("4 : texx is %d\n", g->texx);

	g->step = 1.0 * wall_tex.height / g->lineheight; // -> 세로줄 내려갈 때 시작하는지점.

	g->texpos = (g->drawstart - SCREEN_HEIGHT / 2 + g->lineheight / 2);
	g->texpos *= g->step; // 이거 모르겠다.
												// printf("texpos is %f\n", g->texpos);
	//   g->texpos = 0;
}