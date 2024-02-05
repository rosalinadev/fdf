/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 12:10:25 by rvandepu          #+#    #+#             */
/*   Updated: 2024/02/05 19:33:35 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

bool	alloc_mem(t_fdf *fdf, int depth)
{
	t_point	*line;

	if (depth == fdf->mesh->height)
		return (fdf->mem = ft_calloc(depth, sizeof(t_point *)), fdf->mem);
	line = ft_calloc(fdf->mesh->width, sizeof(t_point));
	if (line == NULL)
		return (false);
	if (!alloc_mem(fdf, depth + 1))
		return (free(line), false);
	return (fdf->mem[depth] = line, true);
}

void	free_mem(t_fdf *fdf)
{
	while (fdf->mesh->height--)
		free(fdf->mem[fdf->mesh->height]);
	free(fdf->mem);
}

static void	traverse(t_fdf *fdf, t_coords c, t_btree *node)
{
	node->checked = !node->checked;
	if (node->right)
	{
		fdf->mem[c.y][c.x + 1] = \
				fdf->proj(fdf, (t_coords){c.x + 1, c.y}, node->right);
		draw_line(fdf, fdf->mem[c.y][c.x], fdf->mem[c.y] + c.x + 1);
		if (node->checked != node->right->checked)
			traverse(fdf, (t_coords){c.x + 1, c.y}, node->right);
	}
	if (node->left)
	{
		fdf->mem[c.y + 1][c.x] = \
				fdf->proj(fdf, (t_coords){c.x, c.y + 1}, node->left);
		draw_line(fdf, fdf->mem[c.y][c.x], fdf->mem[c.y + 1] + c.x);
		if (node->checked != node->left->checked)
			traverse(fdf, (t_coords){c.x, c.y + 1}, node->left);
	}
}

static void	translate(t_fdf *fdf)
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
}

void	ft_hook_loop(void *param)
{
	t_fdf	*fdf;

	fdf = param;
	translate(fdf);
	if (ft_bit_check(fdf->flags, F_TOGGLE_PROJ) && fdf->proj == &proj3d)
		fdf->proj = &proj2d;
	else if (ft_bit_check(fdf->flags, F_TOGGLE_PROJ) && fdf->proj == &proj2d)
		fdf->proj = &proj3d;
	fdf->flags = ft_bit_clear(fdf->flags, F_TOGGLE_PROJ);
	if (fdf->mesh->values)
	{
		ft_bzero(fdf->screen->pixels, fdf->width * fdf->height * 4);
		fdf->mem[0][0] = fdf->proj(fdf, (t_coords){0, 0}, fdf->mesh->values);
		traverse(fdf, (t_coords){0, 0}, fdf->mesh->values);
	}
	if (ft_bit_check(fdf->flags, F_FULLSCREEN)
		|| ft_bit_check(fdf->flags, F_QUIT))
		mlx_close_window(fdf->mlx);
}
