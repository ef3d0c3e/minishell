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
#include "builtins/builtin.h"
#include "ft_printf.h"
#include "tokenizer/tokenizer.h"
#include "util/util.h"
#include <expansion/expansion.h>

static int
	expand_special(
	t_environ *env,
	const char *name,
	t_token *token,
	t_token_list *result)
{
	if (!ft_strcmp(name, "?"))
	{
		stringbuf_free(&token->word);
		token->type = TOK_SINGLE_QUOTE;
		stringbuf_init(&token->word, 16);
		stringbuf_itoa(&token->word, env->last_status);
		token_list_push(result, *token);
		return (1);
	}
	return (0);
}

static inline int
	expand_param_complex(t_environ *env, t_token *token, t_token_list *result)
{
	size_t			sep;
	char			*expanded;
	t_string_buffer	name;
	char			*err;

	sep = 0;
	while (sep < token->word.len && is_param_ident(token->word.str[sep]))
		++sep;
	if (sep != token->word.len)
	{
		ft_asprintf(&err, "Bad substitution: `%.*s` is not a valid"
			" substitution", (int)token->word.len, token->word.str);
		shell_error(env, err, __FUNCTION__);
		return (0);
	}
	name = stringbuf_from_range(token->word.str, token->word.str + sep);
	if (expand_special(env, stringbuf_cstr(&name), token, result))
	{
		stringbuf_free(&name);
		return (1);
	}
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

// TODO: Handle other param expansion with ${...}
int
	expand_param(t_environ *env, t_token *token, t_token_list *result)
{
	char		*expanded;

	if (token->type == TOK_PARAM_SIMPLE)
	{
		if (expand_special(env, stringbuf_cstr(&token->word), token, result))
			return (1);
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
