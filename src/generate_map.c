#include "../include/cub3d.h"

void	set_player_position(t_map *map, int col, int row)
{
	if (map->player.starting_initial || map->player.y || map->player.x)
		exit_with_error("duplicated player value");
	map->player.starting_initial = map->saved_map[col][row];
	map->player.y = (double)col;
	map->player.x = (double)row;
	map->saved_map[col][row] = '0';
}

int	set_map_component(t_map *map, int col, int row)
{
	if (row == 0 || row == (int)ft_strlen(map->saved_map[col]) - 1 || \
			col == 0 || col == map->row - 1)
		return (-1);
	if (map->saved_map[col][row + 1] == ' ' || \
		map->saved_map[col][row - 1] == ' ')
		return (-1);
	if ((int)ft_strlen(map->saved_map[col - 1]) <= row || \
		map->saved_map[col - 1][row] == ' ' || \
		map->saved_map[col - 1][row] == '\0')
		return (-1);
	if ((int)ft_strlen(map->saved_map[col + 1]) <= row || \
		map->saved_map[col + 1][row] == ' ' || \
		map->saved_map[col + 1][row] == '\0')
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
					exit_with_error("A component of the map \
						was saved incorrectly");
			}
		}
	}
	if (!map->player.starting_initial)
		exit_with_error("Failed to save the player's location value");
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
	map->col = row;
	map->row = col;
	free(map->map_value);
	map->map_value = NULL;
}

void	generate_map(t_map *map)
{
	get_map(map);
	set_map(map);
}
