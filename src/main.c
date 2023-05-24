#include "cub3d.h"

void free_all_data(t_game *game)
{
	int idx;

	idx = 0;
	while (++idx <= 4)
	{
		if (game->tex[idx].tex_path_malloc)
		{
			free(game->tex[idx].tex_path_malloc);
			game->tex[idx].tex_path_malloc = NULL;
			free(game->tex[idx].data);
			game->tex[idx].data = NULL;
		}
	}
	if (game->map.map_value)
		free(game->map.map_value);
	game->map.map_value = NULL;
	if (game->map.saved_map)
	{
		idx = -1;
		while (game->map.saved_map[++idx])
		{
			free(game->map.saved_map[idx]);
			game->map.saved_map[idx] = NULL;
		}
		free(game->map.saved_map);
	}
}

/*----------------------------[ error.c ]----------------------------*/



int get_file_fd(char *file_name)
{
	int fd;

	if (!is_valid_extention(file_name, MAP_EXTENSION))
		exit_with_error("Invalid Extension");
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		exit_with_error("Cannot Open File");
	return (fd);
}

int moveable(t_game *game, double nx, double ny)
{
	int x;
	int y;

	x = (int)nx;
	y = (int)ny;
	if (x < 0 || y < 0 || x >= game->map.row || y >= game->map.col)
		return (1);
	if (game->map.saved_map[x][y] > '0')
		return (0);
	else
		return (1);
}

void move(t_game *g, double angle)
{
	double nx;
	double ny;

	nx = g->px + (g->dirx * cos(angle) - g->diry * sin(angle)) * M_UNIT;
	ny = g->py + (g->dirx * sin(angle) + g->diry * cos(angle)) * M_UNIT; // 다음 이동 좌표가 나온다.

	if (!moveable(g, nx, ny) || !moveable(g, -nx + g->planex * 0.25, ny + g->planey * 0.25) || !moveable(g, nx + g->planex * 0.25, ny + g->planey * 0.25) || !moveable(g, -nx + g->planex * 0.25, ny - g->planey * 0.25) || !moveable(g, nx + g->planex * 0.25, ny - g->planey * 0.25)) //??? 이거 모르겠네.
		return;
	g->px = nx;
	g->py = ny;
}

void rotate(t_game *g, double angle)
{
	double tempx;
	double tempy;

	tempx = g->dirx;
	tempy = g->diry;
	g->dirx = tempx * cos(angle) - tempy * sin(angle);
	g->diry = tempx * sin(angle) + tempy * cos(angle);
	tempx = g->planex;
	tempy = g->planey;
	g->planex = tempx * cos(angle) - tempy * sin(angle);
	g->planey = tempx * sin(angle) + tempy * cos(angle);
}

/////////////////////////////////////////////////////////////

/// deal keyboard input //////////////////////

int deal_key(int key_code, t_game *game)
{
	// printf("any input inside !!\n");

	if (key_code == KEY_ESC)
		exit_event(game);
	else if (key_code == KEY_UP || key_code == KEY_W)
		move(game, 0);
	else if (key_code == KEY_DOWN || key_code == KEY_S)
		move(game, M_PI);
	else if (key_code == KEY_A)
		move(game, M_PI_2);
	else if (key_code == KEY_D)
		move(game, -M_PI_2);
	else if (key_code == KEY_RIGHT)
		rotate(game, -R_UNIT);
	else if (key_code == KEY_LEFT)
		rotate(game, R_UNIT);

	return (0);
}

///////////////////////////////

void setscreen(t_game *g)
{
	int y;
	int x;

	y = -1;
	while (++y < SCREEN_HEIGHT)
	{
		x = -1;
		while (++x < SCREEN_WIDTH)
		{
			if (y < SCREEN_HEIGHT / 2)
				g->screen_data[y * SCREEN_WIDTH + x] = g->img.ceil_color;
			else if (y > SCREEN_HEIGHT / 2)
				g->screen_data[y * SCREEN_WIDTH + x] = g->img.floor_color;
			else
				g->screen_data[y * SCREEN_WIDTH + x] = 0;
		}
	}
}

void ray_cal_init(t_game *g, int x)
{
	g->camerax = 2 * x / (double)(SCREEN_WIDTH - 1) - 1;
	g->raydirx = g->dirx + g->planex * g->camerax;
	g->raydiry = g->diry + g->planey * g->camerax;
	g->mapx = (int)(g->px);
	g->mapy = (int)(g->py);
	g->deltadistx = fabs(1 / g->raydirx);
	g->deltadisty = fabs(1 / g->raydiry); // 0 으로 나누는거 예외처리 하라던데
	g->hit = 0;
	
	//printf("%d | %f | %f | %f | %d | %d | %f | %f | %d | \n",x, g->camerax, g->raydirx, g->raydiry, g->mapx, g->mapy,
	// 	g->deltadistx, g->deltadisty, g->hit  );
}

void getsidedist(t_game *g)
{
	if (g->raydirx < 0)
	{
		g->stepx = -1;
		g->sidedistx = (g->px - g->mapx) * g->deltadistx;
	}
	else
	{
		g->stepx = 1;
		g->sidedistx = (g->mapx + 1.0 - g->px) * g->deltadistx;
	}
	if (g->raydiry < 0)
	{
		g->stepy = -1;
		g->sidedisty = (g->py - g->mapy) * g->deltadisty;
	}
	else
	{
		g->stepy = 1;
		g->sidedisty = (g->mapy + 1.0 - g->py) * g->deltadisty;
	}
}

