/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 16:57:19 by rvandepu          #+#    #+#             */
/*   Updated: 2024/02/05 19:30:31 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

#define WIDTH 1024
#define HEIGHT 768
#define START_FULLSCREEN false

static int	parse_args(t_fdf *fdf, int argc, char *argv[])
{
	if (argc < 2)
		return (ft_printf("Usage: %s <file> [rgb [alpha]]\n\n" \
							"Keybindings:\n" \
							"	WASD & QE: move\n" \
							"	Left click: rotate\n" \
							"	Mousewheel: zoom\n" \
							"	Ctrl-mousewheel: vertical scale\n" \
							"	F11: toggle fullscreen\n" \
							"	T: toggle 3D/2D\n" \
							"	Escape: exit\n", \
							argv[0]), \
						-1);
	fdf->args.map_path = argv[1];
	if (argc > 2)
		fdf->args.default_color = parse_nbr_skip(&argv[2]);
	else
		fdf->args.default_color = DEFAULT_COLOR;
	if (argc > 3)
		fdf->args.default_alpha = parse_nbr_skip(&argv[3]);
	else
		fdf->args.default_alpha = DEFAULT_ALPHA;
	if (fdf->args.default_color < 0 || fdf->args.default_alpha < 0)
		return (-1);
	return (0);
}

static void	get_win_size(t_fdf *fdf, bool fullscreen)
{
	fdf->width = WIDTH;
	fdf->height = HEIGHT;
	if (fullscreen)
	{
		mlx_set_setting(MLX_FULLSCREEN, false);
		mlx_set_setting(MLX_HEADLESS, true);
		fdf->mlx = mlx_init(fdf->width, fdf->height, "", false);
		if (!fdf->mlx)
			return (free_mesh(fdf->mesh), exit(EXIT_FAILURE));
		mlx_get_monitor_size(0, &fdf->width, &fdf->height);
		mlx_terminate(fdf->mlx);
		mlx_set_setting(MLX_HEADLESS, false);
	}
}

static bool	init_fdf(t_fdf *fdf)
{
	get_win_size(fdf, START_FULLSCREEN);
	fdf->proj = &proj3d;
	fdf->trans = (t_vec3){0, 0, 0};
	fdf->rot = (t_vec2){asinf(tanf(M_PI / 6)), -M_PI_4};
	fdf->zoom = fmin((float)fdf->width / (float)fdf->mesh->width,
			(float)fdf->height / (float)fdf->mesh->height) * 0.9;
	if ((fmax(abs(fdf->mesh->min), abs(fdf->mesh->max))))
		fdf->scale = ((fmin(fdf->mesh->width, fdf->mesh->height) - 1) / 4.0)
			/ (fmax(abs(fdf->mesh->min), abs(fdf->mesh->max)));
	else
		fdf->scale = 0;
	fdf->res = 0.8;
	if (fdf->mesh->width && fdf->mesh->height)
		fdf->maxsteps = 250000000.0 / (fdf->mesh->width * fdf->mesh->height);
	fdf->flags = 0;
	return (alloc_mem(fdf, 0));
}

static void	init_win(t_fdf *fdf)
{
	static bool	fullscreen = START_FULLSCREEN;

	if (ft_bit_check(fdf->flags, F_FULLSCREEN))
		fullscreen = !fullscreen;
	get_win_size(fdf, fullscreen);
	mlx_set_setting(MLX_FULLSCREEN, fullscreen);
	fdf->mlx = mlx_init(fdf->width, fdf->height, "fdf", !fullscreen);
	if (!fdf->mlx)
		return (free_mesh(fdf->mesh), exit(EXIT_FAILURE));
	fdf->flags = ft_bit_clear(fdf->flags, F_FULLSCREEN);
	mlx_resize_hook(fdf->mlx, &ft_hook_resize, fdf);
	mlx_key_hook(fdf->mlx, &ft_hook_key, fdf);
	mlx_scroll_hook(fdf->mlx, &ft_hook_scroll, fdf);
	mlx_mouse_hook(fdf->mlx, &ft_hook_mouse, fdf);
	mlx_cursor_hook(fdf->mlx, &ft_hook_cursor, fdf);
	mlx_loop_hook(fdf->mlx, &ft_hook_loop, fdf);
}

int	main(int argc, char *argv[])
{
	t_fdf	fdf;

	if (parse_args(&fdf, argc, argv) < 0)
		return (EXIT_FAILURE);
	if (load_map(&fdf) < 0)
		return (EXIT_FAILURE);
	if (fdf.mesh == NULL)
		return (EXIT_FAILURE);
	if (!init_fdf(&fdf))
		return (free_mesh(fdf.mesh), EXIT_FAILURE);
	while (true)
	{
		init_win(&fdf);
		if (!fdf.mlx)
			return (free_mesh(fdf.mesh), EXIT_FAILURE);
		fdf.screen = mlx_new_image(fdf.mlx, fdf.width, fdf.height);
		if (!fdf.screen || (mlx_image_to_window(fdf.mlx, fdf.screen, 0, 0) < 0))
			return (free_mesh(fdf.mesh), mlx_terminate(fdf.mlx), EXIT_FAILURE);
		mlx_loop(fdf.mlx);
		mlx_terminate(fdf.mlx);
		if (!ft_bit_check(fdf.flags, F_FULLSCREEN))
			break ;
	}
	(free_mem(&fdf), free_mesh(fdf.mesh));
	return (EXIT_SUCCESS);
}
