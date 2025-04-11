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
#include "shell/eval.h"
#include "util/util.h"
#include <expansion/expansion.h>
#include <stdio.h>

/** @brief Expands from the content of a variable */
static inline int
	expand_from_var(
	t_token *token,
	t_token_list *result,
	size_t len,
	const char *expanded)
{
	if (expanded)
	{
		token->type = TOK_SINGLE_QUOTE;
		stringbuf_replace(&token->word, 0, len, expanded);
	}
	else
	{
		token_free(token);
		token->type = TOK_ERROR;
		token->err.str = "Failed to perform tilde expansion: variable not set";
		token->err.len = ft_strlen(token->err.str);
	}
	token_list_push(result, *token);
	return (expanded != 0);
}

// TODO: Optionnaly add other expanions
int
	expand_tilde(t_environ *env, t_token *token, t_token_list *result)
{
	t_string	str;

	if (token->type != TOK_WORD
		|| token->word.len < 1 || token->word.str[0] != '~')
		return (0);
	str = (t_string){.str = token->word.str, .len = token->word.len};
	if (!str_cmp(str, "~-"))
		return (expand_from_var(token, result, 2, rb_find(&env->env, "OLDPWD")));
	else if (!str_cmp(str, "~+"))
		return (expand_from_var(token, result, 2, rb_find(&env->env, "PWD")));
	// TODO: Parse next word and check /etc/passwd for home directory
	return (expand_from_var(token, result, 1, rb_find(&env->env, "HOME")));
}
