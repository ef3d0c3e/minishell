/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <expansion/expansion.h>

int
	expand_param(t_environ *env, t_token *token, t_token_list *result)
{
	char	*expanded;

	if (token->type == TOK_PARAM_SIMPLE)
	{
		expanded = rb_find(&env->env, stringbuf_cstr(&token->word));
		if (!expanded)
			expanded = "";
		stringbuf_free(&token->word);
		token->type = TOK_SINGLE_QUOTE;
		token->word = stringbuf_from(expanded);
		token_list_push(result, *token);
		return (1);
	}
	return (0);
}
