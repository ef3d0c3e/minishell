/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "parser/parser.h"
#include "shell/expand/expand.h"
#include "tokenizer/tokenizer.h"
#include "util/util.h"
#include <shell/shell.h>

static int
	expand_from_var(
	t_shell *shell,
	t_fragment_list *list,
	struct s_arg_item *param,
	const char *varname)
{
	char			*expanded;
	char			*err;

	expanded = rb_find(&shell->reg_env, varname);
	if (expanded)
	{
		fraglist_push(list, stringbuf_from_range(expanded,
			expanded + ft_strlen(expanded)), param->flags | FL_SQUOTED);
		return (1);
	}
	if (option_value(shell, "experr"))
	{
		ft_asprintf(&err, "Failed to perform tilde expansion: variable `%s`"
			" not set", varname);
		shell_error(shell, err, SRC_LOCATION);
		return (-1);
	}
	return (0);
}

static struct s_arg_item
	split_arg(struct s_arg_item *item, size_t start)
{
	struct s_arg_item new;

	new.flags = item->flags;
	new.type = item->type;
	new.text = stringbuf_from_range(item->text.str + start, item->text.str + item->text.len);
	new.next = item->next;
	return (new);
}

static int
	expand_home(
	t_shell *shell,
	t_fragment_list *list,
	struct s_arg_item *param,
	t_string str)
{
	struct s_passwd_ent	ent;
	t_string_buffer		username;
	char				*err;

	if (!str_cmp(str, "~"))
		return (expand_from_var(shell, list, param, "HOME"));
	username = stringbuf_from_range(str.str + 1, str.str + str.len);
	if (passwd_query(shell, stringbuf_cstr(&username), &ent))
	{
		fraglist_push(list, stringbuf_from_range(ent.homedir,
			ent.homedir + ft_strlen(ent.homedir)), param->flags);
		passwd_free(&ent);
		stringbuf_free(&username);
		return (1);
	}
	if (option_value(shell, "experr"))
	{
		ft_asprintf(&err, "Failed to perform tilde expansion: user `%.*s`"
			" not found (in /etc/passwd)", (int)username.len, username.str);
		shell_error(shell, err, SRC_LOCATION);
		stringbuf_free(&username);
		return (-1);
	}
	stringbuf_free(&username);
	return (0);
}

int
	expand_tilde(
	t_shell *shell,
	t_fragment_list *list,
	struct s_arg_item *param,
	const char *ifs)
{
	t_string			str;
	size_t				end;
	struct s_arg_item	leftover;
	int					status;

	if (param->text.len == 0 || param->text.str[0] != '~') // Must be the start of a word
		return (0);
	str = (t_string){.str = param->text.str, .len = param->text.len};
	end = str_find(str, "/");
	if (end == (size_t) - 1 && param->next
		&& param->next->flags != param->flags)
		return (0);
	str.len = min_sz(end, str.len);
	status = 0;
	if (!str_cmp(str, "~-"))
		status = expand_from_var(shell, list, param, "OLDPWD");
	else if (!str_cmp(str, "~+"))
		status = expand_from_var(shell, list, param, "PWD");
	else
		status = expand_home(shell, list, param, str);
	if (status >= 0)
	{
		leftover = split_arg(param, str.len);
		expand_literal(shell, list, &leftover, ifs);
		stringbuf_free(&leftover.text);
	}
	return (status);
}
