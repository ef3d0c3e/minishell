/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"
#include "syntax/tokenizer.h"
#include "util/util.h"

size_t	parse_redir(
	t_parser *parser,
	size_t start,
	size_t end,
	struct s_node_cmd *cmd)
{
	t_string_buffer	word;
	size_t			skipped;

	if (parser->list.tokens[start].type == TOK_DIGIT && start + 1 < end)
	{
		if (parser->list.tokens[start + 1].type != TOK_REDIR)
			return (0);
		skipped = parse_word(parser, start + 2, end, &word);
		if (!skipped)
		{
			parser_error(parser, stringbuf_from("Unexpected EOF after redirection token"));
			return (2);
		}
		return (skipped + 2);
	}
	else if (parser->list.tokens[start].type == TOK_REDIR)
	{
		skipped = parse_word(parser, start + 2, end, &word);
		if (!skipped)
		{
			parser_error(parser, stringbuf_from("Unexpected EOF after redirection token"));
			return (1);
		}
		return (skipped + 1);
	}
	return (0);
	/*
     [N]<WORD

     [N]>[|]WORD

     [N]>>WORD

	 --
     &>WORD
and
     >&WORD
Of the two forms, the first is preferred.  This is semantically
equivalent to
     >WORD 2>&1
	
	 --
     &>>WORD
This is semantically equivalent to
     >>WORD 2>&1

     [N]<<[-]WORD
             HERE-DOCUMENT

	 --
     [N]<<< WORD
	 --

     [N]<&WORD
is used to duplicate input file descriptors.  If WORD expands to one or
more digits, the file descriptor denoted by N is made to be a copy of
that file descriptor.  If the digits in WORD do not specify a file
descriptor open for input, a redirection error occurs.  If WORD
evaluates to '-', file descriptor N is closed.  If N is not specified,
the standard input (file descriptor 0) is used.

   The operator
     [N]>&WORD
	 */
	return 0;
}
