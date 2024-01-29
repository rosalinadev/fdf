/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 23:12:02 by rvandepu          #+#    #+#             */
/*   Updated: 2024/01/29 00:44:42 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_hook_resize(int width, int height, void *param)
{
	t_fdf	*fdf;

	fdf = param;
	fdf->width = width;
	fdf->height = height;
	mlx_resize_image(fdf->screen, width, height);
}

void	ft_hook_key(mlx_key_data_t keydata, void *param)
{
	t_fdf	*fdf;

	fdf = param;
	param = (void *)(bool)(keydata.action != MLX_RELEASE);
	if (keydata.key == MLX_KEY_W)
		fdf->flags = ft_bit_set_to(fdf->flags, F_FORWARDS, param);
	else if (keydata.key == MLX_KEY_A)
		fdf->flags = ft_bit_set_to(fdf->flags, F_LEFT, param);
	else if (keydata.key == MLX_KEY_S)
		fdf->flags = ft_bit_set_to(fdf->flags, F_BACKWARDS, param);
	else if (keydata.key == MLX_KEY_D)
		fdf->flags = ft_bit_set_to(fdf->flags, F_RIGHT, param);
	else if (keydata.key == MLX_KEY_Q)
		fdf->flags = ft_bit_set_to(fdf->flags, F_DOWN, param);
	else if (keydata.key == MLX_KEY_E)
		fdf->flags = ft_bit_set_to(fdf->flags, F_UP, param);
	else if (keydata.key == MLX_KEY_LEFT_CONTROL)
		fdf->flags = ft_bit_set_to(fdf->flags, F_MOD, param);
	else if (keydata.key == MLX_KEY_F11)
		fdf->flags = (mlx_close_window(fdf->mlx),
				ft_bit_set_to(fdf->flags, F_FULLSCREEN, param));
	else if (keydata.key == MLX_KEY_T)
		fdf->flags = ft_bit_set_to(fdf->flags, F_TOGGLE_PROJ, param);
	else if (keydata.key == MLX_KEY_ESCAPE)
		mlx_close_window(fdf->mlx);
}

void	ft_hook_scroll(double xdelta, double ydelta, void *param)
{
	t_fdf	*fdf;

	fdf = param;
	(void) xdelta;
	if (ft_bit_check(fdf->flags, F_MOD)
		&& fmax(abs(fdf->mesh->min), abs(fdf->mesh->max)))
		fdf->scale += ((fmin(fdf->mesh->width, fdf->mesh->height) - 1) / 2.0)
			/ (fmax(abs(fdf->mesh->min), abs(fdf->mesh->max))) / 100.0 * ydelta;
	else if (ydelta < 0)
		fdf->zoom /= 1.1;
	else if (ydelta > 0)
		fdf->zoom *= 1.1;
}

void	ft_hook_mouse(mouse_key_t button, action_t action,
						modifier_key_t mods, void *param)
{
	t_fdf	*fdf;

	fdf = param;
	(void) mods;
	if (button == MLX_MOUSE_BUTTON_LEFT)
		fdf->flags = ft_bit_set_to(fdf->flags, F_ROTATE, action != MLX_RELEASE);
}

void	ft_hook_cursor(double xpos, double ypos, void *param)
{
	t_fdf			*fdf;
	static double	lastx = 0;
	static double	lasty = 0;

	fdf = param;
	if (ft_bit_check(fdf->flags, F_ROTATE))
	{
		fdf->rot.y -= (xpos - lastx) / fdf->width * M_PI;
		fdf->rot.x += (ypos - lasty) / fdf->height * M_PI;
	}
	lastx = xpos;
	lasty = ypos;
}
