#ifndef CUB3D_H
# define CUB3D_H

# define RED    "\033[0;31m"
# define GREEN  "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE   "\033[0;34m"
# define RESET  "\033[0m"

# define TRUE 1
# define FALSE 0
# define ERROR -1

# define INIT -1

# define MAP_EXTENSION ".cub"
# define XPM_EXTENSION ".xpm"
# define FAIL 1

# define NORTH "NO "
# define SOUTH "SO "
# define WEST "WE "
# define EAST "EA "
# define FLOOR "F "
# define CEILING "C "

# define UNMOVABLE "1 "
# define MOVABLE "0NSEW"

# define NO 1
# define SO 2
# define WE 3
# define EA 4
# define F 5
# define C 6
# define MAP 7

typedef enum
{
	false = 0,
	true  = 1
}bool;

# include "get_next_line/get_next_line.h"
# include "libft/libft.h"
# include "minilibx_opengl_20191021/mlx.h"
# include <fcntl.h>

#include <stdio.h>

typedef struct	s_img
{
	char    *no;
	char    *so;
	char    *we;
	char    *ea;
    int     ceil_color;
    int     floor_color;
}				t_img;

typedef struct	s_player
{
	char	starting_initial;
	double		y;
	double		x;
	
}				t_player;

typedef struct	s_map
{
	t_img		img;
	t_player	player;
	char		*map_value;
	char		**saved_map;
	int			row;
	int			col;

}				t_map;

typedef struct	s_game
{
	void	*mlx;
	void	*win;
	t_map	map;
}				t_game;

#endif
