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
#include <expansion/expansion.h>

// TODO: Optionnaly add other expanions
int
	expand_tilde(t_environ *env, t_token *token, t_token_list *result)
{
	char	*expanded;

	if (token->word.len < 1 || token->word.str[0] != '~')
		return (0);
	expanded = rb_find(&env->env, "HOME");
	if (expanded)
	{
		stringbuf_replace(&token->word, 0, 1, expanded);
	}
	else
	{
		stringbuf_free(&token->word);
		token->type = TOK_ERROR;
		token->err.str = "Failed to perform `~` expansion: $HOME not set";
		token->err.len = ft_strlen(token->err.str);
	}
	token_list_push(result, *token);
	return (1);
	return (0);
	
}

// TODO: Expansion
t_token_list
	token_expand(t_environ *env, t_token_list list)
{
	t_token_list	new;
	size_t			i;

	new.tokens = xmalloc(list.size * sizeof(t_token));
	new.size = 0;
	new.capacity = list.size;
	i = 0;
	while (i < list.size)
	{
		if (list.tokens[i].type == TOK_SPACE && i && i + 1 < list.size)
		{
			if (!token_isword(list.tokens[i - 1].type)
				|| !token_isword(list.tokens[i + 1].type))
			{
				++i;
				continue;
			}
		}
		if (list.tokens[i].type == TOK_WORD && expand_tilde(env, &list.tokens[i], &new))
			;
		else
			token_list_push(&new, list.tokens[i]);
		++i;
	}
	free(list.tokens);
	return (new);
}
