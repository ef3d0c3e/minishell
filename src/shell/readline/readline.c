/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "tokenizer/tokenizer.h"
#include "util/util.h"
#include <readline/readline.h>
#include <readline/rltypedefs.h>
#include <shell/shell.h>
#include <stdio.h>

t_shell
	*readline_get_session(t_shell *shell)
{
	static t_shell	*session;
	if (shell)
		session = shell;
	return (session);
}

struct s_cmd_completion_tr
{
	size_t			index;
	char			**list;
	t_string_buffer	starts_with;
};

static int
	completion_match(const t_string_buffer *word, const char *str)
{
	size_t	i;

	i = 0;
	while (i < word->len)
	{
		if (!str[i])
			return (-1);
		if (str[i] != word->str[i])
			return (0);
		++i;
	}
	return (1);
}

static void
	complete_path(size_t depth, t_rbnode *node, void *ptr)
{
	struct s_cmd_completion_tr *const	tr = ptr;

	(void)depth;
	if (!tr->starts_with.len)
	{
		tr->list[tr->index++] = ft_strdup(node->key);
		return ;
	}
	if (completion_match(&tr->starts_with, node->key) == 1)
		tr->list[tr->index++] = ft_strdup(node->key);
}

char**
	exec_completion(t_shell *shell, const t_token_list *list, const char *input)
{
	size_t						size;
	struct s_cmd_completion_tr	tr;

	if (list->size > 1 && token_isword(list->tokens[list->size - 2].type))
		return (NULL);
	size = shell->reg_builtins.size + shell->path_cache.size;
	tr.list = xmalloc(sizeof(char *) * (size + 2));
	tr.index = 0;
	tr.list[tr.index++] = ft_strdup(input);
	tr.starts_with.str = NULL;
	stringbuf_init(&tr.starts_with, 24);
	if (list->size && token_isword(list->tokens[list->size - 1].type))
		token_wordcontent(&tr.starts_with, &list->tokens[list->size - 1]);
	rb_apply(&shell->path_cache, complete_path, &tr);
	tr.list[tr.index] = NULL;
	stringbuf_free(&tr.starts_with);
	if (!tr.list[0])
	{
		free(tr.list);
		return (NULL);
	}
	return (tr.list);
}

char **my_completion(const char *text, int start, int end)
{
	t_string		prompt;
	t_token_list	list;
	char			**result;
	rl_attempted_completion_over = 1;
	prompt.str = rl_line_buffer;
	prompt.len = end;
	list = tokenizer_tokenize(prompt);
	//token_list_print(prompt, &list);

	result = exec_completion(readline_get_session(NULL), &list, text);

	token_list_free(&list);
	//size_t i = 0;
	//while (result && result[i])
	//{
	//	ft_dprintf(2, "%zu = %s\n", i, result[i]);
	//	++i;
	//}
	return result;
}

//void my_redisplay(void) {
//    int save_point = rl_point;
//    rl_redisplay();
//	fputs("\r", rl_outstream);
//    fputs(rl_prompt, rl_outstream);
//	/* TODO: Tokenize prompt & color tokens */
//	fputs(rl_line_buffer, rl_outstream);
//    rl_point = save_point;
//}


/* Get current terminal width */
int get_terminal_width() {
	int w;
	rl_get_screen_size(0, &w);
    return w;
}


void
original_redisplay(rl_voidfunc_t *ptr)
{
	static rl_voidfunc_t *orig = NULL;

	if (ptr)
		orig = ptr;
	else
		((rl_voidfunc_t*)orig)();
}

void my_redisplay(void)
{
	char *buf_copy = ft_strdup(rl_line_buffer);

	rl_delete_text(0, rl_end); /* delete current line buffer */
	rl_point = rl_end = 0; /* reset cursor position */
	original_redisplay(NULL); /* refresh the printed line */
	fputs("\x1b[1;31m", stdout); /* insert the desired color code */
	rl_insert_text(buf_copy); /* reinsert the line buffer */


	/*
	char *const		saved_line = rl_line_buffer;
	int point = rl_point;
	t_string_buffer	buf;
	t_token_list	list;
	size_t			i;

	buf = stringbuf_from(saved_line);
	char *r;
	ft_asprintf(&r, "\001\033[31m\002%.*s", buf.len, buf.str);
	buf.len = ft_strlen(r);
	buf.capacity = ft_strlen(r);
	free(buf.str);
	buf.str = r;
	//list = tokenizer_tokenize((t_string){saved_line, ft_strlen(saved_line)});
	//i = 0;
	//while (i < list.size)
	//{
	//	if (list.tokens[i].type == TOK_KEYWORD)
	//	{
	//		// red keyword
	//		stringbuf_replace(&buf, list.tokens[i].start, list.tokens[i].start, "\033[31m");
	//		stringbuf_replace(&buf, list.tokens[i].end, list.tokens[i].end, "\033[0m");
	//	}
	//	++i;
	//}


	rl_line_buffer = stringbuf_cstr(&buf);
	//ft_dprintf(2, "%s", rl_line_buffer);
	original_redisplay(NULL);
	rl_point = point;
	rl_line_buffer = saved_line;
	stringbuf_free(&buf);
	*/
}


void my_display_hook(char **matches, int num, int maxlen) {
    printf("\n=== Custom Completion ===\n");
    for (int i = 1; i <= num; ++i) {
        printf("  [%d] %s\n", i, matches[i]);
    }
    rl_on_new_line();
    rl_redisplay();
}


void
	readline_setup(t_shell *shell)
{
	//RL_PROMPT_END_IGNORE
	readline_get_session(shell);
	rl_attempted_completion_function = my_completion;

	original_redisplay(rl_redisplay_function);
	rl_redisplay_function = my_redisplay;

	rl_getc_function = readline_getc;
	rl_catch_signals = 0;
	using_history();
}
