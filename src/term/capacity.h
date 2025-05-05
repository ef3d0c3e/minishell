/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   capacity.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thschnei <thschnei@student.42perpignan.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:10:01 by thschnei          #+#    #+#             */
/*   Updated: 2025/05/05 11:16:15 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAPACITY_H
#define CAPACITY_H

typedef struct s_termcap
{
	unsigned	truecolor:1;
	unsigned	bold:1;
	unsigned	italic:1;
	unsigned	underline:1;
	unsigned	overstrike:1;
}	t_termcap;

#endif
