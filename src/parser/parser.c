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
#include "ft_printf.h"
#include "util/util.h"
#include <parser/parser.h>

t_parser
	parser_init(t_string input, t_token_list list)
{
	return ((t_parser){
		.input = input,
		.list = list,
		.errors = xmalloc(sizeof(t_string_buffer) * 16),
		.errors_size = 0,
		.errors_cap = 16,
	});
}

void
	parser_free(t_parser *parser)
{
	size_t	i;

	i = 0;
	while (i < parser->errors_size)
		free(parser->errors[i++]);
	free(parser->errors);
}

/** @brief Formats the error */
static char
	*error_format(t_parser *parser, char *msg, size_t start, size_t end)
{
	const t_token	*tok_start = &parser->list.tokens[start];
	const t_token	*tok_end = &parser->list.tokens[end];
	size_t			i;
	char			*indicator;
	char			*err;

	indicator = xmalloc(tok_end->end + 1);
	i = 0;
	while (i + 1 < tok_end->end)
		indicator[i++] = '~';
	indicator[i] = '^';
	indicator[i + 1] = '\0';
	ft_asprintf(&err, "[ Parse error ]\n%.*s%s%.*s%s%.*s\n%s %s\n",
		(int)tok_start->start, parser->input.str,
		"\033[0;31m",
		(int)(tok_end->end - tok_start->start),
		parser->input.str + tok_start->start,
		"\033[0m",
		(int)(parser->input.len - tok_end->end),
		parser->input.str + tok_end->end,
		indicator,
		msg);
	free(indicator);
	free(msg);
	return (err);
}

void
	parser_error(t_parser *parser, char *msg, size_t start, size_t end)
{
	size_t	new_cap;

	// FIXME
	//dprintf(2, "ERROR: %.*s\n", (int)msg.len, msg.str);
	new_cap = parser->errors_cap + !parser->errors_cap * 16;
	while (new_cap < parser->errors_size + 1)
		new_cap *= 2;
	parser->errors = ft_realloc(parser->errors,
			parser->errors_cap * sizeof(char **),
			new_cap * sizeof(char **));
	parser->errors_cap = new_cap;
	parser->errors[parser->errors_size++]
		= error_format(parser, msg, start, end);
}

int
	parser_error_flush(t_parser *parser)
{
	int		has_error;
	size_t	i;

	has_error = 0;
	i = 0;
	while (i < parser->errors_size)
	{
		has_error = 1;
		ft_dprintf(2, "%s", parser->errors[i]);
		free(parser->errors[i]);
		++i;
	}
	parser->errors_size = 0;
	return (!has_error);
}
