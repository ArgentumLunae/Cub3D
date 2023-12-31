/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   draw_images.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/19 12:50:01 by mteerlin      #+#    #+#                 */
/*   Updated: 2022/11/21 14:11:06 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "draw_images.h"
#include "cub3d.h"
#include "ft_memset.h"
#include <stdlib.h>

static void	set_colour_to_pixel(uint32_t rgba, u_int8_t *pixel)
{
	pixel[0] = ((rgba >> 24) & 0xFF);
	pixel[1] = ((rgba >> 16) & 0xFF);
	pixel[2] = ((rgba >> 8) & 0xFF);
	pixel[3] = (rgba & 0xFF);
}

static uint32_t	colour_from_pixel(u_int8_t *pixel)
{
	uint32_t	rgba;

	rgba = 0;
	rgba += pixel[0] << 24;
	rgba += pixel[1] << 16;
	rgba += pixel[2] << 8;
	rgba += pixel[3];
	return (rgba);
}

mlx_image_t	*set_background(t_scene *scene, t_cubed *cubed)
{
	mlx_image_t	*background;
	uint32_t	idx;

	background = mlx_new_image(scene->window, scene->window->width, \
								scene->window->height);
	if (!background)
		exit(EXIT_FAILURE);
	idx = 0;
	while (idx < (background->width * background->height / 2))
	{
		set_colour_to_pixel(cubed->ceiling.rgba, \
							&(background->pixels)[(sizeof(uint32_t) * idx)]);
		idx++;
	}
	while (idx < (background->width * background->height))
	{
		set_colour_to_pixel(cubed->floor.rgba, & \
			(background->pixels)[sizeof(uint32_t) * idx]);
		idx++;
	}
	mlx_image_to_window(scene->window, background, 0, 0);
	return (background);
}

static void	colour_from_texture(t_column *col, t_scene *scene, int ypix)
{
	double		scalar_y;
	uint32_t	texture_y;
	uint32_t	texture_pixel;

	(void)scene;
	scalar_y = (double)col->texture->height / (double)(col->bottom - col->top);
	texture_y = (col->start - col->top);
	texture_y += ypix - col->start;
	texture_y *= scalar_y;
	texture_pixel = (texture_y * col->texture->width) + col->texture_x;
	col->colour = colour_from_pixel(&col->texture->pixels[texture_pixel * 4]);
}

void	draw_column(int x, t_column *column, t_scene *scene)
{
	int		idx;
	int		ypix;

	ypix = column->start;
	idx = (column->start * scene->window->width) + x;
	while (idx < (column->end * scene->window->width))
	{
		colour_from_texture(column, scene, ypix);
		set_colour_to_pixel(column->colour, \
				&(scene->walls->pixels)[sizeof(uint32_t) * idx]);
		idx += scene->window->width;
		ypix++;
	}
}
