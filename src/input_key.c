#include "../include/cub3d.h"

int deal_key(int key_code, t_game *game)
{
  if (key_code == KEY_ESC)
    exit_event(game);
  else if (key_code == KEY_W || key_code == KEY_UP)
    parallel_move(game, 0);
  else if (key_code == KEY_S || key_code == KEY_DOWN)
    parallel_move(game, M_PI);
  else if (key_code == KEY_A)
    parallel_move(game, M_PI_2);
  else if (key_code == KEY_D)
    parallel_move(game, -M_PI_2);
  else if (key_code == KEY_RIGHT)
    rotate_move(game, -R_UNIT);
  else if (key_code == KEY_LEFT)
    rotate_move(game, R_UNIT);
  return (0);
}

int moveable(t_game *game, double nx, double ny)
{
  int x;
  int y;

  x = (int)nx;
  y = (int)ny;
  if (x < 0 || y < 0 || x >= game->map.row || y >= game->map.col)
    return (1);
  if (game->map.saved_map[x][y] > '0')
    return (0);
  else
    return (1);
}

void parallel_move(t_game *g, double angle)
{
  double nx;
  double ny;

  nx = g->px + (g->dirx * cos(angle) - g->diry * sin(angle)) * M_UNIT;
  ny = g->py + (g->dirx * sin(angle) + g->diry * cos(angle)) * M_UNIT;

  if (!moveable(g, nx, ny) || !moveable(g, -nx + g->planex * 0.25, ny + g->planey * 0.25) ||
      !moveable(g, nx + g->planex * 0.25, ny + g->planey * 0.25) ||
      !moveable(g, -nx + g->planex * 0.25, ny - g->planey * 0.25) ||
      !moveable(g, nx + g->planex * 0.25, ny - g->planey * 0.25))
    return;
  g->px = nx;
  g->py = ny;
}

void rotate_move(t_game *g, double angle)
{
  double tempx;
  double tempy;

  tempx = g->dirx;
  tempy = g->diry;
  g->dirx = tempx * cos(angle) - tempy * sin(angle);
  g->diry = tempx * sin(angle) + tempy * cos(angle);
  tempx = g->planex;
  tempy = g->planey;
  g->planex = tempx * cos(angle) - tempy * sin(angle);
  g->planey = tempx * sin(angle) + tempy * cos(angle);
}
