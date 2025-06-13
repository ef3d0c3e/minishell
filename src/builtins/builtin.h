/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/13 13:58:46 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef BUILTIN_H
# define BUILTIN_H

typedef struct s_shell t_shell;
typedef int(*t_builtin_run)(t_shell *shell, int argc, char **argv);
typedef void(*t_builtin_hook)(t_shell *shell);

/** @brief Represents a shell builtin */
typedef struct s_builtin
{
	/** @brief The builtin's command name */
	const char		*name;
	/** @brief The builtin's command description */
	const char		*desc;
	/** @brief The builtin's run function */
	t_builtin_run	run;
	/** @brief The builtin's init hook */
	t_builtin_hook	init;
	/** @brief The builtin's deinit hook */
	t_builtin_hook	deinit;
}	t_builtin;

/**
 * @brief Registers default builtins for the shell
 *
 * @param shell The shell session
 */
void
builtin_init(t_shell *shell);
/**
 * @brief Unregisters builtins for the shell
 *
 * @param shell The shell session
 */
void
builtin_deinit(t_shell *shell);

/** @brief The `echo` builtin */
const t_builtin
*builtin_echo(void);
/** @brief The `source` builtin */
const t_builtin
*builtin_source(void);
/** @brief The `fds` builtin */
const t_builtin
*builtin_fds(void);
/** @brief The `unset` builtin */
const t_builtin
*builtin_unset(void);
/** @brief The `export` builtin */
const t_builtin
*builtin_export(void);
/** @brief The `cd` builtin */
const t_builtin
*builtin_cd(void);
/** @brief The `type` builtin */
const t_builtin
*builtin_type(void);
/** @brief The `pwd` builtin */
const t_builtin
*builtin_pwd(void);
/** @brief The `env` builtin */
const t_builtin
*builtin_env(void);

#endif // BUILTIN_H
