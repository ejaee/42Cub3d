#include "../include/cub3d.h"

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

void init_coordinates(t_game *g)
{
	double angle;

	g->px = g->map.player.y + 0.5f;
	g->py = g->map.player.x + 0.5f;
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

	// printf("init coordinates ::  dirx diry planex planey ::  %f %f %f %f \n", g->dirx, g->diry, g->planex, g->planey);

	// g->miniw = SCREEN_WIDTH * MINIMAP_SCALE; //미니맵은 그냥 축소판
	// g->minih = SCREEN_HEIGHT * MINIMAP_SCALE;
	// g->gridw = g->miniw / g->map->col;
	// g->gridh = g->minih / g->map->row;
	// g->mousemode = 0;
}

int init_window(t_game *game)
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

void init_img(t_game *game)
{
	t_texture *tx;

	tx = game->tex;
	tx[1].texture.img = mlx_xpm_file_to_image(game->mlx, \
		tx[1].tex_path_malloc, &(tx[1].width), &(tx[1].height));
	tx[2].texture.img = mlx_xpm_file_to_image(game->mlx, \
		tx[2].tex_path_malloc, &(tx[2].width), &(tx[2].height));
	tx[3].texture.img = mlx_xpm_file_to_image(game->mlx, \
		tx[3].tex_path_malloc, &(tx[3].width), &(tx[3].height));
	tx[4].texture.img = mlx_xpm_file_to_image(game->mlx, \
		tx[4].tex_path_malloc, &(tx[4].width), &(tx[4].height));
	game->screen.img = mlx_new_image(game->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	// game->minimap.img = mlx_new_image(game->mlx, game->miniw, game->minih);
}
