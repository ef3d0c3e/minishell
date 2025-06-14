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

static size_t
	param_sep(const char *name)
{
	size_t	sep;

	sep = 0;
	if (name[0] == '?' || name[0] == '#' || name[0] == '$' || name[0] == '@'
		|| name[0] == '#')
		sep = 1;
	else
	{
		while ((name[sep] >= '0' && name[sep] <= '9')
			|| (name[sep] >= 'a' && name[sep] <= 'z')
			|| (name[sep] >= 'A' && name[sep] <= 'Z')
			|| (name[sep] == '_'))
			++sep;
	}
	return (sep);
}

int
	parse_param_atom(t_parser *parser, struct s_atom *arg)
{
	t_token *const		token = &parser->list.tokens[parser->pos];
	char				*name;
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
		return (arg->param.name = ft_strdup(name), 1);
	sep = param_sep(name);
	if (sep != token->word.len)
	{
		parser_error(parser, ft_strdup("Param substitution is not implemented"),
			parser->pos, parser->pos);
		return (0);
	}
	arg->param.name = ft_strdup(name);
	return (1);
}
/* TODO:
	static const char	*ops[] = {":-", "-", ":=", "=", ":?", "?", ":+", "+",
	"##", "#", "%%", "%", ":", NULL};
	char				*err;
	arg->param.op = str_alternatives((t_string){name + sep,
		ft_strlen(name + sep)}, ops, 0);
	if (!arg->param.op)
	{
		ft_asprintf(&err, "Invalid parameter expansion: `%s`", name);
		parser_error(parser, err, parser->pos, parser->pos + 1);
	}
	// TODO: Call parser recursively on content & set context for errors
*/

void
	atom_wordcontent(t_string_buffer *buf, const t_atom *atom)
{
	if (atom->type == W_LITERAL || atom->type == W_SUBEXPR)
	{
		stringbuf_append(buf, (t_string){atom->text.str, atom->text.len});
		return ;
	}
	if (atom->param.simple_param)
	{
		stringbuf_append(buf, (t_string){atom->param.name,
			ft_strlen(atom->param.name)});
		return ;
	}
}
