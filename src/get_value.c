#include "../include/cub3d.h"

char	*get_value_of_addr(char *line)
{
	int		idx;
	char	**split_line;
	char	*res;

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

int	get_value_of_color(char *line)
{
	char	*value_line;
	int		color_num;

	if (*line == '\0')
		exit_with_error("ReadLine is empty");
	value_line = get_value_of_addr(line);
	color_num = set_value_of_color(value_line);
	return (color_num);
}

char	*get_value_of_map(t_map *map, char *line)
{
	char	*temp;
	char	*result_value;

	if (map->map_value == 0)
		temp = ft_strdup("");
	else
		temp = ft_strjoin(map->map_value, "\n");
	free(map->map_value);
	result_value = ft_strjoin(temp, line);
	free(temp);
	return (result_value);
}

bool	get_value_of_path(char *path, t_game *game, int idx)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		exit_with_error("Cannot open xpm file");
	close(fd);
	game->tex[idx].tex_path_malloc = ft_strdup(path);
	return (true);
}
