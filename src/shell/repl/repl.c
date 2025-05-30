/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

t_getline
	repl_setup(t_shell *shell)
{
	const char	*home = get_variable_value(shell, "HOME");
	char		*histfile;
	t_getline	line;

	line = getline_setup(shell);
	line.highlighter_fn = repl_highlighter;
	line.comp_provider_fn = repl_completer;
	if (home)
	{
		ft_asprintf(&histfile, "%s/.bash_history", home);
		getline_history_set_file(&line, histfile, 1);
	}
	return (line);
}
