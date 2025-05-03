/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colored.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thschnei <thschnei@student.42perpignan.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:52:09 by thschnei          #+#    #+#             */
/*   Updated: 2025/05/03 13:32:31 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORED_H
# define COLORED_H
# include <shell/shell.h>
# include <libopts.h>

typedef struct s_app
{
	int	truecolor;
	int	bold;
	int	italic;

}	t_app;

#endif
