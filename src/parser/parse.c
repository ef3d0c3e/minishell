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
#include <shell/shell.h>

/*
<script>                ::= <list_of_commands>




<until_clause>            ::= 'until' <list_of_commands> 'do' <list_of_commands> 'done'

<case_clause>            ::= 'case' WORD 'in'
                        ( PATTERN ')' <list_of_commands> ';;' )+
						'esac'
*/

t_ast_node
	*parse(t_parser *parser)
{
	return (parse_cmdlist(parser));
}
