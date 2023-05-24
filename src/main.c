#include "cub3d.h"

/*----------------------------[ error.c ]----------------------------*/

void exit_with_error(char *message)
{
	write(1, message, ft_strlen(message));
	exit(FAIL);
}

/*-------------------------------------------------------------------*/
/*----------------------------[ validation.c ]----------------------------*/

bool is_valid_extention(char *file_name, char *extension)
{
	int file_len;
	int res;

	file_len = ft_strlen(file_name);
	if (!ft_strncmp(file_name + file_len - 4, extension, 4))
		res = true;
	else
		res = false;
	return (res);
}

bool is_contain(char *str, char c)
{
	int idx;

	idx = -1;
	while (str[++idx])
	{
		if (str[idx] == c)
			return (true);
	}
	return (false);
}

void check_saved_component(t_game *game)
{
	if (!game->tex[1].tex_path_malloc || !is_valid_extention(game->tex[1].tex_path_malloc, XPM_EXTENSION))
		exit_with_error("Saved Invalid component no");
	if (!game->tex[2].tex_path_malloc || !is_valid_extention(game->tex[2].tex_path_malloc, XPM_EXTENSION))
		exit_with_error("Saved Invalid component so");
	if (!game->tex[3].tex_path_malloc || !is_valid_extention(game->tex[3].tex_path_malloc, XPM_EXTENSION))
		exit_with_error("Saved Invalid component we");
	if (!game->tex[4].tex_path_malloc || !is_valid_extention(game->tex[4].tex_path_malloc, XPM_EXTENSION))
		exit_with_error("Saved Invalid component ea");

	if (game->img.floor_color == INIT || game->img.ceil_color == INIT ||
			game->img.floor_color == game->img.ceil_color)
		exit_with_error("Saved Invalid color value");

}

void check_color_value(char *line)
{
	int idx;
	int jdx;
	char **split_line;

	split_line = ft_split(line, ',');
	if (!split_line)
		exit_with_error("Invalid RGB Format");
	idx = -1;
	while (split_line[++idx])
	{
		jdx = -1;
		while (split_line[idx][++jdx])
			if (!ft_isdigit(split_line[idx][jdx]))
				exit_with_error("Invalid RGB Format");
	}
	if (idx != 3)
		exit_with_error("Invalid RGB Format");
	idx = -1;
	while (split_line[++idx])
		free(split_line[idx]);
	free(split_line);
}

/*-------------------------------------------------------------------*/
/*----------------------------[ generate_map.c  ]----------------------------*/

static void set_player_position(t_map *map, int col, int row)
{
	if (map->player.starting_initial || map->player.y || map->player.x)
		exit_with_error("duplicated player value");
	map->player.starting_initial = map->saved_map[col][row];
	map->player.y = (double)col;
	map->player.x = (double)row;
	map->saved_map[col][row] = '0';
}

static int set_map_component(t_map *map, int col, int row)
{
	if (row == 0 || row == (int)ft_strlen(map->saved_map[col]) - 1 ||
			col == 0 || col == map->row - 1)
		return (-1);
	if (map->saved_map[col][row + 1] == ' ' || map->saved_map[col][row - 1] == ' ')
		return (-1);
	if ((int)ft_strlen(map->saved_map[col - 1]) <= row || map->saved_map[col - 1][row] == ' ' || map->saved_map[col - 1][row] == '\0')
		return (-1);
	if ((int)ft_strlen(map->saved_map[col + 1]) <= row || map->saved_map[col + 1][row] == ' ' || map->saved_map[col + 1][row] == '\0')
		return (-1);
	if (map->saved_map[col][row] != '0')
		set_player_position(map, col, row);
	return (1);
}

void set_map(t_map *map)
{
	int col;
	int row;

	col = -1;
	while (map->saved_map[++col])
	{
		row = -1;
		while (map->saved_map[col][++row])
		{
			if (ft_strchr(UNMOVABLE, map->saved_map[col][row]))
				continue;
			else if (ft_strchr(MOVABLE, map->saved_map[col][row]))
			{
				if (set_map_component(map, col, row) == ERROR)
					exit_with_error("A component of the map was saved incorrectly");
			}
		}
	}
	if (!map->player.starting_initial)
		exit_with_error("Failed to save the player's location value");
}

void get_map(t_map *map)
{
	int col;
	int row;

	map->saved_map = ft_split(map->map_value, '\n');
	if (map->saved_map == 0)
		exit_with_error("Cannot Saved Map");
	col = -1;
	row = 0;
	while (map->saved_map[++col] != 0)
		if (ft_strlen(map->saved_map[col]) > (size_t)row)
			row = ft_strlen(map->saved_map[col]);
	map->col = row;
	map->row = col;
	free(map->map_value);
	map->map_value = NULL;
}

