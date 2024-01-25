/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 18:09:07 by rvandepu          #+#    #+#             */
/*   Updated: 2024/01/22 12:17:41 by rvandepu         ###   ########.fr       */
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
# define DEFAULT_ALPHA 0xFF
# define SCALE 10

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

typedef struct s_col
{
	float	r;
	float	g;
	float	b;
	float	a;
}	t_col;

typedef struct s_point
{
	t_vec2	pos;
	t_col	col;
}	t_point;

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

typedef struct s_args
{
	char	*map_path;
	bool	fullscreen;
	int		default_color;
	int		default_alpha;
}	t_args;

typedef struct s_fdf
{
	mlx_t		*mlx;
	mlx_image_t	*screen;
	t_mesh		*mesh;
	t_args		args;
	int			width;
	int			height;
	t_vec3		trans;
	float		zoom;
	float		scale;
	float		res;
}	t_fdf;

// draw_utils.c
t_col			convert_to_col(unsigned int color);
unsigned int	convert_from_col(t_col color);
void			draw_line(t_fdf *fdf, t_point a, t_point *b);

// map_loader.c
void			free_mesh(t_mesh *mesh, char *line);
void			load_map(t_fdf *fdf);

// parse_utils.c
int				count_vals(char *line);
int				atoi_base_skip(char **str, int base);
int				parse_nbr_skip(char **str);

#endif
