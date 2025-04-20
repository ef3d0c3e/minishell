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
#include <shell/eval.h>
#include <expansion/expansion.h>
#include <stdio.h>

static void
	repl_to_string_child(t_environ *env, char *s, int *fds)
{
	char			*prompt;
	t_string		input;
	t_token_list	list;
	t_parser		parser;

	prompt = ft_strdup(s);
	env_parser_free(env);
	input.str = prompt;
	input.len = ft_strlen(prompt);
	env->prompt = prompt;
	close(fds[0]);
	if (dup2(fds[1], STDOUT_FILENO) == -1)
		shell_perror(env, EXIT_FAILURE, "dup2() failed", SRC_LOCATION);
	close(fds[1]);
	env->token_list = &list;
	list = tokenizer_tokenize(input);
	if (!report_tokens(input, env->token_list) || !shell_error_flush(env))
		shell_perror(env, EXIT_FAILURE, "tokenization failed", SRC_LOCATION);
	*env->token_list = token_expand(env, *env->token_list);
	if (!report_tokens(input, env->token_list) || !shell_error_flush(env))
		shell_perror(env, EXIT_FAILURE, "expansion failed", SRC_LOCATION);
	parser = parser_init(input, list);
	env->parser = &parser;
	env->program = parse(&parser, 0, list.size);
	if (!shell_error_flush(env))
		shell_exit(env, EXIT_FAILURE);
	eval(env, env->program);
	shell_exit(env, env->last_status);
}

/** @brief Reads incoming data on file descriptor `fd` to buffer `buf` */
static void
	read_incoming(t_environ *env, int fd, t_string_buffer *buf)
{
	ssize_t	sz;

	sz = 1;
	while (sz > 0)
	{
		sz = read(fd, buf->str + buf->len, 1024);
		if (sz < 0)
			shell_perror(env, EXIT_FAILURE, "read() failed", SRC_LOCATION);
		buf->len += sz;
		stringbuf_reserve(buf, buf->len + 1024);
	}
}

static int
	repl_to_string_parent(
	t_environ *env,
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
		read_incoming(env, fds[0], buf);
		waitst = waitpid(pid, &status, WNOHANG);
		if (waitst == -1)
			shell_perror(env, EXIT_FAILURE, "waitpid() failed", SRC_LOCATION);
	}
	read_incoming(env, fds[0], buf);
	if (waitst != pid && waitpid(pid, &status, 0) == -1)
		shell_perror(env, EXIT_FAILURE, "waitpid() failed", SRC_LOCATION);
	env->last_status = status;
	close(fds[0]);
	return (env->last_status);
}

int
	repl_to_string(t_environ *env, char *input, t_string_buffer *buf)
{
	pid_t	pid;
	int		fds[2];
	char	*err;

	if (pipe(fds) == -1)
	{
		ft_asprintf(&err, "pipe() failed: %m");
		shell_error(env, err, SRC_LOCATION);
		return (-1);
	}
	pid = shell_fork(env, SRC_LOCATION);
	if (pid == -1)
	{
		ft_asprintf(&err, "fork() failed: %m");
		shell_error(env, err, SRC_LOCATION);
		return (-1);
	}
	if (!pid)
		repl_to_string_child(env, input, fds);
	close(fds[1]);
	return (repl_to_string_parent(env, buf, fds, pid));
}
