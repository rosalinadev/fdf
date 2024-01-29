/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 12:10:25 by rvandepu          #+#    #+#             */
/*   Updated: 2024/01/28 17:30:48 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	traverse(t_fdf *fdf, t_coords c, t_btree *node, t_point *last)
{
	t_point	next;

	if (last == NULL)
		return (next = fdf->proj(fdf, c, node), traverse(fdf, c, node, &next));
	node->checked = !node->checked;
	if (node->right)
	{
		next = fdf->proj(fdf, (t_coords){c.x + 1, c.y}, node->right);
		draw_line(fdf, *last, &next);
		if (node->checked != node->right->checked)
			traverse(fdf, (t_coords){c.x + 1, c.y}, node->right, &next);
	}
	if (node->left)
	{
		next = fdf->proj(fdf, (t_coords){c.x, c.y + 1}, node->left);
		draw_line(fdf, *last, &next);
		if (node->checked != node->left->checked)
			traverse(fdf, (t_coords){c.x, c.y + 1}, node->left, &next);
	}
}

static void	handle_keys(t_fdf *fdf)
{
	static double	last_time = 0;
	double			time;
	double			upf;

	time = mlx_get_time();
	upf = (time - last_time) * 100 / fdf->zoom;
	if (ft_bit_check(fdf->flags, F_FORWARDS))
		fdf->trans.z -= upf;
	else if (ft_bit_check(fdf->flags, F_BACKWARDS))
		fdf->trans.z += upf;
	if (ft_bit_check(fdf->flags, F_LEFT))
		fdf->trans.x -= upf;
	else if (ft_bit_check(fdf->flags, F_RIGHT))
		fdf->trans.x += upf;
	if (ft_bit_check(fdf->flags, F_UP))
		fdf->trans.y += upf;
	else if (ft_bit_check(fdf->flags, F_DOWN))
		fdf->trans.y -= upf;
	last_time = time;
	if (ft_bit_check(fdf->flags, F_TOGGLE_PROJ) && fdf->proj == &proj3d)
		fdf->proj = &proj2d;
	else if (ft_bit_check(fdf->flags, F_TOGGLE_PROJ) && fdf->proj == &proj2d)
		fdf->proj = &proj3d;
	fdf->flags = ft_bit_clear(fdf->flags, F_TOGGLE_PROJ);
}

void	ft_hook_loop(void *param)
{
	t_fdf	*fdf;

	fdf = param;
	handle_keys(fdf);
	if (fdf->mesh->values)
	{
		ft_bzero(fdf->screen->pixels, fdf->width * fdf->height * 4);
		traverse(fdf, (t_coords){0, 0}, fdf->mesh->values, NULL);
	}
}
