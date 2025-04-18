/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef BUILTIN_H
# define BUILTIN_H

typedef struct s_environ t_environ;
typedef int(*t_builtin_fn)(t_environ *env, int argc, char **argv);

typedef struct s_builtin
{
	const char		*name;
	const char		*desc;
	t_builtin_fn	fn;
}	t_builtin;

/**
 * @brief Registers default builtins for the shell
 *
 * @param env The shell session
 */
void
init_builtin(t_environ *env);

/**
 * @brief The echo builtin
 */
const t_builtin
*builtin_echo(void);


#endif // BUILTIN_H
