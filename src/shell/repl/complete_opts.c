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
#include <shell/shell.h>

/** @brief Gets the command's name */
static char
	*cmd_name(t_getline *line)
{
	t_token_list *const	list = &((t_repl_data *)line->data)->list;
	size_t				i;
	size_t				j;

	i = 0;
	while (i < list->size && list->tokens[i].end < line->cursor_index)
		++i;
	j = 0;
	while (i)
	{
		j = i;
		while (i && token_isword(list->tokens[i - 1].type))
			--i;
		if (i < 2 || list->tokens[i - 1].type != TOK_SPACE)
			break ;
		i -= 2;
		if (i && list->tokens[i - 1].type == TOK_SPACE)
			--i;
	}
	if (i == j)
		return (NULL);
	return (complete_token_content(list, i, j));
}

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
	t_getline *line,
	t_complete_buf *items,
	const char *filter)
{
	char				*cmd;

	cmd = cmd_name(line);
	if (!cmd)
		return ;
	source_completion(line->shell, cmd);
	add_options(items, rb_find(&line->shell->cmd_completion, cmd), filter);
	free(cmd);
}
