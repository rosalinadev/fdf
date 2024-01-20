/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 16:57:19 by rvandepu          #+#    #+#             */
/*   Updated: 2024/01/20 13:03:05 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

#define WIDTH 720
#define HEIGHT 480

void	ft_hook(void *state)
{
	static int	x = 0;
	static int	y = 0;

	if (y == HEIGHT && x == WIDTH)
		return ;
	mlx_put_pixel(((t_fdf *)state)->screen, x, y, 0xFF00FFFF);
	x++;
	if (x == WIDTH)
	{
		x = 0;
		y++;
	}
}

#define SCALE 25

void	traverse(t_fdf *fdf, t_btree *node, int x, int y)
{
	ft_printf("[node] val: %d\n", node->value);
	ft_printf("       color: %#X\n", node->color);
	if (node->left)
	{
		ft_printf("       left: %d\n", node->left->value);
		draw_line(fdf, (t_vec2){x, y}, (t_vec2){x, y + SCALE}, (t_cols){node->color, node->left->color});
	}
	if (node->right)
	{
		ft_printf("       right: %d\n", node->right->value);
		draw_line(fdf, (t_vec2){x, y}, (t_vec2){x + SCALE, y}, (t_cols){node->color, node->right->color});
	}
	node->checked = !node->checked;
	if (node->left && node->checked != node->left->checked)
	{
		ft_printf("[%2d]   going left...\n", node->value);
		traverse(fdf, node->left, x, y + SCALE);
	}
	if (node->right && node->checked != node->right->checked)
	{
		ft_printf("[%2d]   going right...\n", node->value);
		traverse(fdf, node->right, x + SCALE, y);
	}
}

int	main(int argc, char *argv[])
{
	t_fdf	fdf;
	t_mesh	*mesh;

	if (argc < 2)
		return (EXIT_FAILURE);
	mesh = load_map(argc - 1, argv + 1);
	ft_printf("mesh: %p\n", mesh);
	//return (EXIT_SUCCESS);
	fdf.mlx = mlx_init(WIDTH, HEIGHT, "fdf", false);
	if (!fdf.mlx)
		return (EXIT_FAILURE);
	fdf.screen = mlx_new_image(fdf.mlx, WIDTH, HEIGHT);
	if (!fdf.screen || (mlx_image_to_window(fdf.mlx, fdf.screen, 0, 0) == -1))
		return (EXIT_FAILURE);
	mlx_put_pixel(fdf.screen, 0, 0, 0xFF0000FF);
	//draw_line(&fdf, (t_vec2){5, 5}, (t_vec2){500, 300});
	//draw_line(&fdf, (t_vec2){400, 5}, (t_vec2){5, 400});
	if (mesh && mesh->values) traverse(&fdf, mesh->values, 10, 10);
	mlx_loop_hook(fdf.mlx, &ft_hook, &fdf);
	mlx_loop(fdf.mlx);
	mlx_terminate(fdf.mlx);
	free_mesh(mesh, NULL);
	return (EXIT_SUCCESS);
}
