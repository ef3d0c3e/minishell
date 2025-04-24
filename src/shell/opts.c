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
#include <shell/shell.h>

static void
	noop(void *data)
{
	(void)data;
}

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
	env->opts = rb_new((int (*)(const void *, const void *))ft_strcmp,
			noop, free);
	insert_opt(env, "pipefail", "If set, the return value of a pipeline is the"
		"value of the last (rightmost) command to exit with a non-zero status,"
		"or zero if all commands in the pipeline exit successfully."
		"This option is disabled by default.", 0);
	insert_opt(env, "noclobber", "If set, redirections will not be able to"
		"overwrite existing files. To bypass noclobber mode, one can use `>|`"
		"instead of `>`. This option is disabled by default ", 0);
	insert_opt(env, "dbg_token", "Display tokens for debugging", 1);
	insert_opt(env, "dbg_ast", "Display AST for debugging", 1);
}

int
	option_value(t_shell *shell, const char *name)
{
	const struct s_option	*opt = rb_find(&env->opts, (const void *)name);
	char					*err;

	if (!opt)
	{
		ft_asprintf(&err, "Failed to find option `%s`", name);
		shell_fail(env, err, SRC_LOCATION);
	}
	return (opt->value);
}
