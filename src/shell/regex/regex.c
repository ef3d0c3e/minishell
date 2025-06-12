/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell/regex/regex.h"
#include "util/util.h"
#include <shell/shell.h>
#include <stddef.h>

void
	regex_shellopt_register(t_shell *shell)
{
	option_insert(shell, "extglob", "If set, the extended pattern matching "
		"features are enabled", 0);
	option_insert(shell, "globstar", "If set, the pattern '**' used in a"
		" filename expansion context will match all files and zero or more"
		" directories and subdirectories.  If the pattern is followed by a '/',"
		" only directories and subdirectories match.", 0);
	option_insert(shell, "dotglob", "If set, includes filenames beginning with"
			" a '.'  in the results of filename expansion.  The filenames '.'"
			" and '..' must always be matched explicitly, even if 'dotglob' is"
			" set.", 0);
	option_insert(shell, "nocaseglob", "If set, matches filenames in a"
		" case-insensitive fashion when performing filename expansion.", 0);
	option_insert(shell, "nullglob", "If set, allows filename patterns which "
		"match no files to expand to a null string, rather than themselves.", 0);
	option_insert(shell, "failglob", "If set, patterns which fail to match "
		"filenames during filename expansion result in an expansion error.", 0);
}

t_globopts
	regex_shellopt_get(t_shell *shell)
{
	return ((t_globopts){
			.extglob = option_value(shell, "extglob"),
			.globstar = option_value(shell, "globstar"),
			.dotglob = option_value(shell, "dotglob"),
			.nocaseglob = option_value(shell, "nocaseglob"),
			.nullglob = option_value(shell, "nullglob"),
			.failglob = option_value(shell, "failglob"),
	});
}

void
	regex_error(t_reg_parser *parser, const char *msg, size_t pos)
{
	if (parser->regex.errors_size >= parser->regex.errors_capacity)
	{
		parser->regex.errors_capacity = parser->regex.errors_capacity * 2
			+ !parser->regex.errors_capacity;
		parser->regex.errors = ft_realloc(parser->regex.errors,
				sizeof(char *) * parser->regex.errors_size,
				sizeof(char *) * parser->regex.errors_capacity);
	}
	(void)pos;
	parser->regex.errors[parser->regex.errors_size++] = ft_strdup(msg);
}

int
	regex_error_flush(t_reg_parser *parser)
{
	size_t	i;

	i = 0;
	while (i < parser->regex.errors_size)
		ft_dprintf(2, "%s\n", parser->regex.errors[i++]);
	parser->regex.errors_size = 0;
	return (i == 0);
}

size_t
	regex_recurse_depth(const t_regex_ast *node)
{
	size_t	total;
	size_t	status;
	size_t	i;

	if (!node)
		return (0);
	total = 0;
	i = 0;
	if (node->type == M_LITERAL)
	{
		while (node->literal[i])
			total += node->literal[i++] == '/';
	}
	else if (node->type == M_EXTGLOB)
	{
		while (i < node->glob.ngroups)
		{
			status = regex_recurse_depth(node->glob.groups[i++]);
			if (status > total)
				total = status;
		}
	}
	else if (node->type == M_SEQ)
	{
		while (i < node->compound.ngroups)
		{
			status = regex_recurse_depth(node->compound.groups[i++]);
			if (status == (size_t)-1)
				return (-1);
			total += status;
		}
	}
	else if (node->type == M_GLOBSTAR)
		return (-1);
	return (total);
}
