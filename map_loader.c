/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_loader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 15:49:44 by rvandepu          #+#    #+#             */
/*   Updated: 2024/01/20 12:12:24 by rvandepu         ###   ########.fr       */
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

static int	parse_value(t_btree *node, int defaults[2], char **line)
{
	int	color;
	int	alpha;

	if (defaults[0] < 0)
		defaults[0] = DEFAULT_COLOR;
	if (defaults[1] < 0)
		defaults[1] = DEFAULT_ALPHA;
	node->value = atoi_base_skip(line, 10);
	if (**line == ',')
		color = ((*line)++, parse_nbr_skip(line));
	else
		color = defaults[0];
	if (color < 0)
		return (-1);
	if (**line == ',')
		alpha = ((*line)++, parse_nbr_skip(line));
	else
		alpha = defaults[1];
	if (alpha < 0)
		return (-1);
	node->color = color * 0x100 + alpha;
	ft_printf("[parse_value] val: %d, col: %#X, alp: %#X, res: %#X\n",
		node->value, color, alpha, node->color);
	return (0);
}

// lines[0]: last
// lines[1]: curr
static int	parse_line(int count, t_btree *lines[2], int defs[2], char *line)
{
	t_btree	*prev;

	prev = NULL;
	while (count--)
	{
		if (lines[0] != NULL)
			lines[0]->left = lines[1];
		if (prev != NULL)
			prev->right = lines[1];
		if (parse_value(lines[1], defs, &line) < 0)
			return (-1);
		prev = lines[1];
		if (lines[0] != NULL)
			lines[0] = lines[0]->right;
		lines[1]++;
	}
	return (0);
}

static int	parse_map(t_mesh *mesh, int defs[2], int fd)
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
			if (n > mesh->width)
				mesh->width = n;
			curr = ((last = curr), ft_calloc(n, sizeof(t_btree)));
			if (curr == NULL)
				return (free_mesh(mesh, line), -1);
			if (mesh->values == NULL)
				mesh->values = curr;
			if (parse_line(n, (t_btree *[2]){last, curr}, defs, line) < 0)
				return (free_mesh(mesh, line), -1);
			mesh->height++;
		}
		line = (free(line), get_next_line(fd));
	}
	return (0);
}

t_mesh	*load_map(int argc, char *argv[])
{
	t_mesh	*mesh;
	int		fd;
	int		defaults[2];

	if (!argc)
		return (NULL);
	fd = open(*argv, O_RDONLY);
	defaults[0] = ((defaults[1] = -1), -1);
	if (argc >= 2)
		defaults[0] = parse_nbr_skip(argv + 1);
	if (argc >= 3)
		defaults[1] = parse_nbr_skip(argv + 2);
	mesh = ft_calloc(1, sizeof(t_mesh));
	if (fd == -1 || mesh == NULL)
		return ((((fd != -1 && (close(fd) || true)) || true)
				&& (mesh != NULL && (free(mesh), true))), NULL);
	if (parse_map(mesh, defaults, fd) < 0)
		return (close(fd), NULL);
	return (mesh);
}
