/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef READLINE_H
# define READLINE_H

typedef struct s_shell	t_shell;

# include <readline/readline.h>
# include <readline/history.h>

/* @brief Readline getc */
int
readline_getc(FILE *stream);

/**
 * @brief Readline setup
 */
void
readline_setup(t_shell *shell);

#endif // READLINE_H
