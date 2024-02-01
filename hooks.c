/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 23:12:02 by rvandepu          #+#    #+#             */
/*   Updated: 2024/02/01 06:18:11 by rvandepu         ###   ########.fr       */
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

static const unsigned char	g_is_pressed[350] = {\
	[MLX_KEY_W] = F_FORWARDS, \
	[MLX_KEY_A] = F_LEFT, \
	[MLX_KEY_S] = F_BACKWARDS, \
	[MLX_KEY_D] = F_RIGHT, \
	[MLX_KEY_Q] = F_DOWN, \
	[MLX_KEY_E] = F_UP, \
	[MLX_KEY_LEFT_CONTROL] = F_MOD, \
};

static const unsigned char	g_was_pressed[350] = {\
	[MLX_KEY_F11] = F_FULLSCREEN, \
	[MLX_KEY_ESCAPE] = F_QUIT, \
	[MLX_KEY_T] = F_TOGGLE_PROJ, \
};

void	ft_hook_key(mlx_key_data_t keydata, void *param)
{
	t_fdf	*fdf;
	bool	b;

	fdf = param;
	if (keydata.key >= 350)
		return ;
	b = keydata.action != MLX_RELEASE;
	if (g_is_pressed[keydata.key])
		fdf->flags = ft_bit_set_to(fdf->flags, g_is_pressed[keydata.key], b);
	else if (b && g_was_pressed[keydata.key])
		fdf->flags = ft_bit_set(fdf->flags, g_was_pressed[keydata.key]);
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
