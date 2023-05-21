#include "cub3d.h"

void	exit_with_error(char *message)
{
	write(1, message, ft_strlen(message));
	exit(FAIL);
}

static bool	is_valid_extention(char *file_name, char *extension)
{
	int	file_len;
	int	res;

	file_len = ft_strlen(file_name);
	if (!ft_strncmp(file_name + file_len - 4, extension, 4))
		res = true;
	else
		res = false;
	return (res);
}

int	get_file_fd(char *file_name)
{
	int	fd;

	if (!is_valid_extention(file_name, MAP_EXTENSION))
		exit_with_error("Invalid Extension");
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		exit_with_error("Cannot Open File");
	return (fd);
}

int	get_component_type(char *line)
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

int	get_color_type(char *line)
{
	if (!ft_strncmp(line, FLOOR, 2))
		return F;
	else if (!ft_strncmp(line, CEILING, 2))
		return C;
	else 
		return FALSE;
}

bool	ft_contain(char *str, char c)
{
	int	idx;

	idx = -1;
	while (str[++idx])
	{
		if (str[idx] == c)
			return (true);
	}
	return (false);
}

int	get_map_type(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (!ft_contain("01NSEW ", line[i]))
			return (FALSE);
	}
	return (MAP);
}

int	get_type(char *line)
{
	int	type;
	
	type = get_component_type(line);
	if (type)
		return (type);
	type = get_color_type(line);
	if (type)
		return (type);
	type = get_map_type(line);
	return (type);
}

char	*get_value_point(char *line)
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

bool	set_path_in_map(char *path, char **dest)
{
printf("path: :::%s:::\n", path);
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		exit_with_error("Cannot open xpm file");
	close(fd);
	*dest = path;
	return (true);
}

