/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_param.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "parser/parser.h"
#include "util/util.h"
#include <shell/shell.h>

void
	parse_param(t_parser *parser, struct s_arg_item *arg, size_t pos)
{
	static const char	*ops[] = {":-", "-", ":=", "=", ":?", "?", ":+", "+",
	"##", "#", "%%", "%", ":", NULL};
	t_token *const		token = &parser->list.tokens[pos];
	char				*name;
	char				*err;
	size_t				sep;

	arg->type = ARG_PARAMETER;
	arg->param.flags = token->flags;
	arg->param.op = NULL;
	arg->param.word = NULL;
	arg->param.offset = 0;
	arg->param.length = (size_t) - 1;
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
		parser_error(parser, err, pos, pos + 1);
	}
	// TODO: Call parser recursively on content & set context for errors
}
