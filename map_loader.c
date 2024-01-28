/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_loader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 15:49:44 by rvandepu          #+#    #+#             */
/*   Updated: 2024/01/28 03:26:12 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	free_mesh(t_mesh *mesh, char *line)
{
	t_btree	*t;

	if (mesh != NULL)
	{
		while (mesh->values)
		{
			t = mesh->values->left;
			free(mesh->values);
			mesh->values = t;
		}
		free(mesh);
	}
	if (line != NULL)
		free(line);
}

static int	parse_value(t_fdf *fdf, t_btree *node, char **line)
{
	int	color;
	int	alpha;

	node->value = atoi_base_skip(line, 10);
	if (**line == ',')
		color = ((*line)++, parse_nbr_skip(line));
	else
		color = fdf->args.default_color;
	if (color < 0)
		return (-1);
	if (**line == ',')
		alpha = ((*line)++, parse_nbr_skip(line));
	else
		alpha = fdf->args.default_alpha;
	if (alpha < 0)
		return (-1);
	node->color = color * 0x100 + alpha;
	return (0);
}

static int	parse_line(t_fdf *fdf, int count, t_btree *lines[2], char *line)
{
	t_btree	*prev;
	t_btree	*last;
	t_btree	*curr;

	prev = NULL;
	last = lines[0];
	curr = lines[1];
	while (count--)
	{
		if (last != NULL)
			last->left = curr;
		if (prev != NULL)
			prev->right = curr;
		if (parse_value(fdf, curr, &line) < 0)
			return (-1);
		prev = curr;
		if (last != NULL)
			last = last->right;
		curr++;
	}
	return (0);
}

static int	parse_map(t_fdf *fdf, int fd)
{
	char	*line;
	int		n;
	t_btree	*last;
	t_btree	*curr;

	line = ((last = NULL), (curr = NULL), get_next_line(fd));
	while (line != NULL)
	{
		n = count_vals(line);
		if (n)
		{
			if (n > fdf->mesh->width)
				fdf->mesh->width = n;
			curr = ((last = curr), ft_calloc(n, sizeof(t_btree)));
			if (curr == NULL)
				return (free_mesh(fdf->mesh, line), -1);
			if (fdf->mesh->values == NULL)
				fdf->mesh->values = curr;
			if (parse_line(fdf, n, (t_btree *[2]){last, curr}, line) < 0)
				return (free_mesh(fdf->mesh, line), -1);
			fdf->mesh->height++;
		}
		line = (free(line), get_next_line(fd));
	}
	return (0);
}

int	load_map(t_fdf *fdf)
{
	int		fd;
	int		defaults[2];

	fd = open(fdf->args.map_path, O_RDONLY);
	defaults[0] = ((defaults[1] = -1), -1);
	fdf->mesh = ft_calloc(1, sizeof(t_mesh));
	if (fd == -1 || fdf->mesh == NULL)
	{
		if (fd != -1)
			close(fd);
		if (fdf->mesh != NULL)
			fdf->mesh = (free(fdf->mesh), NULL);
		return (-1);
	}
	if (parse_map(fdf, fd) < 0)
		return (close(fd), -1);
	return (0);
}
