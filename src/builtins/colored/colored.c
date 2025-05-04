/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colored.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/05/03 19:03:09 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "colored.h"
#include "ft_printf.h"
#include "libopts.h"

void set_color(void *data, const char **av) {};
void set_style(void *data, const char **av) {};

void
	*init(t_app *a)
{
	static char			use[] = "colored [flags]";
	static t_opt		opts[] = {
		{"--bg", "Set background color", set_color, 1},
		{"--fg", "Set foreground color", set_color, 1},
		{"--bo", "Set bold", set_style, 0},
		{"--it", "Set italic", set_style, 0},
		{"--ul", "Set underline", set_style, 0},
	};
	const t_behavior	flags = NO_EXIT | NO_RESERVE_HELP | GET_CALLER;

	return (_set_behavior(_init_opt(use, opts, *(&opts + 1) - opts, a), flags));
}

int
	colored(t_shell *shell, int argc, char **argv)
{
	t_app	app;
	int		ret;

	(void)shell;
	app.opts = init(&app);
	ret = _parse_args(app.opts, argc, (const char **)argv);
	return (0);
}

const t_builtin
	*builtin_colored(void)
{
	static const t_builtin	builtin = (t_builtin){
		.name = "colored",
		.desc = "Outputs escape sequence according to flags",
		.run = colored,
		.init = NULL,
		.deinit = NULL,
	};

	return (&builtin);
}
