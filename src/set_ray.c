#include "../include/cub3d.h"

void  draw_background(t_game *g)
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
  // 이 부분 유도 과정 그림 그려놓기
  get_sidedist(g);
  g->hit = 0;
}

void get_sidedist(t_game *g)
{
  g->stepx = (g->raydirx < 0) ? -1 : 1;
  g->sidedistx = (g->raydirx < 0) ? (g->px - g->mapx) * g->deltadistx : (g->mapx + 1.0 - g->px) * g->deltadistx;
  g->stepy = (g->raydiry < 0) ? -1 : 1;
  g->sidedisty = (g->raydiry < 0) ? (g->py - g->mapy) * g->deltadisty : (g->mapy + 1.0 - g->py) * g->deltadisty;
}
