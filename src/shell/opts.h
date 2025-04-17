/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opts.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef OPTS_H
# define OPTS_H

typedef struct s_environ t_environ;

/** @brief Represents a configuration option for the shell. Options can be
 * configured using the `set` builtin */
struct s_option
{
	/** @brief Concise option description */
	const char	*desc;
	/** @brief Value of the option */
	int			value;
};

/**
 * @brief Initializes default options for the shell
 */
void
init_options(t_environ *env);

/**
 * @brief Gets the value of an option
 *
 * This function will call to `shell_exit()` if the option doesn't exist
 *
 * @param env The shell session
 * @param name Name of the option to get
 *
 * @returns The value of the option
 */
int
option_get(t_environ *env, const char *name);

#endif // OPTS_H
