/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static void
	evaluator_child(t_ctx *ctx, int *fds)
{
	t_eval_result		result;
	static const char	*fmt = "%s: only meaningful in a 'for', 'while', or "
		"'until' loop\n";

	close(fds[0]);
	if (dup2(fds[1], STDOUT_FILENO) == -1)
		shell_perror(ctx->shell, "dup2() failed", SRC_LOCATION);
	close(fds[1]);
	result = eval(ctx->shell, ctx->program);
	ctx_free(ctx);
	if (result.type == RES_STOP)
		shell_exit(ctx->shell, 130);
	else if (result.type == RES_CONTINUE)
		ft_dprintf(2, fmt, "continue");
	else if (result.type == RES_BREAK)
		ft_dprintf(2, fmt, "break");
	shell_exit(ctx->shell, ctx->shell->last_status);
}

static void
	evaluator_parent(
	t_shell *shell,
	pid_t pid,
	int *fds,
	struct s_eval_string_result *result)
{
	int		status;
	int		waitst;

	stringbuf_init(&result->content, 1024);
	waitst = 0;
	while (waitst != pid)
	{
		read_fd_to_string(shell, fds[0], &result->content);
		waitst = waitpid(pid, &status, WNOHANG);
		if (waitst == -1 && errno != EINTR)
			shell_perror(shell, "waitpid() failed", SRC_LOCATION);
	}
	read_fd_to_string(shell, fds[0], &result->content);
	if (waitst != pid && waitpid(pid, &status, 0) == -1)
		shell_perror(shell, "waitpid() failed", SRC_LOCATION);
	shell->last_status = WEXITSTATUS(status);
	if (!WIFEXITED(status))
		result->result.type = RES_STOP;
	close(fds[0]);
}

static void
	evaluator(t_ctx *ctx, void *cookie)
{
	struct s_eval_string_result *const	result = cookie;
	pid_t								pid;
	int									fds[2];
	char								*err;

	if (pipe(fds) == -1)
	{
		ft_asprintf(&err, "pipe() failed: %m");
		shell_error(ctx->shell, err, SRC_LOCATION);
		return ;
	}
	pid = shell_fork(ctx->shell, SRC_LOCATION);
	if (pid == -1)
	{
		ft_asprintf(&err, "fork() failed: %m");
		shell_error(ctx->shell, err, SRC_LOCATION);
		return ;
	}
	if (!pid)
		evaluator_child(ctx, fds);
	close(fds[1]);
	evaluator_parent(ctx->shell, pid, fds, result);
	ctx_free(ctx);
}

struct s_eval_string_result
	ctx_eval_string(t_shell *shell, char *prompt, char *info)
{
	struct s_eval_string_result	result;
	t_ctx						ctx;

	result.result = (t_eval_result){RES_NONE, 0};
	ctx = ctx_new(shell, info);
	ctx_eval(&ctx, prompt, evaluator, &result);
	return (result);
}