void generate_map(t_map *map)
{
	get_map(map);
	set_map(map);
}

/*-------------------------------------------------------------------*/
/*----------------------------[ set_value.c  ]----------------------------*/

int set_value_of_element(char *value_line, int *idx)
{
	int rgb;

	rgb = 0;
	while (value_line[++(*idx)] && ft_isdigit(value_line[*idx]))
		rgb = rgb * 10 + (value_line[*idx] - '0');
	if (rgb < 0 || 255 < rgb)
		exit_with_error("Error message");
	return (rgb);
}

int set_value_of_color(char *value_line)
{
	int idx;
	int r;
	int g;
	int b;
	int color_num;

	check_color_value(value_line);
	idx = -1;
	r = set_value_of_element(value_line, &idx) * 256 * 256;
	g = set_value_of_element(value_line, &idx) * 256;
	b = set_value_of_element(value_line, &idx);
	color_num = r + g + b;
	free(value_line);
	return (color_num);
}

/*-------------------------------------------------------------------*/
/*----------------------------[ get_value.c  ]----------------------------*/

char *get_value_of_addr(char *line)
{
	int idx;
	char **split_line;
	char *res;

	split_line = ft_split(line, ' ');
	if (!split_line[1] || split_line[2])
		exit_with_error("Invalid file contents");
	res = ft_strdup(split_line[1]);
	idx = -1;
	while (split_line[++idx])
		free(split_line[idx]);
	free(split_line);
	return (res);
}


int get_value_of_color(char *line)
{
	char *value_line;
	int color_num;

	if (*line == '\0')
		exit_with_error("error_message");
	value_line = get_value_of_addr(line);
	color_num = set_value_of_color(value_line);
	return (color_num);
}

char *get_value_of_map(t_map *map, char *line)
{
	char *temp;
	char *result_value;

	if (map->map_value == 0)
		temp = ft_strdup("");
	else
		temp = ft_strjoin(map->map_value, "\n");
	free(map->map_value);
	result_value = ft_strjoin(temp, line);
	free(temp);
	return (result_value);
}

/*-------------------------------------------------------------------*/
/*----------------------------[ set_type.c  ]----------------------------*/

void set_type_of_color(t_img *img, int type, char *line)
{
	if (type == F)
	{
		if (img->floor_color != INIT)
			exit_with_error("already saved the value of F");
		img->floor_color = get_value_of_color(line);
	}
	else if (type == C)
	{
		if (img->ceil_color != INIT)
			exit_with_error("already saved the value of C");
		img->ceil_color = get_value_of_color(line);
	}
}

void set_type_of_map(t_map *map, t_game *game, char *line)
{

	check_saved_component(game);
	map->map_value = get_value_of_map(map, line);
}

bool get_value_of_path(char *path, t_game *game, int idx)
{
	int fd;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		exit_with_error("Cannot open xpm file");
	close(fd);
	game->tex[idx].tex_path_malloc = ft_strdup(path);
	return (true);
}

void set_type_of_component(t_game *game, int type, char *line)
{
	char *value_line;

	value_line = get_value_of_addr(line);
	if (type == NO)
	{
		if (game->tex[NO].tex_path_malloc || !get_value_of_path(value_line, game, NO))
			exit_with_error("Invalid NO file");
	}
	else if (type == SO)
	{
		if (game->tex[SO].tex_path_malloc || !get_value_of_path(value_line, game, SO))
			exit_with_error("Invalid SO file");
	}
	else if (type == WE)
	{
		if (game->tex[WE].tex_path_malloc || !get_value_of_path(value_line, game, WE))
			exit_with_error("Invalid WE file");
	}
	else
	{
		if (game->tex[EA].tex_path_malloc || !get_value_of_path(value_line, game, EA))
			exit_with_error("Invalid EA file");
	}
	free(value_line);
}

///    ////// free, error handling /////////

void free_all_data(t_game *game)
{
	int i;

	i = 0;
	while (++i <= 4)
	{
		if (game->tex[i].tex_path_malloc)
		{
			free(game->tex[i].tex_path_malloc);
			game->tex[i].tex_path_malloc = NULL;
			free(game->tex[i].data);
			game->tex[i].data = NULL;
		}
	}
	// if (game->map.map_value)
	// 	free(game->map.map_value);
	game->map.map_value = NULL;
	if (game->map.saved_map)
	{
		i = -1;
		while (game->map.saved_map[++i])
		{
			free(game->map.saved_map[i]);
			game->map.saved_map[i] = NULL;
		}
		free(game->map.saved_map);
	}
}

int exit_event(t_game *game)
{
	ft_putendl_fd("EXIT CUB3D", 0);
	free_all_data(game);
	exit(1);
}

