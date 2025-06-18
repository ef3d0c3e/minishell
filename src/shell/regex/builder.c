/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "util/util.h"
#include <shell/shell.h>

t_regex_builder
	regex_builder_new(void)
{
	t_regex_builder	builder;

	builder.regex = (t_regex){
		.errors = NULL,
		.errors_size = 0,
		.errors_capacity = 0,
		.expr = regex_new(M_SEQ),
	};
	return (builder);
}

/** @brief Merges two @ref M_SEQ regex nodes */
static void
	regex_merge(t_regex_ast *seq, t_regex_ast *other)
{
	size_t	i;

	seq->compound.groups = ft_realloc(seq->compound.groups,
		sizeof(t_regex_ast *) * seq->compound.ngroups,
		sizeof(t_regex_ast *) * (seq->compound.ngroups
			+ other->compound.ngroups));
	i = 0;
	while (i < other->compound.ngroups)
		seq->compound.groups[seq->compound.ngroups++]
			= other->compound.groups[i++];
	free(other->compound.groups);
	free(other);
}

int
	regex_builder_expr(
	const t_globopts *opts,
	t_regex_builder *builder,
	const char *expr)
{
	t_regex	sub;

	sub = regex_parse(opts, expr);
	if (!sub.expr)
	{
		regex_free(builder->regex.expr);
		return (0);
	}
	regex_merge(builder->regex.expr, sub.expr);
	return (1);
}

int
	regex_builder_literal(
	const t_globopts *opts,
	t_regex_builder *builder,
	const char *literal)
{
	t_regex_ast	*node;

	(void)opts;
	node = regex_new(M_LITERAL);
	node->literal = ft_strdup(literal);
	regex_seq_append(builder->regex.expr, node);
	return (1);
}
