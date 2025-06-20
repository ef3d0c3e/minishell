/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef REDIR_H
# define REDIR_H

# include <parser/parser.h>

typedef struct s_shell	t_shell;

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
 * @brief Initializes the redirection unwind stack
 *
 * @param stack Stack to initialize
 */
void
redir_stack_init(t_redirs_stack *stack);
/**
 * @brief `dup2` wrapper for redirections
 *
 * This function will save file descriptors whose value is smaller than 10.
 * All `dup`ed file descriptors are saved on the unwind stack
 *
 * @param shell The shell session
 * @param stack The redirection unwind stack
 * @param fd1 Oldfd
 * @param fd2 Newfd
 *
 * @returns The new file descriptor. On failures -1 is returned and an error is
 * reported
 */
int
redir_dup2(t_shell *shell, t_redirs_stack *stack, int fd1, int fd2);
/**
 * @brief Opens a file for redirection
 *
 * @param shell The shell session
 * @param redir The filename redirection
 *
 * @returns The opened file descriptor. On errors -1 is returned and an error
 * is reported.
 */
int
redir_open(t_shell *shell, t_redirection *redir);
/**
 * @brief Performs redirection from a redirection list
 *
 * @param shell The shell session
 * @param stack The redirection unwind stack
 * @param redir The redirections to perform
 *
 * @returns 0 on failure, 1 on success
 */
int
do_redir(t_shell *shell, t_redirs_stack *stack, t_redirections *redirs);
/**
 * @brief Undoes redirections from the redirection stack
 *
 * This function will `free` the stack after performing unwinding on all
 * redirected file descriptor.
 *
 * @param shell The shell session
 * @param stack The redirection unwind stack
 */
void
undo_redir(t_shell *shell, t_redirs_stack *stack);

/******************************************************************************/
/* Internal functions                                                         */
/******************************************************************************/

/**
 * @brief Handles redirection internally
 *
 * This functions dispatches to the correct redirection handler
 *
 * @param shell The shell session
 * @param stack The redirection unwind stack
 * @param redir The redirection to perform
 *
 * @return 0 on failure
 */
int
redir_internal(t_shell *shell, t_redirs_stack *stack, t_redirection *redir);
/**
 * @brief Handles file redirection
 *
 * @param shell The shell session
 * @param stack The redirection unwind stack
 * @param redir The file redirection to perform
 *
 * @return 0 on failure
 */
int
redir_internal_file(
	t_shell *shell,
	t_redirs_stack *stack,
	t_redirection *redir);
/**
 * @brief Handles close redirection
 *
 * @param shell The shell session
 * @param stack The redirection unwind stack
 * @param redir The close redirection to perform
 *
 * @return 0 on failure
 */
int
redir_internal_close(
	t_shell *shell,
	t_redirs_stack *stack,
	t_redirection *redir);
/**
 * @brief Handles dup/move redirection
 *
 * @param shell The shell session
 * @param stack The redirection unwind stack
 * @param redir The dup/move redirection to perform
 *
 * @return 0 on failure
 */
int
redir_internal_dup_move(
	t_shell *shell,
	t_redirs_stack *stack,
	t_redirection *redir);
/**
 * @brief Handles dup word redirection
 *
 * @param shell The shell session
 * @param stack The redirection unwind stack
 * @param redir The dup word redirection to perform
 *
 * @return 0 on failure
 */
int
redir_internal_dup_word(
	t_shell *shell,
	t_redirs_stack *stack,
	t_redirection *redir);
/**
 * @brief Handles heredoc redirection
 *
 * @param shell The shell session
 * @param stack The redirection unwind stack
 * @param redir The heredoc redirection to perform
 *
 * @return 0 on failure
 */
int
redir_internal_heredoc(
	t_shell *shell,
	t_redirs_stack *stack,
	t_redirection *redir);
/**
 * @brief Writes the entore content of [buf, buf+len) to file descriptor `fd`
 *
 * @param fd File descriptor to write to
 * @param buf Buffer to write
 * @param len Length of buf
 *
 * @returns The first write error encountered, otherwise the number of written
 * bytes
 */
ssize_t
redir_write_all(int fd, const void *buf, size_t len);

#endif // REDIR_H
