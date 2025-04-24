/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_redir.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EVAL_REDIR_H
# define EVAL_REDIR_H

# include <shell/eval.h>

/** @brief A redirected file descriptor */
typedef struct s_redir_fd
{
	/** @brief The used file descriptor */
	int	fd;
	/** @brief Original fd (saved only if `fd != original_fd`) */
	int	original_fd;
}	t_redir_fd;


/** @brief Stores data to unwind redirections */
typedef struct s_redirs_stack
{
	t_redir_fd	*fds;
	size_t		size;
	size_t		capacity;
}	t_redirs_stack;

/**
 * @brief `dup2` wrapper for redirections
 *
 * This function will save file descriptors whose value is smaller than 10.
 * All `dup`ed file descriptors are saved on the unwind stack
 *
 * @param env The shell session
 * @param stack The redirection unwind stack
 * @param fd1 Oldfd
 * @param fd2 Newfd
 *
 * @returns The new file descriptor. On failures -1 is returned and an error is
 * reported
 */
int
redir_dup(t_environ *env, t_redirs_stack *stack, int fd1, int fd2);
/**
 * @brief Opens a file for redirection
 *
 * @param env The shell session
 * @param redir The filename redirection
 *
 * @returns The opened file descriptor. On errors -1 is returned and an error
 * is reported.
 */
int
redir_open(t_environ *env, t_redirection *redir);
/**
 * @brief Handles redirection internally
 *
 * @param env The shell session
 * @param stack The redirection unwind stack
 * @param redir The redirection
 */
void
redir_internal(t_environ *env, t_redirs_stack *stack, t_redirection *redir);
/**
 * @brief Initializes the redirection unwind stack
 *
 * @param stack Stack to initialize
 */
void
redir_stack_init(t_redirs_stack *stack);

/**
 * @brief Performs redirection from a redirection list
 *
 * @param env The shell session
 * @param stack The redirection unwind stack
 * @param redir The redirections to perform
 */
void
do_redir(t_environ *env, t_redirs_stack *stack, t_redirections *redirs);
/**
 * @brief Undoes redirections from the redirection stack
 *
 * This function will `free` the stack after performing unwinding on all
 * redirected file descriptor.
 *
 * @param env The shell session
 * @param stack The redirection unwind stack
 */
void
undo_redir(t_environ *env, t_redirs_stack *stack);

#endif // EVAL_REDIR_H
