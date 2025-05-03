/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delimiters.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "parser/parser.h"
#include "tokenizer/tokenizer.h"
#include <shell/shell.h>

void
	parser_delimiter_push(t_parser *parser, const char *delim)
{
	t_delimiter_stack *const	stack = &parser->delim_stack;

	if (stack->size >= stack->capacity)
	{
		stack->capacity = 4 * !stack->capacity
			+ stack->capacity * 2;
		stack->delimiters = ft_realloc(
			stack->delimiters,
			sizeof(char *) * stack->size,
			sizeof(char *) * (stack->capacity)
		);
	}
	stack->delimiters[stack->size++] = delim;
}

int
	parser_delimiter_pop(t_parser *parser, const char *delim)
{
	int							status;
	t_delimiter_stack *const	stack = &parser->delim_stack;

	if (!stack->size)
		return (0);
	status = !ft_strcmp(stack->delimiters[--stack->size], delim);
	return (status);
}

int
	expects_delimiter(t_parser *parser, const char *delim)
{
	char	*err;

	if (parser->pos < parser->list.size && !accept(parser, 0, delim))
	{
		ft_asprintf(&err, "Expected `%s` token", delim);
		parser_error(parser, err, parser->pos, parser->pos + 1);
		return (0);
	}
	else if (parser->pos >= parser->list.size
		|| !parser_delimiter_pop(parser, delim))
	{
		ft_asprintf(&err, "Expected `%s` token", delim);
		parser_error(parser, err, parser->pos, parser->pos + 1);
		return (0);
	}
	++parser->pos;
	return (1);
}
