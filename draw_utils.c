/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 19:33:15 by rvandepu          #+#    #+#             */
/*   Updated: 2024/01/20 13:09:21 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

float	distance(t_vec2 a, t_vec2 b)
{
	return (sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2)));
}

void	draw_line(t_fdf *fdf, t_vec2 a, t_vec2 b, t_cols cols)
{
	t_vec2	incr;
	t_vec4	coli;
	t_vec4	col;
	float	steps;

	col = (t_vec4){(*cols >> 24 & 0xFF), (*cols >> 16 & 0xFF),
		(*cols >> 8 & 0xFF), (*cols & 0xFF)};
	steps = ceilf(distance(a, b) / 0.001);
	incr.x = (b.x - a.x) / steps;
	incr.y = (b.y - a.y) / steps;
	coli.x = ((float)((cols[1] >> 24) & 0xFF) - col.x) / steps;
	coli.y = ((float)((cols[1] >> 16) & 0xFF) - col.y) / steps;
	coli.z = ((float)((cols[1] >> 8) & 0xFF) - col.z) / steps;
	coli.w = ((float)(cols[1] & 0xFF) - col.w) / steps;
	while (steps-- > 0)
	{
		mlx_put_pixel(fdf->screen, (int) roundf(a.x), (int) roundf(a.y), \
				((int) roundf(col.x) << 24 | (int) roundf(col.y) << 16 \
				| (int) roundf(col.z) << 8 | (int) roundf(col.w)));
		a.x += ((a.y += incr.y), incr.x);
		col.x += ((col.y += coli.y), coli.x);
		col.z += ((col.w += coli.w), coli.z);
	}
}
