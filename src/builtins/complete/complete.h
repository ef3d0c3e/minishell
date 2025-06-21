/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/13 13:58:46 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef COMPLETE_H
# define COMPLETE_H

# include <util/util.h>

typedef struct s_shell	t_shell;

typedef struct s_cmd_comp_opt
{
	/** @brief Option's long name: `--` */
	char	*longname;
	/** @brief Option's short name: `-` */
	char	*shortname;
	/** @brief Option description */
	char	*description;
}	t_cmd_comp_opt;

typedef struct s_cmd_completion
{
	/** @brief Available options */
	t_cmd_comp_opt	*opts;
	/** @brief Number of opts */
	size_t			opts_size;
	/** @brief Capacity of opts */
	size_t			opts_cap;
	/** @brief Commands to inherit completion from */
	char			**source_from;
}	t_cmd_completion;

/**
 * @brief Insert a new command completion entry in the shell
 *
 * @param shell The shell session
 * @param name Name of the new completion item
 *
 * @returns The newly inserted completion
 */
t_cmd_completion
*complete_new(t_shell *shell, const char *name);
/**
 * @brief Frees a completion item
 *
 * @param ptr The completion item to free
 */
void
complete_free(void *ptr);
/**
 * @brief Adds a completion to the given command
 *
 * If the command doesn't exist, it will be created
 *
 * @param shell The shell session
 * @param cmd The command
 * @param opt The completion option to add
 */
void
complete_add(t_shell *shell, const char *cmd, t_cmd_comp_opt opt);

/** @brief Arguments for the complete builtin */
typedef struct s_complete_args
{
	void		*opts;
	t_shell		*shell;
	int			fail;
	const char	*cmd;
	const char	*shortname;
	const char	*longname;
	const char	*desc;
}	t_complete_args;

#endif // COMPLETE_H
