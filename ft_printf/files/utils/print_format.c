/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_format.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tde-cama <tde-cama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/08 12:28:49 by tde-cama          #+#    #+#             */
/*   Updated: 2021/03/11 18:07:12 by tde-cama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

char		*print_format(va_list args, char *format, char flag)
{
	if (ft_isset(*format, "cspdiuxX%f"))
		return (specifier_layer(args, format));
	else if (*format == '.')
		return (precision_layer(args, ++format));
	else if (ft_isset(*format, "-0"))
		return (flags_layer(args, format));
	else if (*format == '*' || ft_isdigit(*format))
		return (width_layer(args, format, flag));
	else
		return (ft_make_str(3, '\0', 'c', '\0'));
}
