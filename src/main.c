#include "cub3d.h"


	/**
	순서
	1. 파일명 확장자 확인
	2. open 확인
	
	3. getNextLine 을 통해 한줄씩 읽어오기
		- 구성요소 확인 (NO, SO ...)
		- 구성요소가 아니라면 (유효한)맵인지 확인

		- validate 끝났다면 data 저장
		- 구성요소 중 동서남북 구성요소라면 이를 먼저 고려
		- 이미 값이 있다면 중복됨으로 에러
		- 구성요소를 건너 값에 접근
		- xpm 파일이 적혀있으므로 open 하여 유효한 파일인지 확인
		- 파일의 크기만큼 할당 ft_strdup()

		- 구성요소 중 색깔이라면 색깔 유효성 체크
		- 이미 값이 있는지 확인
		- 구성요소를 건너 값에 접근
		- .를 기준으로 숫자 나누기
		- 숫자인지 확인
		- 배열이 3칸 넘어가는지 확인
		- 문자를 숫자로 변경
		- 각각의 숫자가 0 ~ 255 숫자가 아닐 경우 확인
		- 색깔숫자 = r * 256 * 256 + g * 256 + b
		- 색깔숫자를 리턴
		- F, C 에 색깔 숫자를 저장

	4. map 이 나왔는데 위 사항이 하나라도 저장되어있지 않으면 안된다
		따라서 모든 곳이 저장되었는지 확인, 색깔이 서로 다른지 확인
		map을 1차원 배열로 이어 붙이기 strjoin()
		파일 끝까지 읽었다면 parse map

		개행 기준으로 split
		row 별로 col 갯수를 ft_strlen() 으로 구하기
		map 의 row col 저장


	5. 맵 할당이 완료되었다면 map setting
		1 이나 공백을 만나면 그냥 넘어가고 0NSEW 를 만나면 setting
		0NSEW 가 테두리에 있으면 에러
		0NSEW 주변에 공백이면 에러
		0NSEW 에서 0이 아니라면 player position save	
		저장해주고 해당 위치는 0으로 변경

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

