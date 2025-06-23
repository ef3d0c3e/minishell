/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete_opts.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/19 06:48:54 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "builtins/complete/complete.h"
#include <shell/shell.h>

static void
	add_options(
	t_complete_buf *items,
	const t_cmd_completion *comp,
	const char *filter)
{
	size_t					i;
	const t_cmd_comp_opt	*opt;
	char					*fmt;

	if (!comp)
		return ;
	i = 0;
	while (i++ < comp->opts_size)
	{
		opt = &comp->opts[i - 1];
		if (opt->shortname && (!filter || !filter[0]
				|| complete_match(filter + 1, opt->shortname)))
		{
			ft_asprintf(&fmt, "-%s", opt->shortname);
			complete_buf_push(items, (t_complete_item){
				COMPLETE_OPTION, fmt, ft_strdup(opt->description)});
		}
		if (opt->longname && (!filter || !filter[0] || !filter[1]
				|| complete_match(filter + 2, opt->longname)))
		{
			ft_asprintf(&fmt, "--%s", opt->longname);
			complete_buf_push(items, (t_complete_item){
				COMPLETE_OPTION, fmt, ft_strdup(opt->description)});
		}
	}
}

/** @brief Sources completion file from `SHELL_COMPLETION` variable */
static void
	source_completion(t_shell *shell, const char *cmd)
{
	const char	*dir = get_variable_value(shell, "SHELL_COMPLETION");
	t_pathbuf	path;
	char		*source;
	struct stat	sb;

	if (!dir)
		return ;
	pathbuf_init(&path, 64);
	pathbuf_append(&path, dir, 1);
	pathbuf_append(&path, cmd, 0);
	pathbuf_append(&path, ".sh", 0);
	if (stat(pathbuf_cstr(&path), &sb) == -1 || !S_ISREG(sb.st_mode))
	{
		pathbuf_free(&path);
		return ;
	}
	ft_asprintf(&source, "source %s", pathbuf_cstr(&path));
	pathbuf_free(&path);
	ctx_eval_stdout(shell, source);
}

void
	repl_complete_opts(
	t_shell *shell,
	const t_repl_data *data,
	t_complete_buf *items)
{
	if (!data->cmd || !(data->kind & COMP_OPTS))
		return ;
	if (data->filter && data->filter[0] != '-')
		return ;
	source_completion(shell, data->cmd);
	add_options(items, rb_find(&shell->cmd_completion, data->cmd),
		data->filter);
	complete_remove(shell, data->cmd);
}
