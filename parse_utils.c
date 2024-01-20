/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:46:29 by rvandepu          #+#    #+#             */
/*   Updated: 2024/01/20 12:17:16 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	count_vals(char *line)
{
	int		n;
	bool	d;

	n = 0;
	d = true;
	while (*line)
	{
		if (d)
		{
			if (*line != ' ' && (*line < '\t' || '\r' < *line))
				d = (n++, false);
		}
		else if (*line == ' ' || ('\t' <= *line && *line <= '\r'))
			d = true;
		line++;
	}
	return (n);
}

int	atoi_base_skip(char **str, int base)
{
	long	n;
	int		sign;

	n = 0;
	sign = 1;
	while (**str == ' ' || ('\t' <= **str && **str <= '\r'))
		(*str)++;
	if (**str == '-')
		sign = ((*str)++, -sign);
	while (('0' <= **str && **str <= '9')
		|| ('A' <= **str && **str <= 'F')
		|| ('a' <= **str && **str <= 'f'))
	{
		n *= base;
		if ('0' <= **str && **str <= '9')
			n += **str - '0';
		else if ('A' <= **str && **str <= 'F')
			n += **str - 'A' + 10;
		else if ('a' <= **str && **str <= 'f')
			n += **str - 'a' + 10;
		(*str)++;
	}
	return (n * sign);
}

int	parse_nbr_skip(char **str)
{
	while (**str == ' ' || ('\t' <= **str && **str <= '\r'))
		(*str)++;
	if (('1' <= **str && **str <= '9') || **str == '-')
		return (atoi_base_skip(str, 10));
	if (**str != '0')
		return (-1);
	(*str)++;
	if (**str == 'b' || **str == 'B')
		return ((*str)++, atoi_base_skip(str, 2));
	else if ('1' <= **str && **str <= '9')
		return (atoi_base_skip(str, 8));
	else if (**str == 'x' || **str == 'X')
		return ((*str)++, atoi_base_skip(str, 16));
	else
		return (0);
}
