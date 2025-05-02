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

	arg.words = NULL;
	arg.nwords = 0;
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

	arg->words = ft_realloc(arg->words, sizeof(arg->words[0]) * arg->nwords,
		sizeof(arg->words[0]) * (arg->nwords + 1));
	if (token->type == TOK_PARAM || token->type == TOK_PARAM_SIMPLE)
		parse_param(parser, &arg->words[arg->nwords]);
	else if (token->type == TOK_CMD_SUB)
		arg->words[arg->nwords] = (struct s_word){
			.type = W_SUBEXPR,
			.flags = token->flags,
			.text = stringbuf_from_range(token->word.str,
				token->word.str + token->word.len),
		};
	else
	{
		arg->words[arg->nwords] = (struct s_word){
			.flags = token->flags,
			.type = W_LITERAL,
		};
		stringbuf_init(&arg->words[arg->nwords].text, 64);
		token_wordcontent(&arg->words[arg->nwords].text, token);
	}
	if (arg->nwords)
		arg->words[arg->nwords - 1].next = &arg->words[arg->nwords];
	++arg->nwords;
}

void
	arg_free(struct s_wordlist *arg)
{
	size_t	i;

	i = 0;
	while (i < arg->nwords)
	{
		if (arg->words[i].type == W_PARAMETER)
		{
			ast_free(arg->words[i].param.word, 0);
			free(arg->words[i].param.name);
		}
		else
			stringbuf_free(&arg->words[i].text);
		++i;
	}
	free(arg->words);
}

void
	arg_print(size_t depth, const struct s_wordlist *arg)
{
	size_t	i;

	i = 0;
	while (arg->nwords && i++ < depth )
		ft_dprintf(2, " | ");
	i = 0;
	while (i < arg->nwords)
	{
		if (i)
			ft_dprintf(2, " ");
		if (arg->words[i].type == W_LITERAL)
			ft_dprintf(2, "'%.*s'",
				(int)arg->words[i].text.len, arg->words[i].text.str);
		else if (arg->words[i].type == W_PARAMETER)
			ft_dprintf(2, "${%s} (op=%s)",
				arg->words[i].param.name, arg->words[i].param.op);
		else if (arg->words[i].type == W_SUBEXPR)
			ft_dprintf(2, "$(%.*s)",
				(int)arg->words[i].text.len, arg->words[i].text.str);

		++i;
	}
	ft_dprintf(2, "\n");
}
