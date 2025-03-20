/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"

t_parser
	parser_init(t_token_list list)
{

	return ((t_parser){
		.list = list,
		.errors = xmalloc(sizeof(t_string_buffer) * 16),
		.errors_size = 0,
		.errors_cap = 16,
	});
}

void
	parser_error(t_parser *parser, t_string_buffer msg)
{
	size_t	new_cap;

	dprintf(2, "ERROR: %.*s\n", (int)msg.len, msg.str);
	new_cap = parser->errors_cap + !parser->errors_cap * 16;
	while (new_cap < parser->errors_size + 1)
		new_cap *= 2;
	parser->errors = ft_realloc(parser->errors,
			parser->errors_cap * sizeof(t_string_buffer),
			new_cap * sizeof(t_string_buffer));
	parser->errors_cap = new_cap;
	parser->errors[parser->errors_size++] = msg;
}

void
	parser_free(t_parser *parser)
{
	free(parser->errors);
}
