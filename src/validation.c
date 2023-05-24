#include "../include/cub3d.h"

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
