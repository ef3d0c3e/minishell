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

typedef struct s_redir_fd
{
	int	fd;
	int	original_fd;
}	t_redir_fd;

/** @brief Stores data to unwind redirections */
typedef struct s_redirs_stack
{
	t_redir_fd	*fds;
	size_t		size;
	size_t		capacity;
}	t_redirs_stack;

void
redir_stack_init(t_redirs_stack *stack);

int
redir_dup(t_environ *env, t_redirs_stack *stack, int fd1, int fd2);

void
do_redir(t_environ *env, t_redirs_stack *stack, t_redirections *redirs);
void
undo_redir(t_environ *env, t_redirs_stack *stack);

#endif // EVAL_REDIR_H
