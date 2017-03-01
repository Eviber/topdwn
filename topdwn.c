#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WIN_W 1280
#define WIN_H 720
#define TABSIZE 100
#define SPEED 10
#define FPS 25
#define ANIM_FPS 10

#define UP    keys[SDL_SCANCODE_UP]
#define DOWN  keys[SDL_SCANCODE_DOWN]
#define LEFT  keys[SDL_SCANCODE_LEFT]
#define RIGHT keys[SDL_SCANCODE_RIGHT]

int		init(SDL_Window **win, SDL_Renderer **render)
{
	srand(time(NULL));
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
	{
		fprintf(stderr, "Error while initializing SDL: %s\n", SDL_GetError());
		return (1);
	}
	IMG_Init(IMG_INIT_PNG);
	*win = SDL_CreateWindow("topdwn",
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
	SDL_SetRenderDrawColor(*render, 58, 2, 13, 255);
	SDL_RenderClear(*render);
	return (0);
}

SDL_Texture	*load_img(SDL_Renderer *render, char *path)
{
	SDL_Surface	*surf;
	SDL_Texture	*tex;

	if (!(surf = IMG_Load(path)))
	{
		fprintf(stderr, "Error while loading img");
		exit(EXIT_FAILURE);
	}
	if (!(tex = SDL_CreateTextureFromSurface(render, surf)))
	{
		fprintf(stderr, "Error while loading img");
		exit(EXIT_FAILURE);
	}
	SDL_FreeSurface(surf);
	return (tex);
}

void	loop(SDL_Renderer *render)
{
	SDL_Rect		rect;
	SDL_Rect		src;
	SDL_Texture		*tex;
	const Uint8		*keys = SDL_GetKeyboardState(NULL);
	int				xmov;
	int				ymov;
	int				i;
	int				dir;
	unsigned int	anim_ticks;
	unsigned int	ticks;

	tex = load_img(render, "test.png");
	rect.w = 50;
	rect.h = 50;
	rect.x = (WIN_W / 2) - (rect.w / 2);
	rect.y = (WIN_H / 2) - (rect.h / 2);
	src.h = 50;
	src.w = 50;
	i = 0;
	anim_ticks = 0;
	dir = 5;
	while (!SDL_QuitRequested())
	{
		xmov = 0;
		ymov = 0;
		SDL_PumpEvents();
		if (UP || DOWN || LEFT || RIGHT)
		{
			dir = 0;
			if (UP)
				dir = 2;
			if (DOWN)
				dir = 5;
			if (RIGHT)
				dir += 7;
			else if (LEFT)
				dir += 6;
			dir = dir % 8;
		}
		xmov = (LEFT || RIGHT) * SPEED * (-1 * (!RIGHT) + RIGHT);
		ymov = (UP || DOWN) * SPEED * (-1 * (!DOWN) + DOWN);
		if (xmov && ymov)
		{
			xmov /= 2;
			ymov /= 2;
		}
		printf("dir = %d\n", dir);
		rect.x += xmov;
		rect.y += ymov;
		src.x = i * 50;
		src.y = dir * 50;
		SDL_RenderClear(render);
		SDL_RenderCopy(render, tex, &src, &rect);
		SDL_RenderPresent(render);
		ticks = SDL_GetTicks() + (1000 / FPS);
		if ((UP || DOWN || LEFT || RIGHT))
		{
			if (SDL_GetTicks() >= anim_ticks)
			{
				i = (i == 4) ?  1 : i + 1;
				anim_ticks = SDL_GetTicks() + (1000 / ANIM_FPS);
			}
		}
		else
			i = 0;
		while (SDL_GetTicks() < ticks);
//		SDL_Delay(1000 / FPS);
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
	loop(render);
	IMG_Quit();
	SDL_Quit();
	return (0);
}