void exit_error(t_game *game, char *message)
{
	ft_putendl_fd("ERROR", 2);
	if (message)
		ft_putendl_fd(message, 2);
	free_all_data(game);
	exit(1);
}


///////// SAVE TEXTURE - miffy END ///////////////////
void set_type(t_game *game, int type, char *line)
{
	if (NO <= type && type <= EA)
		set_type_of_component(game, type, line);
	else if (F <= type && type <= C)
		set_type_of_color(&game->img, type, line);
	else
		set_type_of_map(&game->map, game, line);
}

/*-------------------------------------------------------------------*/
/*----------------------------[ get_type.c ]----------------------------*/

int get_type_of_component(char *line)
{
	if (!ft_strncmp(line, NORTH, 3))
		return NO;
	else if (!ft_strncmp(line, SOUTH, 3))
		return SO;
	else if (!ft_strncmp(line, WEST, 3))
		return WE;
	else if (!ft_strncmp(line, EAST, 3))
		return EA;
	else
		return FALSE;
}

int get_type_of_color(char *line)
{
	if (!ft_strncmp(line, FLOOR, 2))
		return F;
	else if (!ft_strncmp(line, CEILING, 2))
		return C;
	else
		return FALSE;
}

int get_type_of_map(char *line)
{
	int i;

	i = -1;
	while (line[++i])
	{
		if (!is_contain(MAP_COMPONENT, line[i]))
			return (FALSE);
	}
	return (MAP);
}

int get_type(char *line)
{
	int type;

	type = get_type_of_component(line);
	if (type)
		return (type);
	type = get_type_of_color(line);
	if (type)
		return (type);
	type = get_type_of_map(line);
	return (type);
}

/*-------------------------------------------------------------------*/
/*----------------------------[ main.c ]----------------------------*/

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

void init_game(t_game *game, int fd)
{
	int type;
	char *line;

	ft_memset(game, 0, sizeof(t_game));
	game->img.floor_color = INIT;
	game->img.ceil_color = INIT;
	line = get_next_line(fd);
	while (line != NULL)
	{
		line[ft_strlen(line) - 1] = '\0';
		if (line[0] != '\0')
		{
			type = get_type(line);
			if (!type)
				exit_with_error("Invalid type");
			set_type(game, type, line);
		}
		free(line);
		line = get_next_line(fd);
	}
	generate_map(&game->map);
	close(fd);
	free(line);
}

int window_init(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (1);
	game->win = mlx_new_window(game->mlx, (int)SCREEN_WIDTH,
														 (int)SCREEN_HEIGHT, "CUB_3D");
	if (!game->win)
		return (1);
	return (0);
}

void img_init(t_game *game)
{
	t_texture *tx;

	tx = game->tex;


	tx[1].texture.img = mlx_xpm_file_to_image(game->mlx,
																						tx[1].tex_path_malloc, &(tx[1].width), &(tx[1].height));
	tx[2].texture.img = mlx_xpm_file_to_image(game->mlx,
																						tx[2].tex_path_malloc, &(tx[2].width), &(tx[2].height));
	tx[3].texture.img = mlx_xpm_file_to_image(game->mlx,
																						tx[3].tex_path_malloc, &(tx[3].width), &(tx[3].height));
	tx[4].texture.img = mlx_xpm_file_to_image(game->mlx,
																						tx[4].tex_path_malloc, &(tx[4].width), &(tx[4].height));
	game->screen.img = mlx_new_image(game->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	// game->minimap.img = mlx_new_image(game->mlx, game->miniw, game->minih);
}

//////////////////////////MOVE  & ROTATE///////////////////////////

static int moveable(t_game *game, double nx, double ny)
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

void init_coordinates(t_game *g)
{
	double angle;

	g->px = g->map.player.y;
	g->py = g->map.player.x;
	if (g->map.player.starting_initial == 'N')
		angle = M_PI;
	else if (g->map.player.starting_initial == 'E')
		angle = M_PI_2;
	else if (g->map.player.starting_initial == 'S')
		angle = 0;
	else
		angle = -M_PI_2;
	g->dirx = cos(angle);
	g->diry = sin(angle);
	g->planex = 0.66 * cos(angle - M_PI_2);
	g->planey = 0.66 * sin(angle - M_PI_2);

	//printf("init coordinates ::  dirx diry planex planey ::  %f %f %f %f \n", g->dirx, g->diry, g->planex, g->planey);

	
	// g->miniw = SCREEN_WIDTH * MINIMAP_SCALE; //미니맵은 그냥 축소판
	// g->minih = SCREEN_HEIGHT * MINIMAP_SCALE;
	// g->gridw = g->miniw / g->map->col;
	// g->gridh = g->minih / g->map->row;
	// g->mousemode = 0;
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
	window_init(&game);
	img_init(&game);
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
