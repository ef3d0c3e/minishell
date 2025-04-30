/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opts.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell/env/env.h"
#include <shell/shell.h>

/** @brief Does nothing */
static void
	noop(void *data)
{
	(void)data;
}

/** @brief Insert option into the registry */
static void
	insert_opt(t_shell *shell, char *name, const char *desc, int value)
{
	struct s_option *const	opt = xmalloc(sizeof(struct s_option));

	opt->desc = desc;
	opt->value = value;
	rb_insert(&shell->options, name, opt);
}

void
	options_init(t_shell *shell)
{
	shell->options = rb_new((int (*)(const void *, const void *))ft_strcmp,
			noop, free);
	insert_opt(shell, "pipefail", "If set, the return value of a pipeline is the"
		"value of the last (rightmost) command to exit with a non-zero status,"
		"or zero if all commands in the pipeline exit successfully."
		"This option is disabled by default.", 0);
	insert_opt(shell, "noclobber", "If set, redirections will not be able to"
		"overwrite existing files. To bypass noclobber mode, one can use `>|`"
		"instead of `>`. This option is disabled by default ", 0);
	insert_opt(shell, "experr", "Equivalent to bash's `set -u`", 0);
	insert_opt(shell, "dbg_parser", "Debugging option for parsing", 1);
}

int
	option_value(t_shell *shell, const char *name)
{
	const struct s_option	*opt = rb_find(&shell->options, (const void *)name);
	char					*err;

	if (!opt)
	{
		ft_asprintf(&err, "Failed to find option `%s`", name);
		shell_fail(shell, err, SRC_LOCATION);
	}
	return (opt->value);
}

int
	option_set(t_shell *shell, const char *name, int value)
{
	struct s_option *const	opt = rb_find(&shell->options, (const void *)name);

	if (!opt)
		return (0);
	opt->value = value;
	return (1);
}
