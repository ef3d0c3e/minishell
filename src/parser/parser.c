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
#include <shell/shell.h>

t_parser
	parser_init(t_string input, t_token_list list)
{
	t_parser	parser;

	parser = (t_parser){
		.input = input,
		.list = list,
		.errors = xmalloc(sizeof(t_string_buffer) * 16),
		.errors_size = 0,
		.errors_cap = 16,
		.pos = 0,
		.delim_stack.delimiters = NULL,
		.delim_stack.size = 0,
		.delim_stack.capacity = 0,
		.heredoc_count = 0,
	};
	ft_memset(parser.heredocs, 0, sizeof(t_redirection *) * HEREDOC_MAX);
	return (parser);
}

void
	parser_free(t_parser *parser)
{
	size_t	i;

	i = 0;
	while (i < parser->errors_size)
		free(parser->errors[i++]);
	free(parser->errors);
	free(parser->delim_stack.delimiters);
}

/** @brief Formats the error */
static char
	*error_format(t_parser *parser, char *msg, size_t start, size_t end)
{
	const t_token	*tok_start;
	size_t			end_pos;
	size_t			i;
	char			*indicator;
	char			*err;

	if (end >= parser->list.size)
		end_pos = parser->input.len;
	else
		end_pos = parser->list.tokens[end].end;
	if (start >= parser->list.size)
		start = (parser->list.size - 1) * !!parser->list.size;
	tok_start = &parser->list.tokens[start];
	indicator = xmalloc(tok_start->start + 2);
	i = 0;
	while (i < tok_start->start)
		indicator[i++] = '~';
	indicator[i] = '^';
	indicator[i + 1] = '\0';
	ft_asprintf(&err, "[ Parse error ]\n%.*s%s%.*s%s%.*s\n%s %s\n",
		(int)tok_start->start, parser->input.str, "\033[0;31m",
		(int)(end_pos - tok_start->start), parser->input.str + tok_start->start,
		"\033[0m", (int)(parser->input.len - end_pos), parser->input.str
		+ end_pos, indicator, msg);
	return (free(indicator), free(msg), err);
}

void
	parser_error(t_parser *parser, char *msg, size_t start, size_t end)
{
	size_t	new_cap;
	char	*err;

	new_cap = parser->errors_cap + !parser->errors_cap * 16;
	while (new_cap < parser->errors_size + 1)
		new_cap *= 2;
	parser->errors = ft_realloc(parser->errors,
			parser->errors_cap * sizeof(char **),
			new_cap * sizeof(char **));
	parser->errors_cap = new_cap;
	if (parser->list.size)
		parser->errors[parser->errors_size++]
			= error_format(parser, msg, start, end);
	else
	{
		ft_asprintf(&err, "[ Parse error ]\n%s\n", msg);
		parser->errors[parser->errors_size++]
			= err;
	}
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
