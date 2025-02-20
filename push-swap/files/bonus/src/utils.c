/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-cama <tde-cama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/24 15:05:41 by tde-cama          #+#    #+#             */
/*   Updated: 2021/06/29 14:33:22 by tde-cama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker.h"

int	ft_isdigit(char *c)
{
	while (*c)
		if (!(*c >= '0' && *c++ <= '9'))
			return (0);
	return (1);
}

void	free_lst(t_list **lst)
{
	t_list	*next;

	if (!*lst)
		return ;
	while (*lst)
	{
		next = (*lst)->next;
		free(*lst);
		*lst = next;
	}
	*lst = NULL;
}

int	ft_strcmp(char *str1, char *str2)
{
	while (*str1 && *str2)
	{
		if (*str1++ != *str2++)
			return (*(--str1) - *(--str2));
	}
	if (*str1 == *str2)
		return (0);
	return (*str1 - *str2);
}

static int	convert(const char *str, char is_neg, size_t index)
{
	long	ret;
	int		i;

	ret = 0;
	while (str[index] >= '0' && str[index] <= '9')
	{
		i = (int)(str[index] - '0');
		if (is_neg == '-' && i)
			i *= -1;
		ret = i + (ret * 10);
		index++;
		if (ret > INT_MAX || ret < INT_MIN)
		{
			if (is_neg == '-')
				return (0);
			return (-1);
		}
	}
	return (ret);
}

int	ft_atoi(const char *str)
{
	int			ret;
	char		is_neg;
	size_t		i;

	ret = 0;
	is_neg = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		is_neg = str[i];
		i++;
	}
	ret = convert(str, is_neg, i);
	return (ret);
}
