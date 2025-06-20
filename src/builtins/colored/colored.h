/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colored.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thschnei <thschnei@student.42perpignan.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:52:09 by thschnei          #+#    #+#             */
/*   Updated: 2025/05/05 11:10:51 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORED_H
# define COLORED_H
# include <shell/shell.h>
# include <libopts.h>

typedef enum e_flag
{
	SET_BOLD,
	SET_ITALIC,
	SET_UNDERLINE,
	SET_OVERSTRIKE,
	SET_FG,
	SET_BG,
}	t_flag;

typedef struct s_color
{
	int		r;
	int		g;
	int		b;
	int		err;
	char	*litteral;
}	t_color;

typedef struct s_app
{
	void	*opts;
	t_flag	flags;
	t_color	fg;
	t_color	bg;
	char	*out;
}	t_app;

#endif
