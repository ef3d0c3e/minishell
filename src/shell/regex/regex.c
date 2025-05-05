/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	regex_error(t_reg_parser *parser, const char *msg, size_t pos)
{
	if (parser->regex.errors_size >= parser->regex.errors_capacity)
	{
		parser->regex.errors_capacity = parser->regex.errors_capacity * 2
			+ !parser->regex.errors_capacity;
		parser->regex.errors = ft_realloc(parser->regex.errors,
				sizeof(char *) * parser->regex.errors_size,
				sizeof(char *) * parser->regex.errors_capacity);
	}
	(void)pos;
	// TODO Format using `pos`
	parser->regex.errors[parser->regex.errors_size++] = ft_strdup(msg);
}

int
	regex_error_flush(t_reg_parser *parser)
{
	size_t	i;

	i = 0;
	while (i < parser->regex.errors_size)
		ft_dprintf(2, "%s\n", parser->regex.errors[i++]);
	parser->regex.errors_size = 0;
	return (i == 0);
}
