/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 18:09:07 by rvandepu          #+#    #+#             */
/*   Updated: 2024/01/15 04:58:53 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <stdlib.h>
// TODO remove stdio
# include <stdio.h>
# include <fcntl.h>
# include <math.h>

# include "MLX42/MLX42.h"
# include "libft.h"

typedef struct s_fdf
{
	mlx_t		*mlx;
	mlx_image_t	*screen;
	t_mesh		*mesh;
}	t_fdf;

typedef struct s_mesh
{
	int				**values;
	unsigned int	width;
	unsigned int	height;
}	t_mesh;

typedef struct s_vec2
{
	float	x;
	float	y;
}	t_vec2;

typedef struct s_vec3
{
	float	x;
	float	y;
	float	z;
}	t_vec3;

// draw_utils.c
void	draw_line(t_fdf *fdf, t_vec2 a, t_vec2 b);

#endif
