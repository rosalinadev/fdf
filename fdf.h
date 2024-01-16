/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 18:09:07 by rvandepu          #+#    #+#             */
/*   Updated: 2024/01/16 08:18:10 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <stdlib.h>
# include <unistd.h>
// TODO remove stdio
# include <stdio.h>
# include <fcntl.h>
# include <math.h>

# include "MLX42/MLX42.h"
# include "libft.h"

# define DEFAULT_COLOR 0xFFFFFF

typedef struct s_btree
{
	int				value;
	unsigned int	color;
	struct s_btree	*left;
	struct s_btree	*right;
	bool			checked;
}	t_btree;

typedef struct s_mesh
{
	t_btree	*values;
	int		width;
	int		height;
}	t_mesh;

typedef struct s_fdf
{
	mlx_t		*mlx;
	mlx_image_t	*screen;
	t_mesh		*mesh;
}	t_fdf;

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

// map_loader.c
void	free_mesh(t_mesh *mesh, char *line);
t_mesh	*load_map(char *path);

// parse_utils.c
int		count_vals(char *line);
int		atoi_base_skip(char **str, int base);
int		parse_nbr_skip(char **str);

#endif
