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
	exec_completion(t_shell *shell, const t_token_list *list)
{
	size_t						size;
	struct s_cmd_completion_tr	tr;

	if (list->size > 1 && token_isword(list->tokens[list->size - 2].type))
		return (NULL);
	size = shell->reg_builtins.size + shell->path_cache.size;
	tr.list = xmalloc(sizeof(char *) * (size + 1));
	tr.index = 0;
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

char **my_generator(const char *text, int start, int end)
{
	t_string		prompt;
	t_token_list	list;
	char			**result;
	
	prompt.str = rl_line_buffer;
	prompt.len = end;
	list = tokenizer_tokenize(prompt);
	//token_list_print(prompt, &list);

	result = exec_completion(readline_get_session(NULL), &list);

	token_list_free(&list);
	return result;
}

void
	readline_setup(t_shell *shell)
{
	readline_get_session(shell);
	rl_attempted_completion_function = my_generator;
	//rl_redisplay_function = my_redisplay;
	rl_getc_function = readline_getc;
	rl_catch_signals = 0;
	using_history();
}
