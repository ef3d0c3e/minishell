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
	t_getline	line;

	line = getline_setup(shell);
	line.highlighter_fn = repl_highlighter;
	line.comp_provider_fn = repl_completer;
	getline_history_set_file(&line, ft_strdup("/home/user/.bash_history"), 1);
	return (line);
}
