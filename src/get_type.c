#include "../include/cub3d.h"

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
