#include "../include/cub3d.h"

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


void set_type(t_game *game, int type, char *line)
{
	if (NO <= type && type <= EA)
		set_type_of_component(game, type, line);
	else if (F <= type && type <= C)
		set_type_of_color(&game->img, type, line);
	else
		set_type_of_map(&game->map, game, line);
}
