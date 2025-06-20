/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_util.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

t_word
	word_from_string(const char *str, int flags)
{
	t_word	word;

	word.natoms = 1;
	word.atoms = xmalloc(sizeof(t_atom) * word.natoms);
	word.atoms[0] = (t_atom){
		.type = W_LITERAL,
		.flags = flags,
		.next = NULL,
		.text = stringbuf_from(str),
	};
	return (word);
}

t_word
	word_from_int(int value, int flags)
{
	t_word			word;
	t_string_buffer	buf;

	stringbuf_init(&buf, 24);
	stringbuf_append_i(&buf, value);
	word.natoms = 1;
	word.atoms = xmalloc(sizeof(t_atom) * word.natoms);
	word.atoms[0] = (t_atom){
		.type = W_LITERAL,
		.flags = flags,
		.next = NULL,
		.text = buf,
	};
	return (word);
}

t_word
	parse_word(t_parser *parser, int eat_minus)
{
	const t_token	*tok;
	t_word			arg;

	arg.atoms = NULL;
	arg.natoms = 0;
	if (parser->pos < parser->list.size
		&& parser->list.tokens[parser->pos].type == TOK_SPACE)
		++parser->pos;
	while (parser->pos < parser->list.size)
	{
		tok = &parser->list.tokens[parser->pos];
		if ((tok->type == TOK_PARAM || tok->type == TOK_PARAM_SIMPLE
				|| tok->type == TOK_CMD_SUB
				|| accept_word(parser, 0))
			&& (!eat_minus || tok->type != TOK_MINUS))
			word_push(parser, &arg);
		else
			break ;
		++parser->pos;
	}
	return (arg);
}
