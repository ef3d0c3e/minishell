/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atom.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tokenizer/tokenizer.h"
#include <shell/shell.h>

t_atom
	atom_copy(const t_atom *in)
{
	t_atom	out;

	out = *in;
	if (out.type == W_PARAMETER)
	{
		out.param.name = ft_strdup(out.param.name);
		out.param.word = ft_strdup(out.param.word);
	}
	else
		out.text = stringbuf_copy(&out.text);
	return (out);
}

void
	parse_param_atom(t_parser *parser, struct s_atom *arg)
{
	static const char	*ops[] = {":-", "-", ":=", "=", ":?", "?", ":+", "+",
	"##", "#", "%%", "%", ":", NULL};
	t_token *const		token = &parser->list.tokens[parser->pos];
	char				*name;
	char				*err;
	size_t				sep;

	arg->type = W_PARAMETER;
	arg->flags = token->flags;
	arg->param.op = NULL;
	arg->param.word = NULL;
	arg->param.offset = 0;
	arg->param.length = (size_t) - 1;
	arg->param.simple_param = token->type == TOK_PARAM_SIMPLE;
	name = stringbuf_cstr(&token->word);
	if (token->type == TOK_PARAM_SIMPLE)
	{
		arg->param.name = ft_strdup(name);
		return ;
	}
	
	sep = 0;
	if (name[0] == '?' || name[0] == '#' || name[0] == '$' || name[0] == '@'
		|| name[0] == '#')
	{
		sep = 1;
	}
	else
	{
		while ((name[sep] >= '0' && name[sep] <= '9')
			|| (name[sep] >= 'a' && name[sep] <= 'z')
			|| (name[sep] >= 'A' && name[sep] <= 'Z')
			|| (name[sep] == '_'))
			++sep;
	}
	if (sep == token->word.len)
	{
		arg->param.name = ft_strdup(name);
		return ;
	}
	ft_dprintf(2, "TODO(%s)\n\n ", SRC_LOCATION);
	exit(1);
	arg->param.op = str_alternatives((t_string){name + sep,
		ft_strlen(name + sep)}, ops);
	if (!arg->param.op)
	{
		ft_asprintf(&err, "Invalid parameter expansion: `%s`", name);
		parser_error(parser, err, parser->pos, parser->pos + 1);
	}
	// TODO: Call parser recursively on content & set context for errors
}
