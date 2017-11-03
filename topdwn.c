#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>

#define WIN_W 1920/2
#define WIN_H 1080/2
#define TABSIZE 100
#define SPEED 8
#define FPS 30
#define ANIM_FPS 15
#define FRAMES 12
#define SPRITE_W 32
#define SPRITE_H 32

#define UP    keys[SDL_SCANCODE_UP]
#define DOWN  keys[SDL_SCANCODE_DOWN]
#define LEFT  keys[SDL_SCANCODE_LEFT]
#define RIGHT keys[SDL_SCANCODE_RIGHT]

typedef struct			s_entity
{
	SDL_Rect		dst;
	SDL_Rect		src;
	SDL_Texture		*tex;
	SDL_Point		mov;
	int				dir;
}						t_entity;

int		init(SDL_Window **win, SDL_Renderer **render)
{
	srand(time(NULL));
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
	{
		fprintf(stderr, "Error while initializing SDL: %s\n", SDL_GetError());
		return (1);
	} IMG_Init(IMG_INIT_PNG); *win = SDL_CreateWindow("topdwn",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			WIN_W, WIN_H, 0);
	if (!*win)
	{
		fprintf(stderr, "Error while creating window: %s\n", SDL_GetError());
		return (2);
	}
	*render = SDL_CreateRenderer(*win, -1, SDL_RENDERER_ACCELERATED);
	if (!*render)
	{
		fprintf(stderr, "Error while creating renderer: %s\n", SDL_GetError());
		return (3);
	}
	SDL_SetRenderDrawColor(*render, 22, 64, 127, 255);
	SDL_RenderClear(*render);
	return (0);
}

SDL_Texture	*load_img(SDL_Renderer *render, char *path)
{
	SDL_Surface	*surf;
	SDL_Texture	*tex;

	if (!(surf = IMG_Load(path)))
	{
		fprintf(stderr, "Error while loading img: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	if (!(tex = SDL_CreateTextureFromSurface(render, surf)))
	{
		fprintf(stderr, "Error while loading img: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_FreeSurface(surf);
	return (tex);
}

void	load_player(SDL_Renderer *render, t_entity *p)
{
	p->tex = load_img(render, "sprite.png");
	p->dst.w = SPRITE_W * 4;
	p->dst.h = SPRITE_H * 4;
	p->dst.x = (WIN_W / 2) - (p->dst.w / 2);
	p->dst.y = (WIN_H / 2) - (p->dst.h / 2);
	p->src.w = SPRITE_W;
	p->src.h = SPRITE_H;
}
 
void	handle_key(SDL_Window *win, const Uint8 *keys, int *dir, SDL_Point *mov)
{
	if (keys[SDL_SCANCODE_F])
		SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
	if (keys[SDL_SCANCODE_ESCAPE])
		SDL_SetWindowFullscreen(win, 0);
	if (DOWN || LEFT)
	{
		*dir = 1;
		if (LEFT)
			*dir = 2;
	}
	if (UP || RIGHT)
	{
		*dir = 0;
		if (RIGHT)
			*dir = 3;
	}
	if (DOWN && RIGHT)
		*dir = 1;
	mov->x = 0;
	mov->y = 0;
	SDL_PumpEvents();
	mov->x = (LEFT || RIGHT) * SPEED * (-1 * (!RIGHT) + RIGHT);
	mov->y = (UP || DOWN) * SPEED * (-1 * UP + (!UP));
}

void	move(SDL_Rect *dst, SDL_Point *mov)
{
	SDL_Point	p;
	SDL_Point	initspeed;

	initspeed = *mov;
	p.x = dst->x + mov->x;
	p.y = dst->y + mov->y;
	mov->x = (p.x < 0) ? mov->x - (dst->x + mov->x) : mov->x;
	mov->x = (p.x + dst->w > WIN_W) ? mov->x - (p.x + dst->w - WIN_W) : mov->x;
	mov->y = (p.y < 0) ? mov->y - (dst->y + mov->y) : mov->y;
	mov->y = (p.y + dst->h > WIN_H) ? mov->y - (p.y + dst->h - WIN_H) : mov->y;
	if (mov->x && mov->y && mov->x == initspeed.x && mov->y == initspeed.y)
	{
		mov->x = initspeed.x / 2;
		mov->y = initspeed.y / 2;
	}
	dst->x += mov->x;
	dst->y += mov->y;
}

void	anim(SDL_Rect *src, int dir, const Uint8 *keys)
{
	static unsigned int	anim_ticks = 0;
	static int			i = 0;

	src->x = i * SPRITE_W;
	src->y = dir * SPRITE_H;
	if ((UP || DOWN || LEFT || RIGHT))
	{
		if (SDL_GetTicks() >= anim_ticks)
		{
			i = (i == FRAMES) ?  1 : i + 1;
			anim_ticks = SDL_GetTicks() + (1000 / ANIM_FPS);
		}
	}
	else
		i = 0;
}

void	loop(SDL_Window *win, SDL_Renderer *render)
{
	t_entity		p;
	const Uint8		*keys = SDL_GetKeyboardState(NULL);
	unsigned int	ticks;

	load_player(render, &p);
	p.dir = 2;
	while (!SDL_QuitRequested())
	{
		handle_key(win, keys, &(p.dir), &(p.mov));
		move(&(p.dst), &(p.mov));
		anim(&(p.src), p.dir, keys);
		ticks = SDL_GetTicks() + (1000 / FPS);
//		SDL_SetRenderDrawColor(render, 127, 85, 22, 255);
//		SDL_RenderFillRect(render, &(p.dst));
//		SDL_SetRenderDrawColor(render, 22, 64, 127, 255);
		SDL_RenderCopy(render, p.tex, &(p.src), &(p.dst));
		SDL_RenderPresent(render);
		SDL_RenderClear(render);
		while (SDL_GetTicks() < ticks);
	}
}

int		main(void)
{
	SDL_Window		*win;
	SDL_Renderer	*render;
	int				i;

	if((i = init(&win, &render)))
		return (i);
	i = 0;
	loop(win, render);
	IMG_Quit();
	SDL_Quit();
	return (0);
}
