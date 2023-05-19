#ifndef CUB3D_H
# define CUB3D_H

# define FAIL 1

typedef struct	s_img
{
	void	*img;
	int		*data;
	int		dpp
}				t_img;

typedef struct	s_texture
{
	char	*tex_path_malloc;

}				t_texture;


typedef struct	s_map
{
	t_texture	texture[4];
	char		*tmp_map;
	char		**map;
	int			f_color;
	int			c_color;
	int			row;
	int			col;

}				t_map;

typedef struct	s_player
{
	char	starting_initial;
	double		px;
	double		py;
	
}				t_player;

typedef struct	s_game
{
	void	*mlx;
	void	*win;
	t_map	*map;
	
}				t_game;


#endif