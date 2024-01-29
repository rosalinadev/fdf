/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 19:47:44 by rvandepu          #+#    #+#             */
/*   Updated: 2024/01/29 01:20:12 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_point	proj2d(t_fdf *fdf, t_coords c, t_btree *node)
{
	t_point	p;

	p.pos.x = (c.x - ((fdf->mesh->width - 1) / 2.0) + fdf->trans.x) * fdf->zoom
		+ (fdf->width / 2.0);
	p.pos.y = (c.y - ((fdf->mesh->height - 1) / 2.0) + fdf->trans.z) * fdf->zoom
		+ (fdf->height / 2.0);
	p.col = convert_to_col(node->color);
	return (p);
}

t_point	proj3d(t_fdf *fdf, t_coords coords, t_btree *node)
{
	t_vec3	a;
	t_point	b;
	t_vec2	t;

	t = fdf->rot;
	a.x = coords.x - ((fdf->mesh->width - 1) / 2.0) + fdf->trans.x;
	a.z = coords.y - ((fdf->mesh->height - 1) / 2.0) + fdf->trans.z;
	a.y = -(node->value * fdf->scale + fdf->trans.y);
	b.pos.x = cosf(t.y) * a.x - sinf(t.y) * a.z;
	b.pos.y = sinf(t.x) * sinf(t.y) * a.x + cosf(t.x) * a.y
		+ sinf(t.x) * cosf(t.y) * a.z;
	b.pos.x = b.pos.x * fdf->zoom + fdf->width / 2.0;
	b.pos.y = b.pos.y * fdf->zoom + fdf->height / 2.0;
	b.col = convert_to_col(node->color);
	return (b);
}
