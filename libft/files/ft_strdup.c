/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-cama <tde-cama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 14:16:36 by tde-cama          #+#    #+#             */
/*   Updated: 2021/02/14 16:10:29 by tde-cama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *str)
{
	char	*ptr;
	char	*tcstr;
	size_t	size;

	tcstr = (char*)str;
	size = ft_strlen(tcstr) + 1;
	ptr = (char*)malloc(size);
	if (!ptr)
		return (0);
	ft_memcpy(ptr, tcstr, size);
	return (ptr);
}
