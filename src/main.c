#include "cub3d.h"

void free_all_data(t_game *game, int idx)
{
	while (++idx <= 4)
	{
		if (game->tex[idx].tex_path_malloc)
		{
			free(game->tex[idx].tex_path_malloc);
			game->tex[idx].tex_path_malloc = NULL;
			free(game->tex[idx].data);
			game->tex[idx].data = NULL;
		}
	}
	if (game->map.map_value)
		free(game->map.map_value);
	game->map.map_value = NULL;
	if (game->map.saved_map)
	{
		idx = -1;
		while (game->map.saved_map[++idx])
		{
			free(game->map.saved_map[idx]);
			game->map.saved_map[idx] = NULL;
		}
		free(game->map.saved_map);
	}
}

/*----------------------------[ error.c ]----------------------------*/

int get_file_fd(char *file_name)
{
	int fd;

	if (!is_valid_extention(file_name, MAP_EXTENSION))
		exit_with_error("Invalid Extension");
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		exit_with_error("Cannot Open File");
	return (fd);
}

/////////////////////////////////////////////////////////////

/// deal keyboard input //////////////////////

///////////////////////////////



void check()
{
	system("leaks --list -- cub3D");
}

int main(int argc, char **argv)
{
	atexit(check);
	int fd;
	t_game game;

	if (argc != 2)
		exit_with_error("Error Message");
	fd = get_file_fd(argv[1]);
	init_game(&game, fd);

	init_coordinates(&game); // game_init
	init_window(&game);
	init_img(&game);
	printf(" any problem ??? \n\n");

	mlx_hook(game.win, X_EVENT_KEY_PRESS, 0, &deal_key, &game);
	mlx_hook(game.win, X_EVENT_KEY_EXIT, 0, &exit_event, &game);
	printf(" ssibal ssibal \n\n");
	mlx_loop_hook(game.mlx, &main_loop, &game);

	printf(" =============== check texture part ===============\n");
	printf(" height of textures %d\n ", game.tex[1].height);
	printf(" height of textures %d\n ", game.tex[2].height);
	printf(" height of textures %d\n ", game.tex[3].height);
	printf(" height of textures %d\n ", game.tex[4].height);

	printf("\033[0;32m[ Check saved Component value ]\n\033[0m");
	printf(" value of textures %s\n ", game.tex[1].tex_path_malloc);
	printf(" value of textures %s\n ", game.tex[2].tex_path_malloc);
	printf(" value of textures %s\n ", game.tex[3].tex_path_malloc);
	printf(" value of textures %s\n\n\n ", game.tex[4].tex_path_malloc);
	printf("floor value: ||\033[0;31m%d\033[0m||\n", game.img.floor_color);
	printf("ceil value: ||\033[0;31m%d\033[0m||\n", game.img.ceil_color);

	printf("\n\033[0;32m[ Check saved Map ]\n\033[0m");
	int idx = -1;
	while (game.map.saved_map[++idx])
		printf("\033[0;31m%s\033[0m\n", game.map.saved_map[idx]);

	mlx_loop(game.mlx);

	// printf(" value of textures %s\n ", game.map.tex[0].tex_path_malloc);

	free_all_data(&game, 0); // fixed
}

/*-------------------------------------------------------------------*/
