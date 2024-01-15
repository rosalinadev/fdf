/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 19:33:15 by rvandepu          #+#    #+#             */
/*   Updated: 2024/01/12 09:47:36 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

float	distance(t_vec2 a, t_vec2 b)
{
	return (sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2)));
}

void	draw_line(t_fdf *fdf, t_vec2 a, t_vec2 b)
{
	t_vec2	incr;
	float	steps;

	steps = ceilf(distance(a, b) / 0.001);
	incr.x = (b.x - a.x) / steps;
	incr.y = (b.y - a.y) / steps;
	while (steps-- > 0)
	{
		mlx_put_pixel(fdf->screen, (int) roundf(a.x), (int) roundf(a.y), 0xFFFFFFFF);
		a.x += incr.x;
		a.y += incr.y;
	}
}
