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
#include <shell/shell.h>

/** @brief Expands from the content of a variable */
static inline int
	expand_from_var(
	t_shell *shell,
	t_token *token,
	size_t len,
	const char	*varname)
{
	char			*expanded;
	char			*err;

	expanded = rb_find(&shell->reg_env, varname);
	if (expanded)
	{
		token->type = TOK_SINGLE_QUOTE;
		stringbuf_replace(&token->word, 0, len, expanded);
	}
	else
	{
		ft_asprintf(&err, "Failed to perform tilde expansion: variable `%s`"
			" not set", varname);
		shell_error(shell, err, SRC_LOCATION);
	}
	return (0);
}

/** @brief Expands using a user's home directory */
static inline int
	expand_home(
	t_shell *shell,
	t_string str,
	t_token *token,
	t_token_list *result)
{
	struct s_passwd_ent	ent;
	t_string_buffer		username;
	char				*err;

	if (!str_cmp(str, "~"))
		return (expand_from_var(shell, token, 1, "HOME"));
	username = stringbuf_from_range(str.str + 1, str.str + str.len);
	if (passwd_query(shell, stringbuf_cstr(&username), &ent))
	{
		token->type = TOK_SINGLE_QUOTE;
		stringbuf_replace(&token->word, 0, str.len, ent.homedir);
		passwd_free(&ent);
	}
	else
	{
		ft_asprintf(&err, "Failed to perform tilde expansion: user `%.*s`"
			" not found (in /etc/passwd)", (int)username.len, username.str);
		shell_error(shell, err, SRC_LOCATION);
	}
	token_list_push_token(result, *token);
	stringbuf_free(&username);
	return (1);
}

int
	_expand_tilde(t_shell *shell, t_token *token, t_token_list *result)
{
	t_string	str;
	size_t		end;

	if (token->type != TOK_WORD
		|| token->word.len < 1 || token->word.str[0] != '~')
		return (0);
	if (result->size != 0
		&& token_isword(result->tokens[result->size - 1].type))
		return (0);
	str = (t_string){.str = token->word.str, .len = token->word.len};
	end = min_sz(str.len, str_find(str, "/"));
	str.len = end;
	if (!str_cmp(str, "~-"))
		return (expand_from_var(shell, token, 2, "OLDPWD"));
	else if (!str_cmp(str, "~+"))
		return (expand_from_var(shell, token, 2, "PWD"));
	else
		return (expand_home(shell, str, token, result));
}
