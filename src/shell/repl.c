/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util/util.h"
#include <shell/eval.h>
#include <expansion/expansion.h>

// TODO: Proper error checking
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
		shell_perror(env, EXIT_FAILURE, "dup2() failed", __func__);
	close(fds[1]);
	env->token_list = &list;
	list = tokenizer_tokenize(input);
	*env->token_list = token_expand(env, *env->token_list);
	parser = parser_init(input, list);
	env->parser = &parser;
	env->program = parse(&parser, 0, list.size);
	eval(env, env->program);
	shell_exit(env, EXIT_SUCCESS);
}

static int
	repl_to_string_parent(
	t_environ *env,
	t_string_buffer *buf,
	int *fds,
	pid_t pid)
{
	ssize_t	sz;
	int		status;
	int		waitst;

	stringbuf_init(buf, 1024);
	waitst = 0;
	sz = 0;
	while (waitst != pid)
	{
		if (!sz)
			sz = 1;
		while (sz > 0)
		{
			sz = read(fds[0], buf->str + buf->len, 1024);
			if (sz < 0)
				shell_perror(env, EXIT_FAILURE, "read() failed", __func__);
			buf->len += sz;
			stringbuf_reserve(buf, buf->len + 1024);
		}
		waitst = waitpid(pid, &status, WNOHANG);
		if (waitst == -1)
			shell_perror(env, EXIT_FAILURE, "waitpid() failed", __func__);
	}
	if (!sz)
		sz = 1;
	while (sz > 0)
	{
		sz = read(fds[0], buf->str + buf->len, 1024);
		if (sz < 0)
			shell_perror(env, EXIT_FAILURE, "read() failed", __func__);
		buf->len += sz;
		stringbuf_reserve(buf, buf->len + 1024);
	}
	close(fds[0]);
	return (status);
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
		shell_error(env, err, __func__);
		return (-1);
	}
	pid = shell_fork(env, __func__);
	if (pid == -1)
	{
		ft_asprintf(&err, "fork() failed: %m");
		shell_error(env, err, __func__);
		return (-1);
	}
	if (!pid)
		repl_to_string_child(env, input, fds);
	close(fds[1]);
	return (repl_to_string_parent(env, buf, fds, pid));
}
