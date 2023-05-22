#include "cub3d.h"

/*----------------------------[ error.c ]----------------------------*/

void	exit_with_error(char *message)
{
	write(1, message, ft_strlen(message));
	exit(FAIL);
}

/*-------------------------------------------------------------------*/
/*----------------------------[ validation.c ]----------------------------*/

bool	is_valid_extention(char *file_name, char *extension)
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

bool	is_contain(char *str, char c)
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

/*-------------------------------------------------------------------*/
/*----------------------------[ generate_map.c  ]----------------------------*/

static void	set_player_position(t_map *map, int col, int row)
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
		set_player_position(map, col, row);
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
			if (ft_strchr(UNMOVABLE, map->saved_map[col][row]))
				continue ;
			else if (ft_strchr(MOVABLE, map->saved_map[col][row]))
			{
				if (set_map_component(map, col, row) == ERROR)
					exit_with_error("Invalid Map1");
			}
		}
	}
	if (!map->player.starting_initial)
		exit_with_error("Invalid Map2");
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

void	generate_map(t_map *map)
{
	get_map(map);
	set_map(map);
}

/*-------------------------------------------------------------------*/
/*----------------------------[ set_value.c  ]----------------------------*/

int	set_value_of_element(char *value_line, int *idx)
{
	int	rgb;

	rgb = 0;
	while (value_line[++(*idx)] && ft_isdigit(value_line[*idx]))
		rgb = rgb * 10 + (value_line[*idx] - '0');
	if (rgb < 0 || 255 < rgb)
		exit_with_error("Error message");
	return (rgb);
}

int	set_value_of_color(char *value_line)
{
	int	idx;
	int	r;
	int	g;
	int	b;
	int color_num;

	check_color_value(value_line);
	idx = -1;
	r = set_value_of_element(value_line, &idx) * 256 * 256;
	g = set_value_of_element(value_line, &idx) * 256;
	b = set_value_of_element(value_line, &idx);
	color_num = r + g + b;
	return (color_num);
}

/*-------------------------------------------------------------------*/
/*----------------------------[ get_value.c  ]----------------------------*/

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

bool	get_value_of_path(char *path, char **dest)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		exit_with_error("Cannot open xpm file");
	close(fd);
	*dest = path;
	return (true);
}

int	get_value_of_color(char *line)
{
	char	*value_line;
	int		color_num;

	if (*line == '\0')
		exit_with_error("error_message");
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
	result_value = ft_strjoin(temp, line);

	free(temp);
	return (result_value);
}

/*-------------------------------------------------------------------*/
/*----------------------------[ set_type.c  ]----------------------------*/

void	set_type_of_component(t_img *img,  int type, char *line)
{
	char	*value_line;

	value_line = get_value_of_addr(line);
	if (type == NO)
	{
		if (img->no || !get_value_of_path(value_line, (&img->no)))
			exit_with_error("Invalid NO file");
	}
	else if (type == SO)
	{
		if (img->so || !get_value_of_path(value_line, (&img->so)))
			exit_with_error("Invalid SO file");
	}
	else if (type == WE)
	{
		if (img->we || !get_value_of_path(value_line, (&img->we)))
			exit_with_error("Invalid WE file");
	}
	else
	{
		if (img->ea || !get_value_of_path(value_line, (&img->ea)))
			exit_with_error("Invalid EA file");
	}
}

void	set_type_of_color(t_img *img, int type, char *line)
{
	if (type == F)
	{
		if (img->floor_color != INIT)
			exit_with_error("error_message");
		img->floor_color = get_value_of_color(line);
	}
	else if (type == C)
	{
		if (img->ceil_color != INIT)
			exit_with_error("error_message");
		img->ceil_color = get_value_of_color(line);
	}
}

void	set_type_of_map(t_map *map, t_img *img, char *line)
{
	check_saved_component(img);
	map->map_value = get_value_of_map(map, line);
}

void	set_type(t_map *map, t_img *img, int type, char *line)
{
	if (NO <= type && type <= EA)
		set_type_of_component(img, type, line);
	else if (F <= type && type <= C)
		set_type_of_color(img, type, line);
	else
		set_type_of_map(map, img, line);
}

/*-------------------------------------------------------------------*/
/*----------------------------[ get_type.c ]----------------------------*/

int	get_type_of_component(char *line)
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

int	get_type_of_color(char *line)
{
	if (!ft_strncmp(line, FLOOR, 2))
		return F;
	else if (!ft_strncmp(line, CEILING, 2))
		return C;
	else 
		return FALSE;
}

int	get_type_of_map(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (!is_contain("01NSEW ", line[i]))
			return (FALSE);
	}
	return (MAP);
}

int	get_type(char *line)
{
	int	type;
	
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
			set_type(&(game->map), &(game->map.img), type, line);
		}
		free(line);
		line = get_next_line(fd);
	}
	generate_map(&game->map);
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

printf("\033[0;32m[ Check saved Component value ]\n\033[0m");
printf("no value: ||\033[0;31m%s\033[0m||\n", game.map.img.no);
printf("so value: ||\033[0;31m%s\033[0m||\n", game.map.img.so);
printf("we value: ||\033[0;31m%s\033[0m||\n", game.map.img.we);
printf("ea value: ||\033[0;31m%s\033[0m||\n", game.map.img.ea);
printf("floor value: ||\033[0;31m%d\033[0m||\n", game.map.img.floor_color);
printf("ceil value: ||\033[0;31m%d\033[0m||\n", game.map.img.ceil_color);


printf("\n\033[0;32m[ Check saved Map ]\n\033[0m");
int	idx = -1;
while (game.map.saved_map[++idx])
	printf("\033[0;31m%s\033[0m\n", game.map.saved_map[idx]);

}

/*-------------------------------------------------------------------*/
