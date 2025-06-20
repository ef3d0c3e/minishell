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
	t_string_buffer		cmd;

	i = 0;
	while (i < list->size && list->tokens[i].end < line->cursor_index)
		++i;
	j = 0;
	while (i)
	{
		j = i;
		while (i && token_isword(list->tokens[i - 1].type))
			--i;
		if (i <= 2 || (i >= 2 && !token_isword(list->tokens[i - 2].type)))
			break ;
		i -= 2;
		if (i && list->tokens[i - 1].type == TOK_SPACE)
			--i;
	}
	if (i == j)
		return (NULL);
	stringbuf_init(&cmd, 24);
	while (i < j)
	{
		if (!token_isword(list->tokens[i].type))
			break ;
		token_wordcontent(&cmd, &list->tokens[i++]);
	}
	return (stringbuf_cstr(&cmd));
}

static void
	add_options(t_complete_buf *items, const t_cmd_completion *comp)
{
	size_t					i;
	const t_cmd_comp_opt	*opt;
	char					*fmt;

	if (!comp)
		return ;
	i = 0;
	while (i < comp->opts_size)
	{
		opt = &comp->opts[i];
		if (opt->shortname)
		{
			ft_asprintf(&fmt, "-%s", opt->shortname);
			complete_buf_push(items, (t_complete_item){
					COMPLETE_OPTION, fmt, ft_strdup(opt->description) });
		}
		if (opt->longname)
		{
			ft_asprintf(&fmt, "--%s", opt->longname);
			complete_buf_push(items, (t_complete_item){
					COMPLETE_OPTION, fmt, ft_strdup(opt->description) });
		}
		++i;
	}
}

void
	repl_complete_opts(t_getline *line, t_complete_buf *items, const char *filter)
{
	char				*cmd;

	cmd = cmd_name(line);
	if (!cmd)
		return ;
	add_options(items, rb_find(&line->shell->cmd_completion, cmd));
	free(cmd);
}
