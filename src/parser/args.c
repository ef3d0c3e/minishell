/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

struct s_wordlist
	arg_parse(t_parser *parser, int eat_minus)
{
	const t_token	*tok;
	struct s_wordlist	arg;

	arg.items = NULL;
	arg.nitems = 0;
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
			arg_push(parser, &arg);
		else
			break ;
		++parser->pos;
	}
	return (arg);
}

void
	arg_push(t_parser *parser, struct s_wordlist *arg)
{
	const t_token	*token = &parser->list.tokens[parser->pos];

	arg->items = ft_realloc(arg->items, sizeof(arg->items[0]) * arg->nitems,
		sizeof(arg->items[0]) * (arg->nitems + 1));
	if (token->type == TOK_PARAM || token->type == TOK_PARAM_SIMPLE)
		parse_param(parser, &arg->items[arg->nitems]);
	else if (token->type == TOK_CMD_SUB)
		arg->items[arg->nitems] = (struct s_word){
			.type = ARG_SUBEXPR,
			.flags = token->flags,
			.text = stringbuf_from_range(token->word.str,
				token->word.str + token->word.len),
		};
	else
	{
		arg->items[arg->nitems] = (struct s_word){
			.flags = token->flags,
			.type = ARG_LITERAL,
		};
		stringbuf_init(&arg->items[arg->nitems].text, 64);
		token_wordcontent(&arg->items[arg->nitems].text, token);
	}
	if (arg->nitems)
		arg->items[arg->nitems - 1].next = &arg->items[arg->nitems];
	++arg->nitems;
}

void
	arg_free(struct s_wordlist *arg)
{
	size_t	i;

	i = 0;
	while (i < arg->nitems)
	{
		if (arg->items[i].type == ARG_PARAMETER)
		{
			ast_free(arg->items[i].param.word, 0);
			free(arg->items[i].param.name);
		}
		else
			stringbuf_free(&arg->items[i].text);
		++i;
	}
	free(arg->items);
}

void
	arg_print(size_t depth, const struct s_wordlist *arg)
{
	size_t	i;

	i = 0;
	while (arg->nitems && i++ < depth )
		ft_dprintf(2, " | ");
	i = 0;
	while (i < arg->nitems)
	{
		if (i)
			ft_dprintf(2, " ");
		if (arg->items[i].type == ARG_LITERAL)
			ft_dprintf(2, "'%.*s'",
				(int)arg->items[i].text.len, arg->items[i].text.str);
		else if (arg->items[i].type == ARG_PARAMETER)
			ft_dprintf(2, "${%s} (op=%s)",
				arg->items[i].param.name, arg->items[i].param.op);
		else if (arg->items[i].type == ARG_SUBEXPR)
			ft_dprintf(2, "$(%.*s)",
				(int)arg->items[i].text.len, arg->items[i].text.str);

		++i;
	}
	ft_dprintf(2, "\n");
}
