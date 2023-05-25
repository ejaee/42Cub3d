#include "../include/cub3d.h"

void draw_background(t_game *g)
{
  int y;
  int x;

  y = -1;
  while (++y < SCREEN_HEIGHT)
  {
    x = -1;
    while (++x < SCREEN_WIDTH)
    {
      if (y < SCREEN_HEIGHT / 2)
        g->screen_data[y * SCREEN_WIDTH + x] = g->img.ceil_color;
      else if (y > SCREEN_HEIGHT / 2)
        g->screen_data[y * SCREEN_WIDTH + x] = g->img.floor_color;
      else
        g->screen_data[y * SCREEN_WIDTH + x] = 0;
    }
  }
}

void ray_cal_init(t_game *g, int x)
{
  g->camerax = 2 * x / (double)(SCREEN_WIDTH - 1) - 1;
  g->raydirx = g->dirx + g->planex * g->camerax;
  g->raydiry = g->diry + g->planey * g->camerax;
  g->mapx = (int)(g->px);
  g->mapy = (int)(g->py);
  
  g->deltadistx = (g->raydiry == 0) ? 0 : ((g->raydirx == 0) ? 1 : fabs(1 / g->raydirx));
  g->deltadisty = (g->raydirx == 0) ? 0 : ((g->raydiry == 0) ? 1 : fabs(1 / g->raydiry));
  getsidedist(g);

  g->hit = 0;
}

void getsidedist(t_game *g)
{
  if (g->raydirx < 0)
  {
    g->stepx = -1;
    g->sidedistx = (g->px - g->mapx) * g->deltadistx;
  }
  else
  {
    g->stepx = 1;
    g->sidedistx = (g->mapx + 1.0 - g->px) * g->deltadistx;
  }
  if (g->raydiry < 0)
  {
    g->stepy = -1;
    g->sidedisty = (g->py - g->mapy) * g->deltadisty;
  }
  else
  {
    g->stepy = 1;
    g->sidedisty = (g->mapy + 1.0 - g->py) * g->deltadisty;
  }
}
