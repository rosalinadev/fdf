/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 16:57:19 by rvandepu          #+#    #+#             */
/*   Updated: 2024/01/28 03:58:39 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	parse_args(t_fdf *fdf, int argc, char *argv[])
{
	if (argc < 2)
		return (ft_printf("Usage: %s <file> [--fullscreen] [color] [alpha]\n",
				argv[0]), -1);
	fdf->args.map_path = argv[1];
	if (argc > 2)
		fdf->args.fullscreen = !ft_strcmp(argv[2], "--fullscreen");
	else
		fdf->args.fullscreen = false;
	if (argc > 3)
		fdf->args.default_color = parse_nbr_skip(&argv[3]);
	else
		fdf->args.default_color = DEFAULT_COLOR;
	if (argc > 4)
		fdf->args.default_alpha = parse_nbr_skip(&argv[4]);
	else
		fdf->args.default_alpha = DEFAULT_ALPHA;
	if (fdf->args.default_color < 0 || fdf->args.default_alpha < 0)
		return (-1);
	return (0);
}

static void	init_fdf(t_fdf *fdf)
{
	fdf->width = 480;
	fdf->height = 320;
	// TODO toggle fullscreen?
	if (fdf->args.fullscreen)
	{
		mlx_set_setting(MLX_HEADLESS, true);
		fdf->mlx = mlx_init(fdf->width, fdf->height, "", false);
		mlx_get_monitor_size(0, &fdf->width, &fdf->height);
		mlx_terminate(fdf->mlx);
		mlx_set_setting(MLX_HEADLESS, false);
	}
	mlx_set_setting(MLX_FULLSCREEN, fdf->args.fullscreen);
	fdf->mlx = mlx_init(fdf->width, fdf->height, "fdf", !fdf->args.fullscreen);
	fdf->proj = &proj3d;
	fdf->trans = (t_vec3){0, 0, 0};
	fdf->rot = (t_vec2){asinf(tanf(M_PI / 6)), -M_PI_4};
	// TODO default zoom?
	fdf->zoom = 10;
	// TODO default scale?
	fdf->scale = 0.1;
	fdf->res = 0.8;
	fdf->ctrldown = (fdf->mouseheld = false);
	mlx_resize_hook(fdf->mlx, &ft_hook_resize, fdf);
	mlx_key_hook(fdf->mlx, &ft_hook_key, fdf);
	mlx_scroll_hook(fdf->mlx, &ft_hook_scroll, fdf);
	mlx_mouse_hook(fdf->mlx, &ft_hook_mouse, fdf);
	mlx_cursor_hook(fdf->mlx, &ft_hook_cursor, fdf);
	mlx_loop_hook(fdf->mlx, (void *)&render, fdf);
}

int	main(int argc, char *argv[])
{
	t_fdf	fdf;

	if (parse_args(&fdf, argc, argv) < 0)
		return (EXIT_FAILURE);
	ft_printf("loading map: %s\n", argv[1]);
	if (load_map(&fdf) < 0)
		return (EXIT_FAILURE);
	ft_printf("mesh loaded: %p, width:%d, height:%d\n", fdf.mesh, fdf.mesh->width, fdf.mesh->height);
	if (fdf.mesh == NULL)
		return (EXIT_FAILURE);
	ft_printf("starting init\n", fdf.width, fdf.height);
	init_fdf(&fdf);
	ft_printf("init done, width: %d, height: %d\n", fdf.width, fdf.height);
	if (!fdf.mlx)
		return (EXIT_FAILURE);
	fdf.screen = mlx_new_image(fdf.mlx, fdf.width, fdf.height);
	if (!fdf.screen || (mlx_image_to_window(fdf.mlx, fdf.screen, 0, 0) == -1))
		return (EXIT_FAILURE);
	ft_printf("starting render...\n");
	render(&fdf);
	ft_printf("render complete\n");
	mlx_loop(fdf.mlx);
	mlx_terminate(fdf.mlx);
	free_mesh(fdf.mesh, NULL);
	return (EXIT_SUCCESS);
}
