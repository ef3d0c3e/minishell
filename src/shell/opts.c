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
#include <shell/eval.h>

static void
	noop(void *data)
{
	(void)data;
}

static void
	insert_opt(t_environ *env, char *name, const char *desc, int value)
{
	struct s_option *const	opt = xmalloc(sizeof(struct s_option));

	opt->desc = desc;
	opt->value = value;
	rb_insert(&env->opts, name, opt);
}

void
	init_options(t_environ *env)
{
	env->opts = rb_new((int (*)(const void *, const void *))ft_strcmp,
			noop, free);
	insert_opt(env, "pipefail", "If set, the return value of a pipeline is the"
		"value of the last (rightmost) command to exit with a non-zero status,"
		"or zero if all commands in the pipeline exit successfully."
		"This option is disabled by default.", 0);
}

int
	option_get(t_environ *env, const char *name)
{
	const struct s_option	*opt = rb_find(&env->opts, (const void *)name);
	char					*err;

	if (!opt)
	{
		ft_asprintf(&err, "Failed to find option `%s`", name);
		shell_error(env, err, __FUNCTION__);
		shell_exit(env, EXIT_FAILURE);
	}
	return (opt->value);
}
