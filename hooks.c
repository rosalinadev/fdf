/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 23:12:02 by rvandepu          #+#    #+#             */
/*   Updated: 2024/01/28 03:58:37 by rvandepu         ###   ########.fr       */
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

// TODO other keys, ESC, translation, res, help, fullscreen?
void	ft_hook_key(mlx_key_data_t keydata, void *param)
{
	t_fdf	*fdf;

	fdf = param;
	if (keydata.key == MLX_KEY_LEFT_CONTROL)
		fdf->ctrldown = keydata.action != MLX_RELEASE;
	ft_printf("ctrl:%d\n", fdf->ctrldown);
}

void	ft_hook_scroll(double xdelta, double ydelta, void *param)
{
	t_fdf	*fdf;

	fdf = param;
	(void) xdelta;
	if (fdf->ctrldown)
		fdf->scale += ydelta / 100.0;
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
	{
		if (action == MLX_PRESS)
			fdf->mouseheld = true;
		else if (action == MLX_RELEASE)
			fdf->mouseheld = false;
	}
}

void	ft_hook_cursor(double xpos, double ypos, void *param)
{
	t_fdf			*fdf;
	static double	lastx = 0;
	static double	lasty = 0;

	fdf = param;
	if (fdf->mouseheld)
	{
		fdf->rot.y -= (xpos - lastx) / fdf->width * M_PI;
		fdf->rot.x += (ypos - lasty) / fdf->height * M_PI;
	}
	lastx = xpos;
	lasty = ypos;
}
