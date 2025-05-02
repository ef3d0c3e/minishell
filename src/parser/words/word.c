/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/words/words.h"
#include "util/util.h"
#include <shell/shell.h>

void
	word_push(t_parser *parser, t_word *arg)
{
	const t_token	*token = &parser->list.tokens[parser->pos];

	arg->atoms = ft_realloc(arg->atoms, sizeof(arg->atoms[0]) * arg->natoms,
		sizeof(arg->atoms[0]) * (arg->natoms + 1));
	if (token->type == TOK_PARAM || token->type == TOK_PARAM_SIMPLE)
		parse_param_aton(parser, &arg->atoms[arg->natoms]);
	else if (token->type == TOK_CMD_SUB)
		arg->atoms[arg->natoms] = (t_atom){
			.type = W_SUBEXPR,
			.flags = token->flags,
			.text = stringbuf_from_range(token->word.str,
				token->word.str + token->word.len),
		};
	else
	{
		arg->atoms[arg->natoms] = (t_atom){
			.flags = token->flags,
			.type = W_LITERAL,
		};
		stringbuf_init(&arg->atoms[arg->natoms].text, 64);
		token_wordcontent(&arg->atoms[arg->natoms].text, token);
	}
	if (arg->natoms)
		arg->atoms[arg->natoms - 1].next = &arg->atoms[arg->natoms];
	++arg->natoms;
}

void
	word_free(t_word *arg)
{
	size_t	i;

	i = 0;
	while (i < arg->natoms)
	{
		if (arg->atoms[i].type == W_PARAMETER)
		{
			ast_free(arg->atoms[i].param.word, 0);
			free(arg->atoms[i].param.name);
		}
		else
			stringbuf_free(&arg->atoms[i].text);
		++i;
	}
	free(arg->atoms);
}

void
	word_print(size_t depth, const t_word *arg)
{
	size_t	i;

	i = 0;
	while (arg->natoms && i++ < depth )
		ft_dprintf(2, " | ");
	i = 0;
	while (i < arg->natoms)
	{
		if (i)
			ft_dprintf(2, " ");
		if (arg->atoms[i].type == W_LITERAL)
			ft_dprintf(2, "'%.*s'",
				(int)arg->atoms[i].text.len, arg->atoms[i].text.str);
		else if (arg->atoms[i].type == W_PARAMETER)
			ft_dprintf(2, "${%s} (op=%s)",
				arg->atoms[i].param.name, arg->atoms[i].param.op);
		else if (arg->atoms[i].type == W_SUBEXPR)
			ft_dprintf(2, "$(%.*s)",
				(int)arg->atoms[i].text.len, arg->atoms[i].text.str);

		++i;
	}
	ft_dprintf(2, "\n");
}

t_word
	word_copy(const t_word *in)
{
	size_t	i;
	t_word	out;

	out.natoms = in->natoms;
	out.atoms = xmalloc(sizeof(t_atom) * out.natoms);
	i = 0;
	while (i < out.natoms)
	{
		out.atoms[i] = atom_copy(&in->atoms[i]);
		++i;
	}
	return (out);
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
