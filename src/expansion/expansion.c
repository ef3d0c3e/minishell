/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tokenizer/tokenizer.h"
#include <shell/shell.h>

/** @brief Removes spaces before and after operators */
static int
	remove_space(const t_token_list *list, size_t *i)
{
	const t_token	*prev;
	const t_token	*next;
	if (list->tokens[*i].type != TOK_SPACE)
		return (0);
	if (*i + 1 >= list->size
		|| *i == 0)
	{
		++*i;
		return (1);
	}
	prev = &list->tokens[*i - 1];
	next = &list->tokens[*i + 1];
	if ((token_isword(next->type) || next->type == TOK_PARAM
		|| next->type == TOK_PARAM_SIMPLE || next->type == TOK_CMD_SUB
		|| next->type == TOK_REDIR)
		&& (token_isword(prev->type) || prev->type == TOK_PARAM
		|| prev->type == TOK_PARAM_SIMPLE || prev->type == TOK_CMD_SUB))
		return (0);
	++*i;
	return (1);
	/*
	if (list->tokens[*i].type == TOK_SPACE
		&& ((*i && (token_precedence(&list->tokens[*i - 1]) >= 0
		|| list->tokens[*i - 1].type == TOK_REDIR))
		|| (*i + 1 < list->size
		&& token_precedence(&list->tokens[*i + 1]) >= 0)
		|| *i + 1 >= list->size))
	{
		++(*i);
		return (1);
	}
	return (0);
	*/
}

/** @brief Removes spaces before and after operators */
static int
	remove_comment(const t_token_list *list, size_t *i)
{
	if (list->tokens[*i].type != TOK_COMMENT)
		return (0);
	++*i;
	return (1);
}

static int
	merge_subsequent(t_token *first, t_token *second)
{
	t_string_buffer	content;

	if (!token_isword(first->type) || !token_isword(second->type))
		return (0);
	if (first->type == TOK_ASSIGN || second->type == TOK_ASSIGN)
		return (0);
	if (first->flags != second->flags)
		return (0);
	if (first->type == TOK_DIGIT && second->type == TOK_MINUS)
		return (0);
	stringbuf_init(&content, second->end - first->start);
	token_wordcontent(&content, first);
	token_wordcontent(&content, second);
	token_free(first);
	token_free(second);
	first->end = second->end;
	first->type = TOK_SINGLE_QUOTE;
	first->word = content;
	return (1);
}

static t_token_list
	token_join(t_shell *shell, t_token_list list)
{
	t_token_list	new;
	size_t			i;
	size_t			j;

	token_list_init(&new, list.size);
	i = 0;
	while (i < list.size)
	{
		j = i + 1;
		while (j < list.size && merge_subsequent(&list.tokens[i], &list.tokens[j]))
		{
			++j;
		}
		token_list_push_token(&new, list.tokens[i]);
		i = j;
	}
	free(list.tokens);
	return (new);
}

t_token_list
	token_expand(t_shell *shell, t_token_list list)
{
	t_token_list	new;
	size_t			i;

	token_list_init(&new, list.size);
	i = 0;
	while (i < list.size)
	{
		if (remove_space(&list, &i) || remove_comment(&list, &i))
			continue ;
		//if (expand_tilde(shell, &list.tokens[i], &new))
		//	;
		//else if (expand_param(shell, &list.tokens[i], &new))
		//	;
		//else if (expand_cmdsub(shell, &list.tokens[i], &new))
		//	;
		//else if (expand_filename(shell, &list.tokens[i], &new))
		//	;
		//else
		token_list_push_token(&new, list.tokens[i]);
		++i;
	}
	free(list.tokens);
	return (token_join(shell, new));
}
