/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 19:33:15 by rvandepu          #+#    #+#             */
/*   Updated: 2024/01/22 14:26:19 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

float	distance(t_vec2 a, t_vec2 b)
{
	return (sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2)));
}

t_col	convert_to_col(unsigned int color)
{
	return ((t_col){(color >> 24 & 0xFF), (color >> 16 & 0xFF),
		(color >> 8 & 0xFF), (color & 0xFF)});
}

unsigned int	convert_from_col(t_col color)
{
	return ((int) roundf(color.r) << 24 | (int) roundf(color.g) << 16
		| (int) roundf(color.b) << 8 | (int) roundf(color.a));
}

void	draw_line(t_fdf *fdf, t_point a, t_point *b)
{
	t_vec2	incr;
	t_col	coli;
	float	steps;

	steps = ceilf(distance(a.pos, b->pos) / fdf->res);
	incr.x = (b->pos.x - a.pos.x) / steps;
	incr.y = (b->pos.y - a.pos.y) / steps;
	coli.r = (b->col.r - a.col.r) / steps;
	coli.g = (b->col.g - a.col.g) / steps;
	coli.b = (b->col.b - a.col.b) / steps;
	coli.a = (b->col.a - a.col.a) / steps;
	while (steps-- > 0)
	{
		mlx_put_pixel(fdf->screen, (int) roundf(a.pos.x),
			(int) roundf(a.pos.y), convert_from_col(a.col));
		a.pos.x += ((a.pos.y += incr.y), incr.x);
		a.col.r += ((a.col.g += coli.g), coli.r);
		a.col.b += ((a.col.a += coli.a), coli.b);
	}
}
