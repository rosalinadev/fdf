/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 19:33:15 by rvandepu          #+#    #+#             */
/*   Updated: 2024/01/28 23:27:24 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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
	t_vec2		incr;
	t_col		coli;
	float		steps;
	t_coords	c;

	steps = ceilf(sqrtf(powf(a.pos.x - b->pos.x, 2)
				+ powf(a.pos.y - b->pos.y, 2)) / fdf->res);
	incr.x = (b->pos.x - a.pos.x) / steps;
	incr.y = (b->pos.y - a.pos.y) / steps;
	coli.r = (b->col.r - a.col.r) / steps;
	coli.g = (b->col.g - a.col.g) / steps;
	coli.b = (b->col.b - a.col.b) / steps;
	coli.a = (b->col.a - a.col.a) / steps;
	if (steps > fdf->maxsteps)
		return ;
	while (steps-- > 0)
	{
		c = (t_coords){roundf(a.pos.x), roundf(a.pos.y)};
		if (0 <= c.x && c.x < fdf->width && 0 <= c.y && c.y < fdf->height)
			mlx_put_pixel(fdf->screen, c.x, c.y, convert_from_col(a.col));
		a.pos.x += ((a.pos.y += incr.y), (a.col.r += coli.r), incr.x);
		a.col.g += ((a.col.b += coli.b), (a.col.a += coli.a), coli.g);
	}
}
