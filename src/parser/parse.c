/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/args.h"
#include "parser/parser.h"
#include "util/util.h"
#include <shell/shell.h>

/* TODO:
<case_clause>            ::= 'case' WORD 'in'
                        ( PATTERN ')' <list_of_commands> ';;' )+
						'esac'
*/

t_ast_node
	*parse(t_parser *parser)
{
	t_ast_node	*list;

	list = parse_cmdlist(parser);
	if (parser->pos != parser->list.size)
		parser_error(parser, ft_strdup("Leftover tokens"), parser->pos,
				parser->list.size);
	return (list);
}
