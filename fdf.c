/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 16:57:19 by rvandepu          #+#    #+#             */
/*   Updated: 2024/01/06 19:13:06 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

#define WIDTH 720
#define HEIGHT 480

static mlx_image_t	*g_screen;

void	ft_hook(void *watisdis)
{
	static int	x = 0;
	static int	y = 0;

	(void) watisdis;
	if (y == HEIGHT && x == WIDTH)
		return ;
	mlx_put_pixel(g_screen, x, y, 0xFF00FFFF);
	x++;
	if (x == WIDTH)
	{
		x = 0;
		y++;
	}
}

int	main(void)
{
	mlx_t		*mlx;

	mlx = mlx_init(WIDTH, HEIGHT, "fdf", false);
	if (!mlx)
		return (EXIT_FAILURE);
	g_screen = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!g_screen || (mlx_image_to_window(mlx, g_screen, 0, 0) == -1))
		return (EXIT_FAILURE);
	mlx_put_pixel(g_screen, 0, 0, 0xFF0000FF);
	mlx_loop_hook(mlx, &ft_hook, NULL);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}
