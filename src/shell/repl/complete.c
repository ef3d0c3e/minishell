/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/19 06:48:54 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static void
	get_filter(t_repl_data *data)
{
	size_t			i;
	t_string_buffer	buf;
	const t_token	*tok;

	if (data->tok_start >= data->list.size)
		return ;
	i = 0;
	stringbuf_init(&buf, 24);
	while (data->tok_start + i <= data->tok_end)
	{
		tok = &data->list.tokens[data->tok_start + i];
		if (tok->type == TOK_PARAM_SIMPLE)
		{
			data->kind |= COMP_PARAM;
			stringbuf_append(&buf, (t_string){tok->word.str, tok->word.len});
		}
		else
			token_wordcontent(&buf, tok);
		++i;
	}
	if (buf.len)
		data->filter = stringbuf_cstr(&buf);
	else
		stringbuf_free(&buf);
}

static void
	get_command(t_repl_data *data, size_t sequence_start)
{
	const t_token	*tok;
	t_string_buffer	buf;
	size_t			i;

	data->sequence_start = data->tok_start == sequence_start;
	if (data->tok_start == sequence_start)
		data->kind |= COMP_CMD;
	else
		data->kind |= COMP_OPTS;
	i = 0;
	stringbuf_init(&buf, 24);
	while (sequence_start + i < data->list.size)
	{
		tok = &data->list.tokens[sequence_start + i];
		if (tok->type == TOK_SPACE || !token_isword(tok->type))
			break ;
		token_wordcontent(&buf, tok);
		++i;
	}
	if (buf.len)
		data->cmd = stringbuf_cstr(&buf);
	else
		stringbuf_free(&buf);
}

static int
	completion_get_populate(
	t_repl_data *data,
	size_t *start,
	size_t *end,
	size_t sequence_start)
{
	const t_token	*tok;

	tok = &data->list.tokens[data->tok_end];
	*start = data->list.tokens[data->tok_start].start;
	*end = data->list.tokens[data->tok_end].end;
	if (tok->type == TOK_SPACE)
	{
		data->tok_start = ++data->tok_end;
		*start = data->list.tokens[data->list.size - 1].end;
		*end = data->list.tokens[data->list.size - 1].end;
	}
	get_filter(data);
	get_command(data, sequence_start);
	return (1);
}

static int
	completion_get(t_repl_data *data, size_t *start, size_t *end, size_t cursor)
{
	size_t			seq_start;
	const t_token	*tok;

	if (!data->list.size)
		return (data->kind |= COMP_CMD, data->sequence_start = 1, 0);
	seq_start = 0;
	while (data->list.tokens[data->tok_end].end < cursor)
	{
		tok = &data->list.tokens[data->tok_end];
		if (tok->type == TOK_PIPELINE || tok->type == TOK_SEQUENCE)
		{
			seq_start = data->tok_end + 1;
			data->tok_start = data->tok_end + 1;
		}
		if (tok->type == TOK_SPACE)
		{
			if (seq_start == data->tok_end)
				++seq_start;
			data->tok_start = data->tok_end + 1;
		}
		++data->tok_end;
	}
	return (completion_get_populate(data, start, end, seq_start));
}

//token_list_print((t_string){line->input.buffer.str, line->input.buffer.len},
//&data->list);
//ft_dprintf(2, "\n\r");
//ft_dprintf(2, "s/e= %zu %zu\n\r", *start, *end);
//ft_dprintf(2, "toks= %zu %zu\n\r", data->tok_start, data->tok_end);
//ft_dprintf(2, "filter='%s'\n\r", data->filter);
//ft_dprintf(2, "cmd='%s'\n\r", data->cmd);
//ft_dprintf(2, "is_start=%d\n\r", data->sequence_start);
//ft_dprintf(2, "kind=%x\n\r", data->kind);
//exit(1);
t_complete_item
	*repl_completer(t_getline *line, size_t *start, size_t *end)
{
	t_repl_data *const	data = line->data;
	t_complete_buf		items;

	data->filter = NULL;
	data->cmd = NULL;
	data->kind = COMP_FILES;
	data->tok_start = 0;
	data->tok_end = 0;
	ft_memset(&items, 0, sizeof(t_complete_buf));
	*start = 0;
	*end = line->cursor_index;
	completion_get(data, start, end, line->cursor_index);
	repl_complete_opts(line->shell, data, &items);
	repl_complete_cmd(line->shell, data, &items);
	repl_complete_filename(line->shell, data, &items);
	repl_complete_params(line->shell, data, &items);
	free(data->filter);
	free(data->cmd);
	quicksort(items.data, items.size, sizeof(t_complete_item), complete_sort);
	complete_buf_push(&items, (t_complete_item){0, NULL, NULL});
	return (items.data);
}
