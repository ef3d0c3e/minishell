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
#include "ft_printf.h"
#include "shell/eval.h"
#include "util/util.h"
#include <expansion/expansion.h>
#include <stddef.h>

/** @brief Expands from the content of a variable */
static inline int
	expand_from_var(
	t_environ *env,
	t_token *token,
	t_token_list *result,
	const char	*varname)
{
	const size_t	len = ft_strlen(varname);
	char			*expanded;
	char			*err;

	expanded = rb_find(&env->env, varname);
	if (expanded)
	{
		token->type = TOK_SINGLE_QUOTE;
		stringbuf_replace(&token->word, 0, len, expanded);
	}
	else
	{
		ft_asprintf(&err, "Failed to perform tilde expansion: variable `%s`"
			" not set", varname);
		shell_error(env, err, __FUNCTION__);
	}
	token_list_push(result, *token);
	return (expanded != 0);
}

/** @brief Expands using a user's home directory */
static inline int
	expand_home(
	t_environ *env,
	t_string str,
	t_token *token,
	t_token_list *result)
{
	struct s_passwd_ent	ent;
	t_string_buffer		username;
	char				*err;

	if (!str_cmp(str, "~"))
		return (expand_from_var(env, token, result, "HOME"));
	username = stringbuf_from_range(str.str + 1, str.str + str.len);
	if (passwd_query(env, stringbuf_cstr(&username), &ent))
	{
		token->type = TOK_SINGLE_QUOTE;
		stringbuf_replace(&token->word, 0, str.len, ent.homedir);
		passwd_free(&ent);
	}
	else
	{
		ft_asprintf(&err, "Failed to perform tilde expansion: user `%.*s`"
			" not found (in /etc/passwd)", (int)username.len, username.str);
		shell_error(env, err, __FUNCTION__);
	}
	token_list_push(result, *token);
	stringbuf_free(&username);
	return (1);
}

int
	expand_tilde(t_environ *env, t_token *token, t_token_list *result)
{
	t_string	str;
	size_t		end;

	if (token->type != TOK_WORD
		|| token->word.len < 1 || token->word.str[0] != '~')
		return (0);
	str = (t_string){.str = token->word.str, .len = token->word.len};
	end = min_sz(str.len, str_find(str, "/"));
	str.len = end;
	if (!str_cmp(str, "~-"))
		return (expand_from_var(env, token, result, "OLDPWD"));
	else if (!str_cmp(str, "~+"))
		return (expand_from_var(env, token, result, "PWD"));
	else
		return (expand_home(env, str, token, result));
}
