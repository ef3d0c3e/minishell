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

/** @brief Adds a redirection to command */
static inline void
	add_redir(
			struct s_node_cmd *cmd,
			t_token token,
			int fd_from,
			t_string_buffer file_to)
{
	size_t	new_cap;

	new_cap = cmd->redirs_capacity + !cmd->redirs_capacity * 16;
	while (new_cap < cmd->redirs_size + 1)
		new_cap *= 2;
	cmd->redirs = ft_realloc(cmd->redirs,
			cmd->redirs_capacity * sizeof(t_redir_data),
			new_cap * sizeof(t_redir_data));
	cmd->redirs_capacity = new_cap;
	cmd->redirs[cmd->redirs_size++] = (t_redir_data){
		.token = token,
		.from = fd_from,
		.to = file_to,
	};
}

/*
 [N]>[|]WORD
 [N]>>WORD
 [N]>&WORD

 [N]<WORD
 [N]<&WORD

 [N]<&DIGIT-
 [N]>&DIGIT-

 &>WORD
 >&WORD
 >WORD 2>&1
 &>>WORD

 [N]<<[-]WORD
		HERE-DOCUMENT
 [N]<<< WORD
*/

size_t	parse_redir(
	t_parser *parser,
	size_t start,
	size_t end,
	struct s_node_cmd *cmd)
{
	t_string_buffer	word;
	size_t			skipped;

	// TODO: When clone, try to parse word as digit first, otherwise parse as file
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
	if (parser->list.tokens[start].type == TOK_REDIR)
	{
		skipped = parse_word(parser, start + 1, end, &word);
		if (!skipped)
		{
			parser_error(parser, stringbuf_from("Unexpected EOF after redirection token"));
			return (1);
		}
		return (skipped + 1);
	}
	return (0);
}
