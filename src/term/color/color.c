/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thschnei <thschnei@student.42perpignan.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:58:26 by thschnei          #+#    #+#             */
/*   Updated: 2025/06/19 16:45:30 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell/env/env.h"
#include <shell/shell.h>

typedef enum e_styleflag
{
	SET_BOLD = 1 << 0,
	SET_ITALIC = 1 << 1,
	SET_UNDERLINE = 1 << 2,
	SET_FG = 1 << 3,
	SET_BG = 1 << 4,
	VGACOLOR = 1 << 5,
	COLOR256 = 1 << 6,
}	t_styleflag;

void
	chk_truecolor(t_shell *shell)
{
	char	*env;

	env = get_variable_value(shell, "COLORTERM");
	if (!env)
	{
		shell->cap->flags.truecolor = shell->cap->nums.colors;
		if (!shell->cap->flags.truecolor)
			shell->cap->flags.truecolor = -1;
	}
	else
	{
		if (ft_strcmp(env, "truecolor") || ft_strcmp(env, "24bit"))
			shell->cap->flags.truecolor = 't';
	}
}

void
	set_style(t_shell *shell, t_styleflag flags)
{
	if (!shell->cap->flags.truecolor)
		chk_truecolor(shell);
	if (shell->cap->flags.truecolor == 256)
		convert_256();
	else if (shell->cap->flags.truecolor == 't')
		convert_tc();
	else
		convert_vga();
}
