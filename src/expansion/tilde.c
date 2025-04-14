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

	if (!str_cmp(str, "~"))
		return (expand_from_var(token, result, 1, rb_find(&env->env, "HOME")));
	username = stringbuf_from_range(str.str + 1, str.str + str.len);
	if (passwd_query(env, stringbuf_cstr(&username), &ent))
	{
		token->type = TOK_SINGLE_QUOTE;
		stringbuf_replace(&token->word, 0, str.len, ent.homedir);
		passwd_free(&ent);
	}
	else
	{
		token_free(token);
		token->type = TOK_ERROR;
		token->err.str = "Failed to perform tilde expansion: user not found";
		token->err.len = ft_strlen(token->err.str);
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
		return (expand_from_var(token, result, 2, rb_find(&env->env, "OLDPWD")));
	else if (!str_cmp(str, "~+"))
		return (expand_from_var(token, result, 2, rb_find(&env->env, "PWD")));
	else
		return (expand_home(env, str, token, result));
}