void dda(t_game *g)
{
	//printf(" \n\n 1   ------------  inside dda \n\n");
	while (g->hit == 0)
	{
	//	printf(" dda -  inside loop  hit   :: %d\n", g->hit);
		
	//	printf(" dda - sidedistx sidedisty ::");
		//printf(" %f ", g->sidedistx);
		//printf(" %f ", g->sidedisty);
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

	//printf("1 : wallx py perpwalldist raydiry ::  %f %f %f %f\n", g->py, g->perpwalldist, g->raydiry, g->wallx);
	// floor 한거랑 mapx 한거 값 차이 찍어봐서 비교하자.
	g->wallx -= floor((g->wallx)); // x좌표 사각형 왼쪽위 니까 내림하는거 필요없이 map으로 갈겨도 상관 무
	// g->wallx -= g->mapx;					// 이거로 해도 무관하다 생각함. 확실하진 않노.
	//printf("2 : wallx is %f\n", g->wallx);

	g->texx = (int)(g->wallx * (double)(wall_tex.width));

	//printf("3 : texx is %d\n", g->texx);

	if (g->side == 0 && g->raydirx > 0)
		g->texx = wall_tex.width - g->texx - 1;
	if (g->side == 1 && g->raydiry < 0)
		g->texx = wall_tex.width - g->texx - 1;

	//printf("4 : texx is %d\n", g->texx);

	g->step = 1.0 * wall_tex.height / g->lineheight; // -> 세로줄 내려갈 때 시작하는지점.

	g->texpos = (g->drawstart - SCREEN_HEIGHT / 2 + g->lineheight / 2);
	g->texpos *= g->step; // 이거 모르겠다.
	//printf("texpos is %f\n", g->texpos);
	//  g->texpos = 0;
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

void cast_one_ray(t_game *g, int x)
{
	t_texture wall_tex;
//	printf(":::  1  : -------------x is %d -----------\n", x);
	ray_cal_init(g, x);
//	printf(":::  2  : ------------------------\n");
	getsidedist(g);

//	printf(":::  3  : ------------------------\n");
	dda(g);

//	printf(":::  4  : ------------------------\n");
	getdrawpoint(g);

//	printf(":::  5  : ------------------------\n");

	wall_tex = getwalltexture(g);

//	printf("\n\nvalue pf wall_tex is  %s\n", wall_tex.tex_path_malloc);

	g->wall_data = (t_ui *)mlx_get_data_addr(wall_tex.texture.img,
																					 &(wall_tex.texture.bpp), &(wall_tex.texture.size_l),
																					 &(wall_tex.texture.endian)); // 이런거를 잘 모르겠다 스바
	//printf("wall data is %u\n", *g->wall_data);
	cal_texture(g, wall_tex);
	drawline(g, wall_tex, x);
}

int main_loop(t_game *g)
{
	//printf("1   :  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

	mlx_clear_window(g->mlx, g->win);
	g->screen_data = (t_ui *)mlx_get_data_addr(g->screen.img, &(g->screen.bpp),
																						 &(g->screen.size_l), &(g->screen.endian));
	
	//printf("2   :  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	setscreen(g);
	//printf("3   :  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	// g->minimap_data = (t_ui *)mlx_get_data_addr(g->minimap.img,
	//		&(g->minimap.bpp), &(g->minimap.size_l), &(g->minimap.endian));

	int x;
	x = -1;
	//printf("4   :  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	while (++x < SCREEN_WIDTH)
	{
		//printf("in side cast one ray loop\n");
		cast_one_ray(g, x);
	}
	//printf("5   :  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	mlx_put_image_to_window(g->mlx, g->win, g->screen.img, 0, 0);
	// paint_minimap(g);
	return (0);
}


void	check() {
	system("leaks --list -- cub3D");
}

int main(int argc, char **argv)
{
	atexit(check);
	int fd;
	t_game game;

	if (argc != 2)
		exit_with_error("Error Message");
	fd = get_file_fd(argv[1]);
	init_game(&game, fd);

	init_coordinates(&game); 		//game_init
	init_window(&game);
	init_img(&game);
	printf(" any problem ??? \n\n");

	mlx_hook(game.win, X_EVENT_KEY_PRESS, 0, &deal_key, &game);
	mlx_hook(game.win, X_EVENT_KEY_EXIT, 0, &exit_event, &game);
	printf(" ssibal ssibal \n\n");
	mlx_loop_hook(game.mlx, &main_loop, &game);

	printf(" =============== check texture part ===============\n");
	printf(" height of textures %d\n ", game.tex[1].height);
	printf(" height of textures %d\n ", game.tex[2].height);
	printf(" height of textures %d\n ", game.tex[3].height);
	printf(" height of textures %d\n ", game.tex[4].height);

	printf("\033[0;32m[ Check saved Component value ]\n\033[0m");
	printf(" value of textures %s\n ", game.tex[1].tex_path_malloc);
	printf(" value of textures %s\n ", game.tex[2].tex_path_malloc);
	printf(" value of textures %s\n ", game.tex[3].tex_path_malloc);
	printf(" value of textures %s\n\n\n ", game.tex[4].tex_path_malloc);
	printf("floor value: ||\033[0;31m%d\033[0m||\n", game.img.floor_color);
	printf("ceil value: ||\033[0;31m%d\033[0m||\n", game.img.ceil_color);

	printf("\n\033[0;32m[ Check saved Map ]\n\033[0m");
	int idx = -1;
	while (game.map.saved_map[++idx])
		printf("\033[0;31m%s\033[0m\n", game.map.saved_map[idx]);

	mlx_loop(game.mlx);

	// printf(" value of textures %s\n ", game.map.tex[0].tex_path_malloc);

	free_all_data(&game);//fixed
}

/*-------------------------------------------------------------------*/
