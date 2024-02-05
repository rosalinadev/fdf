/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 18:09:07 by rvandepu          #+#    #+#             */
/*   Updated: 2024/02/05 19:30:16 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <math.h>

# include "MLX42/MLX42.h"
# include "libft.h"
# include "ft_bitwise.h"

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

typedef struct s_coords
{
	int	x;
	int	y;
}	t_coords;

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
	int		min;
	int		max;
}	t_mesh;

typedef struct s_args
{
	char	*map_path;
	bool	fullscreen;
	int		default_color;
	int		default_alpha;
}	t_args;

typedef unsigned int	t_flags;
typedef enum e_flag
{
	F_FORWARDS = 1,
	F_BACKWARDS,
	F_LEFT,
	F_RIGHT,
	F_UP,
	F_DOWN,
	F_FULLSCREEN,
	F_QUIT,
	F_MOD,
	F_ROTATE,
	F_TOGGLE_PROJ,
}	t_flag;

typedef struct s_fdf
{
	mlx_t		*mlx;
	mlx_image_t	*screen;
	t_mesh		*mesh;
	t_args		args;
	int			width;
	int			height;
	t_point		(*proj)(struct s_fdf *, t_coords, t_btree *);
	t_vec3		trans;
	t_vec2		rot;
	float		zoom;
	float		scale;
	float		res;
	int			maxsteps;
	t_flags		flags;
	t_point		**mem;
}	t_fdf;

// map_loader.c
void			free_mesh(t_mesh *mesh);
int				load_map(t_fdf *fdf);

// parse_utils.c
int				count_vals(char *line);
int				atoi_base_skip(char **str, int base);
int				parse_nbr_skip(char **str);

// hooks.c
void			ft_hook_resize(int width, int height, void *param);
void			ft_hook_key(mlx_key_data_t keydata, void *param);
void			ft_hook_scroll(double xdelta, double ydelta, void *param);
void			ft_hook_mouse(mouse_key_t button, action_t action,
					modifier_key_t mods, void *param);
void			ft_hook_cursor(double xpos, double ypos, void *param);

// draw_utils.c
t_col			convert_to_col(unsigned int color);
unsigned int	convert_from_col(t_col color);
void			draw_line(t_fdf *fdf, t_point a, t_point *b);

// projection.c
t_point			proj2d(t_fdf *fdf, t_coords c, t_btree *node);
t_point			proj3d(t_fdf *fdf, t_coords c, t_btree *node);

// loop.c
bool			alloc_mem(t_fdf *fdf, int depth);
void			free_mem(t_fdf *fdf);
void			ft_hook_loop(void *param);

#endif
