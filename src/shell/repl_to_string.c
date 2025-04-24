/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_to_string.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static void
	repl_to_string_child(t_shell *shell, char *s, int *fds)
{
	char			*prompt;
	t_string		input;
	t_token_list	list;
	t_parser		parser;

	prompt = ft_strdup(s);
	shell_parser_free(shell);
	input.str = prompt;
	input.len = ft_strlen(prompt);
	shell->prompt = prompt;
	close(fds[0]);
	if (dup2(fds[1], STDOUT_FILENO) == -1)
		shell_perror(shell, "dup2() failed", SRC_LOCATION);
	close(fds[1]);
	shell->token_list = &list;
	list = tokenizer_tokenize(input);
	if (!report_tokens(input, shell->token_list) || !shell_error_flush(shell))
		shell_fail(shell, "tokenization failed", SRC_LOCATION);
	*shell->token_list = token_expand(shell, *shell->token_list);
	if (!report_tokens(input, shell->token_list) || !shell_error_flush(shell))
		shell_fail(shell, "expansion failed", SRC_LOCATION);
	parser = parser_init(input, list);
	shell->parser = &parser;
	shell->program = parse(&parser, 0, list.size, 0);
	if (!parser_error_flush(&parser))
		shell_exit(shell, EXIT_FAILURE);
	eval(shell, shell->program);
	shell_exit(shell, shell->last_status);
}

/** @brief Reads incoming data on file descriptor `fd` to buffer `buf` */
static void
	read_incoming(t_shell *shell, int fd, t_string_buffer *buf)
{
	ssize_t	sz;

	sz = 1;
	while (sz > 0)
	{
		sz = read(fd, buf->str + buf->len, 1024);
		if (sz < 0)
			shell_perror(shell, "read() failed", SRC_LOCATION);
		buf->len += sz;
		stringbuf_reserve(buf, buf->len + 1024);
	}
}

static int
	repl_to_string_parent(
	t_shell *shell,
	t_string_buffer *buf,
	int *fds,
	pid_t pid)
{
int		status;
int		waitst;

	stringbuf_init(buf, 1024);
	waitst = 0;
	while (waitst != pid)
	{
		read_incoming(shell, fds[0], buf);
		waitst = waitpid(pid, &status, WNOHANG);
		if (waitst == -1)
			shell_perror(shell, "waitpid() failed", SRC_LOCATION);
	}
	read_incoming(shell, fds[0], buf);
	if (waitst != pid && waitpid(pid, &status, 0) == -1)
		shell_perror(shell, "waitpid() failed", SRC_LOCATION);
	shell->last_status = WEXITSTATUS(status);
	close(fds[0]);
	return (shell->last_status);
}

int
	repl_to_string(t_shell *shell, char *input, t_string_buffer *buf)
{
	pid_t	pid;
	int		fds[2];
	char	*err;

	if (pipe(fds) == -1)
	{
		ft_asprintf(&err, "pipe() failed: %m");
		shell_error(shell, err, SRC_LOCATION);
		return (-1);
	}
	pid = shell_fork(shell, SRC_LOCATION);
	if (pid == -1)
	{
		ft_asprintf(&err, "fork() failed: %m");
		shell_error(shell, err, SRC_LOCATION);
		return (-1);
	}
	if (!pid)
		repl_to_string_child(shell, input, fds);
	close(fds[1]);
	return (repl_to_string_parent(shell, buf, fds, pid));
}
