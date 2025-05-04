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
#include "parser/parser.h"
#include "ft_printf.h"
#include "parser/words/words.h"
#include "shell/regex/regex.h"
#include "util/util.h"
#include <shell/shell.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct s_reg_parser
{
	/** @brief Pattern options */
	const t_globopts	*opts;
	/** @brief Input string */
	const char			*str;
	/** @brief Parser's position */
	size_t				pos;
	/** @brief Errors from the parser */
	char				**errors;
	/** @brief Number of errors */
	size_t				errors_size;
	/** @brief Errors capacity */
	size_t				errors_capacity;
}	t_reg_parser;

void
	regex_error(t_reg_parser *parser, const char *msg, size_t pos)
{
	if (parser->errors_size >= parser->errors_capacity)
	{
		parser->errors_capacity = parser->errors_capacity * 2
			+ !parser->errors_capacity;
		parser->errors = ft_realloc(parser->errors,
				sizeof(char *) * parser->errors_size,
				sizeof(char *) * parser->errors_capacity);
	}
	(void)pos;
	// TODO Format using `pos`
	parser->errors[parser->errors_size++] = ft_strdup(msg);
}

int
	regex_error_flush(t_reg_parser *parser)
{
	size_t	i;

	i = 0;
	while (i < parser->errors_size)
		ft_dprintf(2, "%s\n", parser->errors[i++]);
	parser->errors_size = 0;
	return (i == 0);
}

void
	regex_seq_append(t_regex_ast *seq, t_regex_ast *node)
{
	seq->compound.groups = ft_realloc(seq->compound.groups, 
			sizeof(t_regex_ast *) * (seq->compound.ngroups),
			sizeof(t_regex_ast *) * (seq->compound.ngroups + 1));
	seq->compound.groups[seq->compound.ngroups++] = node;
}

t_regex_ast
*regex_parse_sequence(t_reg_parser *parser);
t_regex_ast
*regex_parse_sequence_until(t_reg_parser *parser, const char *until);

t_regex_ast
	*regex_parse_extglob(t_reg_parser *parser)
{
	t_regex_ast	*node;
	t_regex_ast	*sub;

	node = regex_new(M_EXTGLOB);
	node->glob.op = parser->str[parser->pos++];
	++parser->pos;
	while (1)
	{
		sub = regex_parse_sequence_until(parser, "|)");
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

t_regex_ast
	*regex_parse_element(t_reg_parser *parser)
{
	t_regex_ast	*node;

	if (parser->str[parser->pos] == '\\' && parser->str[parser->pos + 1])
	{
		++parser->pos;
		node = regex_new(M_LITERAL);
		// TODO: Unicode ?
		ft_asprintf(&node->literal, "%c", parser->str[parser->pos++]);
		return (node);
	}
	else if (parser->opts->extglob && parser->str[parser->pos]
			&& ft_strchr("?*+@!", parser->str[parser->pos])
			&& parser->str[parser->pos + 1] == '(')
		return (regex_parse_extglob(parser));
	else if (parser->str[parser->pos] == '?' && ++parser->pos)
		return (regex_new(M_ANY));
	else if (parser->str[parser->pos] == '*' && ++parser->pos)
	{
		if (parser->str[parser->pos] == '*' && ++parser->pos)
			return (regex_new(M_GLOBSTAR));
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

t_regex_ast
	*regex_parse_sequence_until(t_reg_parser *parser, const char *s)
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
		regex_seq_append(node, inner);
	}
	return (node);
}

t_regex_ast
	*regex_parse_sequence(t_reg_parser *parser)
{
	t_regex_ast	*node;
	t_regex_ast	*inner;

	node = xmalloc(sizeof(t_regex_ast));
	node->type = M_SEQ;
	node->compound.groups = NULL;
	node->compound.ngroups = 0;
	while (parser->str[parser->pos] && parser->str[parser->pos] != ')')
	{
		inner = regex_parse_element(parser);
		if (!inner)
			break ;
		regex_seq_append(node, inner);
	}
	return (node);
}

t_regex_ast
	*regex_parse(const t_globopts *opts, const char *pattern)
{
	t_reg_parser	parser;
	t_regex_ast		*ast;

	parser.opts = opts;
	parser.str = pattern;
	parser.pos = 0;
	parser.errors = NULL;
	parser.errors_size = 0;
	parser.errors_capacity = 0;

	ast = regex_parse_sequence(&parser);
	if (!parser.errors_size && parser.str[parser.pos] != '\0' )
		regex_error(&parser, "Invalid pattern", parser.pos);
	if (regex_error_flush(&parser))
		return (ast);
	regex_free(ast);
	return (NULL);
}

int main(int ac, char **av, const char **envp)
{
	t_globopts opts = {
		.extglob = 1,
		.globstar = 1,
		.dotglob = 1,
		.nocaseglob = 0,
		.nullglob = 1,
		.failglob = 1,
	};
	t_regex_ast	*ast = regex_parse(&opts, av[1]);
	regex_print(0, ast);
	regex_free(ast);
	return (0);
}
