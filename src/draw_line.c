#include "../include/cub3d.h"

void dda(t_game *g)
{
	// printf(" \n\n 1   ------------  inside dda \n\n");
	while (g->hit == 0)
	{
		//	printf(" dda -  inside loop  hit   :: %d\n", g->hit);

		//	printf(" dda - sidedistx sidedisty ::");
		// printf(" %f ", g->sidedistx);
		// printf(" %f ", g->sidedisty);
		if (g->sidedistx < g->sidedisty)
		{
			g->sidedistx += g->deltadistx;
			g->mapx += g->stepx;
			g->side = 0;
		}
		else
		{
			g->sidedisty += g->deltadisty;
			g->mapy += g->stepy;
			g->side = 1;
		}
		if (g->map.saved_map[g->mapx][g->mapy] > '0')
			g->hit = 1;
		// 존나 신기하게도 맵을 읽는데, 스폰 장소가 이동이 됨. 원래 막혀야 하는거 가
	}
	//	printf("  2   ------------  inside dda \n\n");
	if (g->side == 0)
	{
		// g->perpwalldist = g->sidedistx - g->deltadistx;
		g->perpwalldist = (g->mapx - g->px + (1 - g->stepx) / 2) / g->raydirx;
	}
	else
	{
		// g->perpwalldist = g->sidedisty - g->deltadisty;
		g->perpwalldist = (g->mapy - g->py + (1 - g->stepy) / 2) / g->raydiry;
	}
	//	printf("  3   ------------  inside dda \n\n");
	// 이거 내가 공식 바꾼건데 제대로 굴러가는거 같긴 함. 기왕이면 설명 가능한 수식으로 ...
}

void getdrawpoint(t_game *g)
{
	g->lineheight = (int)(SCREEN_HEIGHT / g->perpwalldist); // 화면에 표시될 벽 높이
	g->drawstart = (-1 * g->lineheight / 2) + (SCREEN_HEIGHT / 2);
	if (g->drawstart < 0)
		g->drawstart = 0;
	g->drawend = g->lineheight / 2 + SCREEN_HEIGHT / 2;
	if (g->drawend >= SCREEN_HEIGHT)
		g->drawend = SCREEN_HEIGHT - 1;
}



void drawline(t_game *g, t_texture wall_tex, int x)
{
	int color;
	int y;

	y = g->drawstart - 1;
	while (++y < g->drawend)
	{
		// g->texy = (int)(g->texpos); -> 이거만 있어도 된다.
		g->texy = (int)(g->texpos) & (wall_tex.height - 1);
		// overflow 방지. 텍스트의 높이(y) 가 벗어나지 않도록 마스킹해주는것
		// 0~63 값은 & 연산해도 값ㅇ ㅣ안바뀌어 버림. 넘어가면 오버플로우가 난 것.
		// 그런 넘어간 값이 0~63 범위 안으로 내려옴.
		g->texpos += g->step;

		color = g->wall_data[wall_tex.height * g->texy + g->texx];
		if (g->side == 1)
			color = (color >> 1) & 8355711;
		// 이 새끼가 남,북 쪽 색깔 더두운 여부 만들어줌.
		// RGB 1111111 1111111 1111111
		g->screen_data[y * SCREEN_WIDTH + x] = color;
	}
}

