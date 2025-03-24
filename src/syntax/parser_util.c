/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "syntax/tokenizer.h"

size_t
	parser_next_operator(
		t_parser *parser,
		size_t start,
		size_t end
			)
{
	const t_token	*tok;
	size_t			i;

	i = start;
	while (i < end)
	{
		tok = &parser->list.tokens[i];
		if (tok->type == TOK_PIPELINE || tok->type == TOK_SEQUENCE)
			return (i);
		++i;
	}
	return ((size_t)-1);
}
