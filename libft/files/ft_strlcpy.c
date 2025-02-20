/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-cama <tde-cama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 18:37:38 by tde-cama          #+#    #+#             */
/*   Updated: 2021/02/14 16:10:29 by tde-cama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	char *tcsrc;

	tcsrc = (char*)src;
	if (!src)
		return (0);
	if (size)
	{
		while (--size && *tcsrc)
		{
			*dest++ = *tcsrc++;
		}
		*dest = '\0';
	}
	return (ft_strlen(src));
}
