/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cowsay.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/13 13:58:46 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef COWSAY_H
# define COWSAY_H

# include <util/util.h>

typedef struct s_shell	t_shell;

/** @brief Arguments for the cowsay builtin */
typedef struct s_cowsay_args
{
	void		*opts;
	const char	*eyes;
	const char	*mouth;
	const char	*tongue;
	const char	*udder;
}	t_cowsay_args;

/**
 * @brief Prints message using cowsay
 *
 * @param args Cowsayt parameters
 * @param msg Message to print
 */
void
cowsay_print(const t_cowsay_args *args, const char *msg);

#endif // COWSAY_H
