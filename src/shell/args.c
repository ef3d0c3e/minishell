/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

/** @brief Prints the shell's version */
static void
	print_version(void *ptr, const char **av)
{
	(void)ptr;
	(void)av;
	ft_dprintf(2, "Minishell -- A simple C shell\n"
		"Copyright (c) 2025 Lino Gamba and Thomas Schneider\n"
		"Minishell is licensed under the GNU General Public License version 3"
		" (GPLv3),\nunder the terms of the Free Software Foundation"
		" <https://www.gnu.org/licenses/agpl-3.0.en.html>.\n\n"
		"Minishell Version: 0.1\n");
	exit(0);
}

/** @brief Set headless command */
static void
	headless_mode(void *ptr, const char **av)
{
	t_program_opts *const	opts = ptr;

	opts->headless = av[1];
}

/** @brief Start shell in clean mode */
static void
	clean_mode(void *ptr, const char **av)
{
	t_program_opts *const	opts = ptr;

	(void)av;
	opts->clean_mode = 1;
}

void
	*shell_args_init(void *handle)
{
	static char			use[] = "minishell [OPTIONS...]";
	static t_opt		opts[] = {
	{"-c", "Evaluate argument in headless mode", headless_mode, 1},
	{"-z", "Start in clean mode (no profile)", clean_mode, 0},
	{"-v", "Displays version", print_version, 0},
	};
	const t_behavior	flags = GET_CALLER;

	return (_set_behavior(_init_opt(use, opts, *(&opts + 1) - opts, handle),
			flags));
}
