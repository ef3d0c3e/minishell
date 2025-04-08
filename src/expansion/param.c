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
#include "tokenizer/tokenizer.h"
#include "util/util.h"
#include <expansion/expansion.h>
#include <math.h>

static inline int
	expand_param_complex(t_environ *env, t_token *token, t_token_list *result)
{
	size_t			sep;
	char			*expanded;
	t_string_buffer	name;

	sep = 0;
	while (sep < token->word.len && is_param_ident(token->word.str[sep]))
		++sep;
	if (sep != token->word.len)
	{
		stringbuf_free(&token->word);
		token->type = TOK_ERROR;
		token->err.str = "Bad substitution: Unsupported parameter expansion";
		token->err.len = ft_strlen(token->err.str);
		token_list_push(result, *token);
		return (0);
	}
	name = stringbuf_from_range(token->word.str, token->word.str + sep);
	expanded = rb_find(&env->env, stringbuf_cstr(&name));
	if (!expanded)
		expanded = "";
	stringbuf_free(&token->word);
	stringbuf_free(&name);
	token->type = TOK_SINGLE_QUOTE;
	token->word = stringbuf_from(expanded);
	token_list_push(result, *token);
	return (1);
}

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
	else if (token->type == TOK_PARAM)
		return (expand_param_complex(env, token, result));
	return (0);
}
