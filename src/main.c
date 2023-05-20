#include "cub3d.h"


	/**
	����
	1. ���ϸ� Ȯ���� Ȯ��
	2. open Ȯ��
	
	3. getNextLine �� ���� ���پ� �о����
		- ������� Ȯ�� (NO, SO ...)
		- ������Ұ� �ƴ϶�� (��ȿ��)������ Ȯ��

		- validate �����ٸ� data ����
		- ������� �� �������� ������Ҷ�� �̸� ���� ���
		- �̹� ���� �ִٸ� �ߺ������� ����
		- ������Ҹ� �ǳ� ���� ����
		- xpm ������ ���������Ƿ� open �Ͽ� ��ȿ�� �������� Ȯ��
		- ������ ũ�⸸ŭ �Ҵ� ft_strdup()

		- ������� �� �����̶�� ���� ��ȿ�� üũ
		- �̹� ���� �ִ��� Ȯ��
		- ������Ҹ� �ǳ� ���� ����
		- .�� �������� ���� ������
		- �������� Ȯ��
		- �迭�� 3ĭ �Ѿ���� Ȯ��
		- ���ڸ� ���ڷ� ����
		- ������ ���ڰ� 0 ~ 255 ���ڰ� �ƴ� ��� Ȯ��
		- ������� = r * 256 * 256 + g * 256 + b
		- ������ڸ� ����
		- F, C �� ���� ���ڸ� ����

	4. map �� ���Դµ� �� ������ �ϳ��� ����Ǿ����� ������ �ȵȴ�
		���� ��� ���� ����Ǿ����� Ȯ��, ������ ���� �ٸ��� Ȯ��
		map�� 1���� �迭�� �̾� ���̱� strjoin()
		���� ������ �о��ٸ� parse map

		���� �������� split
		row ���� col ������ ft_strlen() ���� ���ϱ�
		map �� row col ����


	5. �� �Ҵ��� �Ϸ�Ǿ��ٸ� map setting
		1 �̳� ������ ������ �׳� �Ѿ�� 0NSEW �� ������ setting
		0NSEW �� �׵θ��� ������ ����
		0NSEW �ֺ��� �����̸� ����
		0NSEW ���� 0�� �ƴ϶�� player position save	
		�������ְ� �ش� ��ġ�� 0���� ����

	*/

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

void	set_component_type(t_map *map,  int type, char *line)
{
	char	*value_line;

	value_line = get_value_point(line);
printf("type is: :::%d:::\n", type);
	if (type == NO)
	{
		if (map->img->no || !set_path_in_map(value_line, (&map->img->no)))
			exit_with_error("Invalid NO file");
	}
	else if (type == SO)
	{
		if (map->img->so || !set_path_in_map(value_line, (&map->img->so)))
			exit_with_error("Invalid SO file");
	}
	else if (type == WE)
	{
		if (map->img->we || !set_path_in_map(value_line, (&map->img->we)))
			exit_with_error("Invalid WE file");
	}
	else
	{
		if (map->img->ea || !set_path_in_map(value_line, (&map->img->ea)))
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

void	set_color_type(t_map *map, int type, char *line)
{
	if (type == F)
	{
		if (map->img->floor_color != -1)
			exit_with_error("error_message");
		map->img->floor_color = get_color_num(line);
	}
	else if (type == C)
	{
		if (map->img->ceil_color != -1)
			exit_with_error("error_message");
		map->img->ceil_color = get_color_num(line);
	}
}

void	check_saved_component(t_map *map)
{
	if (!is_valid_extention(map->img->no, XPM_EXTENSION))
		exit_with_error("Error Message");
	if (!is_valid_extention(map->img->so, XPM_EXTENSION))
		exit_with_error("Error Message");
	if (!is_valid_extention(map->img->we, XPM_EXTENSION))
		exit_with_error("Error Message");
	if (!is_valid_extention(map->img->ea, XPM_EXTENSION))
		exit_with_error("Error Message");
	if (map->img->floor_color == INIT || map->img->ceil_color == INIT || \
		map->img->floor_color == map->img->ceil_color)
		exit_with_error("Invalid color value");
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

void	set_map_type(t_map *map, char *line)
{
	// check_saved_component(map);
	map->map_value = get_map_value(map, line);

}

void	set_date(t_map *map, int type, char *line)
{
	if (NO <= type && type <= EA)
		set_component_type(map, type, line);
	else if (F <= type && type <= C)
		set_color_type(map, type, line);
	else
		set_map_type(map, line);
}

void	get_map_size(t_map *map)
{
	int	col;
	int	row;

	map->saved_map = ft_split(map->map_value, '\n');
	if (map->saved_map == 0)
		exit_with_error("error message");
	col = -1;
	row = 0;
	while (map->saved_map[++col] != 0)
		if (ft_strlen(map->saved_map[col]) > (size_t)row)
			row = ft_strlen(map->saved_map[col]);
	map->col = col;
	map->row = row;
	map->map_value = 0;
}

void	init_game(t_game *game, int fd)
{
	int		type;
	char	*line;
	t_map	tmp_map;
	t_img	tmp_img;
	
	game->map = &tmp_map;
	ft_memset(&tmp_map, 0, sizeof(t_map));
	game->map->img = &tmp_img;
	ft_memset(&tmp_img, 0, sizeof(t_img));
	tmp_img.floor_color = INIT;
	tmp_img.ceil_color = INIT;

	line = get_next_line(fd);
	while (line  != NULL)
	{
		line[ft_strlen(line) - 1] = '\0';
		if (line[0] != '\0')
		{
			type = get_type(line);
			if (!type)
				exit_with_error("Invalid type");
			set_date(&tmp_map, type, line);
		}
		free(line);
		line = get_next_line(fd);
	}
	// set_date(game->map, type, line);
	get_map_size(game->map);
	close(fd);
	free(line);
printf("6. map's no: %s\n", game->map->img->no);
}

// void	check_saved_map(t_map *map)
// {
// 	int	row;
// 	int	col;

// 	col = -1;
// 	while (map->saved_map[++col])
// 	{
// 		row = -1;
// 		while (map->saved_map[col][++row])
// 		{
// 			if (ft_strchr("1 ", map->saved_map[col][row]) != NULL)
// 				continue ;
// 			else if (ft_strchr("0NSEW", map->saved_map[col][row]) != NULL)
// 			{
// 				if (setting_handler(map, row, col) == ERROR)
// 					exit_error(map, "Invalid Map");
// 			}
// 		}
// 	}
// 	if (!map->player.starting_sight)
// 		exit_error(map, "Invalid Map");
// }


int	main(int	argc, char	**argv)
{
	int		fd;
	t_game	game;

	if (argc != 2)
		exit_with_error("Error Message");
	fd = get_file_fd(argv[1]);
	init_game(&game, fd);
	// check_saved_map(game->map);

	int	idx = -1;

printf("1: %s\n", game.map->img->no);
printf("2: %s\n", game.map->img->so);
printf("3: %s\n", game.map->img->we);
printf("4: %s\n", game.map->img->ea);

printf(">>>>>> test for save map!! <<<<<<\n\n");

	while (game.map->saved_map[++idx])
		printf("%s\n", game.map->saved_map[idx]);
}