void	check_color_value(char *line)
{
	int		idx;
	int		jdx;
	char	**split_line;

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

int	get_color_by_value_point(char *value_line, int *idx)
{
	int	rgb;

	rgb = 0;
	while (value_line[++(*idx)] && ft_isdigit(value_line[*idx]))
		rgb = rgb * 10 + (value_line[*idx] - '0');
	if (rgb < 0 || 255 < rgb)
		exit_with_error("Error message");
	return (rgb);
}

int	set_value_point(char *value_line)
{
	int	idx;
	int	r;
	int	g;
	int	b;
	int color_num;

	check_color_value(value_line);
	idx = -1;
	r = get_color_by_value_point(value_line, &idx) * 256 * 256;
	g = get_color_by_value_point(value_line, &idx) * 256;
	b = get_color_by_value_point(value_line, &idx);
	color_num = r + g + b;
	return (color_num);
}

void	set_component_type(t_img *img,  int type, char *line)
{
	char	*value_line;

	value_line = get_value_point(line);
	if (type == NO)
	{
		if (img->no || !set_path_in_map(value_line, (&img->no)))
			exit_with_error("Invalid NO file");
	}
	else if (type == SO)
	{
		if (img->so || !set_path_in_map(value_line, (&img->so)))
			exit_with_error("Invalid SO file");
	}
	else if (type == WE)
	{
		if (img->we || !set_path_in_map(value_line, (&img->we)))
			exit_with_error("Invalid WE file");
	}
	else
	{
		if (img->ea || !set_path_in_map(value_line, (&img->ea)))
			exit_with_error("Invalid EA file");
	}
}

int	get_color_num(char *line)
{
	char	*value_line;
	int		color_num;

	if (*line == '\0')
		exit_with_error("error_message");
	value_line = get_value_point(line);
	color_num = set_value_point(value_line);
	return (color_num);
}

void	set_color_type(t_img *img, int type, char *line)
{
	if (type == F)
	{
		if (img->floor_color != INIT)
			exit_with_error("error_message");
		img->floor_color = get_color_num(line);
	}
	else if (type == C)
	{
		if (img->ceil_color != INIT)
			exit_with_error("error_message");
		img->ceil_color = get_color_num(line);
	}
}

void	check_saved_component(t_img *img)
{
	if (!is_valid_extention(img->no, XPM_EXTENSION))
		exit_with_error("Saved Invalid component no");
	if (!is_valid_extention(img->so, XPM_EXTENSION))
		exit_with_error("Saved Invalid component so");
	if (!is_valid_extention(img->we, XPM_EXTENSION))
		exit_with_error("Saved Invalid component we");
	if (!is_valid_extention(img->ea, XPM_EXTENSION))
		exit_with_error("Saved Invalid component ea");
	if (img->floor_color == INIT || img->ceil_color == INIT || \
		img->floor_color == img->ceil_color)
		exit_with_error("Saved Invalid color value");
}

char	*get_map_value(t_map *map, char *line)
{
	char	*temp;
	char	*result_value;

	if (map->map_value == 0)
		temp = ft_strdup("");
	else 
		temp = ft_strjoin(map->map_value, "\n");
	result_value = ft_strjoin(temp, line);

	free(temp);
	return (result_value);
}

void	set_map_type(t_map *map, t_img *img, char *line)
{
	check_saved_component(img);
	map->map_value = get_map_value(map, line);
}

void	set_date(t_map *map, t_img *img, int type, char *line)
{
	if (NO <= type && type <= EA)
		set_component_type(img, type, line);
	else if (F <= type && type <= C)
		set_color_type(img, type, line);
	else
		set_map_type(map, img, line);
}

void	get_map(t_map *map)
{
	int	col;
	int	row;

	map->saved_map = ft_split(map->map_value, '\n');
	if (map->saved_map == 0)
		exit_with_error("Cannot Saved Map");
	col = -1;
	row = 0;
	while (map->saved_map[++col] != 0)
		if (ft_strlen(map->saved_map[col]) > (size_t)row)
			row = ft_strlen(map->saved_map[col]);
	map->col = col;
	map->row = row;
	free(map->map_value);
	map->map_value = NULL;
}

static void	save_player_position(t_map *map, int col, int row)
{
	if (map->player.starting_initial || map->player.y || map->player.x)
		exit_with_error("duplicated player value");
	map->player.starting_initial = map->saved_map[col][row];
	map->player.y = (double)col;
	map->player.x = (double)row;
	map->saved_map[col][row] = '0';
}

static int	set_map_component(t_map *map, int col, int row)
{
	if (row == 0 || row == (int)ft_strlen(map->saved_map[col]) - 1|| \
	col == 0 || col == map->row - 1)
		return (-1);
		
	if (map->saved_map[col][row + 1] == ' ' || map->saved_map[col][row - 1] == ' ')
		return (-1);
	if ((int)ft_strlen(map->saved_map[col - 1]) <= row \
	|| map->saved_map[col - 1][row] == ' ' || map->saved_map[col - 1][row] == '\0')
		return (-1);
	if ((int)ft_strlen(map->saved_map[col + 1]) <= row \
	|| map->saved_map[col + 1][row] == ' ' || map->saved_map[col + 1][row] == '\0')
		return (-1);
	if (map->saved_map[col][row] != '0')
		save_player_position(map, col, row);
	return (1);
}

void	set_map(t_map *map)
{
	int	col;
	int	row;

	col = -1;
	while (map->saved_map[++col])
	{
		row = -1;
		while (map->saved_map[col][++row])
		{
			if (ft_strchr(WALL, map->saved_map[col][row]))
				continue ;
			else if (ft_strchr(SPACE, map->saved_map[col][row]))
			{
				if (set_map_component(map, col, row) == ERROR)
				{

					exit_with_error("Invalid Map1");

				}
			}
		}
	}
	if (!map->player.starting_initial)
		exit_with_error("Invalid Map2");
}

void	create_map(t_map *map)
{
	get_map(map);
	set_map(map);
}

void	init_game(t_game *game, int fd)
{
	int		type;
	char	*line;
	
	ft_memset(&game->map, 0, sizeof(t_map));
	game->map.img.floor_color = INIT;
	game->map.img.ceil_color = INIT;
	line = get_next_line(fd);
	while (line  != NULL)
	{
		line[ft_strlen(line) - 1] = '\0';
		if (line[0] != '\0')
		{
			type = get_type(line);
			if (!type)
				exit_with_error("Invalid type");
			set_date(&(game->map), &(game->map.img), type, line);
		}
		free(line);
		line = get_next_line(fd);
	}
	create_map(&game->map);
	close(fd);
	free(line);
}

int	main(int	argc, char	**argv)
{
	int		fd;
	t_game	game;

	if (argc != 2)
		exit_with_error("Error Message");
	fd = get_file_fd(argv[1]);
	init_game(&game, fd);

// printf("1: %s\n", game.map.img.no);
// printf("2: %s\n", game.map.img.so);
// printf("3: %s\n", game.map.img.we);
// printf("4: %s\n", game.map.img.ea);
// printf("5 test color: ||%d||\n", game.map.img.floor_color);
// printf("5 test color: ||%d||\n", game.map.img.ceil_color);
// int	idx = -1;
// while (game.map.saved_map[++idx])
// 	printf("CREATE MAP ::%s::\n", game.map.saved_map[idx]);

}

