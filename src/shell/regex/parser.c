/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static t_regex_ast
	*regex_parse_sequence(t_reg_parser *parser, const char *until);

static t_regex_ast
	*regex_parse_extglob(t_reg_parser *parser)
{
	t_regex_ast	*node;
	t_regex_ast	*sub;

	node = regex_new(M_EXTGLOB);
	node->glob.op = parser->str[parser->pos++];
	++parser->pos;
	while (1)
	{
		sub = regex_parse_sequence(parser, "|)");
		node->glob.groups = ft_realloc(node->glob.groups,
				sizeof(t_regex_ast *) * node->glob.ngroups,
				sizeof (t_regex_ast *) * (node->glob.ngroups + 1));
		node->glob.groups[node->glob.ngroups++] = sub;
		if (parser->str[parser->pos] != '|')
			break ;
		++parser->pos;
	}
	if (parser->str[parser->pos] != ')')
		regex_error(parser, "Unmatched `(`", parser->pos);
	++parser->pos;
	return (node);
}

static t_regex_ast
	*regex_parse_element_special(t_reg_parser *parser)
{
	t_regex_ast	*node;

	if (parser->str[parser->pos] == '\\' && parser->str[parser->pos + 1])
	{
		++parser->pos;
		node = regex_new(M_LITERAL);
		ft_asprintf(&node->literal, "%c", parser->str[parser->pos++]);
		return (node);
	}
	else if (parser->opts->extglob && parser->str[parser->pos]
		&& ft_strchr("?*+@!", parser->str[parser->pos])
		&& parser->str[parser->pos + 1] == '(')
		return (regex_parse_extglob(parser));
	return (NULL);
}

static t_regex_ast
	*regex_parse_element(t_reg_parser *parser)
{
	t_regex_ast	*node;

	node = regex_parse_element_special(parser);
	if (node)
		return (node);
	else if (parser->str[parser->pos] == '?' && ++parser->pos)
		return (regex_new(M_ANY));
	else if (parser->str[parser->pos] == '*' && ++parser->pos)
	{
		if (parser->opts->globstar && parser->str[parser->pos] == '*'
			&& ++parser->pos)
		{
			if (!parser->str[parser->pos]
				|| (parser->str[parser->pos] == '/' && ++parser->pos))
				return (regex_new(M_GLOBSTAR));
		}
		return (regex_new(M_STAR));
	}
	else if (parser->str[parser->pos])
	{
		node = regex_new(M_LITERAL);
		ft_asprintf(&node->literal, "%c", parser->str[parser->pos++]);
		return (node);
	}
	return (NULL);
}

static t_regex_ast
	*regex_parse_sequence(t_reg_parser *parser, const char *s)
{
	t_regex_ast	*node;
	t_regex_ast	*inner;

	node = xmalloc(sizeof(t_regex_ast));
	node->type = M_SEQ;
	node->compound.groups = NULL;
	node->compound.ngroups = 0;
	while (parser->str[parser->pos] && !ft_strchr(s, parser->str[parser->pos]))
	{
		inner = regex_parse_element(parser);
		if (!inner)
			break ;
		node->compound.groups = ft_realloc(node->compound.groups,
				sizeof(t_regex_ast *) * (node->compound.ngroups),
				sizeof(t_regex_ast *) * (node->compound.ngroups + 1));
		node->compound.groups[node->compound.ngroups++] = inner;
	}
	return (node);
}

t_regex
	regex_parse(const t_globopts *opts, const char *pattern)
{
	t_reg_parser	parser;

	parser.opts = opts;
	parser.str = pattern;
	parser.pos = 0;
	parser.regex.errors = NULL;
	parser.regex.errors_size = 0;
	parser.regex.errors_capacity = 0;
	parser.regex.expr = regex_parse_sequence(&parser, ")");
	if (!parser.regex.errors_size && parser.str[parser.pos] != '\0' )
		regex_error(&parser, "Invalid pattern", parser.pos);
	if (!regex_error_flush(&parser))
	{
		regex_free(parser.regex.expr);
		free(parser.regex.errors);
		parser.regex.expr = NULL;
	}
	return (parser.regex);
}
