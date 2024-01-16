/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_loader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 15:49:44 by rvandepu          #+#    #+#             */
/*   Updated: 2024/01/16 09:04:51 by rvandepu         ###   ########.fr       */
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

static int	parse_value(t_btree *node, char **line)
{
	int	color;

	node->value = atoi_base_skip(line, 10);
	if (**line == ',')
		color = ((*line)++, parse_nbr_skip(line));
	else
		color = DEFAULT_COLOR;
	if (color < 0)
		return (-1);
	node->color = color * 0x100 + 0xFF;
	ft_printf("[parse_value] val: %d, local: %#X, color: %#X\n", node->value, color, node->color);
	return (0);
}

static int	parse_line(int count, t_btree *last, t_btree *curr, char *line)
{
	t_btree	*prev;

	prev = NULL;
	while (count--)
	{
		if (last != NULL)
			last->left = curr;
		if (prev != NULL)
			prev->right = curr;
		if (parse_value(curr, &line) < 0)
			return (-1);
		prev = curr;
		if (last != NULL)
			last = last->right;
		curr++;
	}
	return (0);
}

static int	parse_map(t_mesh *mesh, int fd)
{
	char	*line;
	int		n;
	t_btree	*last;
	t_btree	*curr;

	last = NULL;
	curr = NULL;
	line = get_next_line(fd);
	while (line != NULL)
	{
		n = count_vals(line);
		if (!mesh->width && n)
			mesh->width = n;
		else if (mesh->width != n)
			return (free_mesh(mesh, line), -1);
		last = curr;
		curr = ft_calloc(mesh->width, sizeof(t_btree));
		if (curr == NULL)
			return (free_mesh(mesh, line), -2);
		if (mesh->values == NULL)
			mesh->values = curr;
		int ret = parse_line(mesh->width, last, curr, line);
		if (ret < 0)
			return (free_mesh(mesh, line), ft_printf("parse_line ret: %d\n", ret), -3);
		line = (free(line), get_next_line(fd));
	}
	return (0);
}

t_mesh	*load_map(char *path)
{
	t_mesh	*mesh;
	int		fd;

	fd = open(path, O_RDONLY);
	mesh = ft_calloc(1, sizeof(t_mesh));
	if (fd == -1 || mesh == NULL)
		return ((((fd == -1 && (close(fd) || true)) || true)
				&& (mesh == NULL && (free(mesh), true))), NULL);
	int ret = parse_map(mesh, fd);
	if (ret < 0)
		return (close(fd), ft_printf("parse_map ret: %d\n", ret), NULL);
	return (mesh);
}
