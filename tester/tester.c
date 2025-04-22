/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tester.h"
#include <stdlib.h>

void
	run_test_child(t_eval_test *test, int *fds)
{
	t_environ	env;

	close(fds[1]);
	close(fds[2]);
	close(fds[4]);
	dup2(fds[3], STDOUT_FILENO);
	close(fds[3]);
	dup2(fds[5], STDERR_FILENO);
	close(fds[5]);
	dup2(fds[0], STDIN_FILENO);
	close(fds[0]);
	env = env_new(test->envp);
	repl(&env, ft_strdup(stringbuf_cstr(&test->expr)));
	test_free(test);
	shell_exit(&env, env.last_status);
}

static void
	read_incoming(int fd, t_string_buffer *buf)
{
	ssize_t	sz;

	sz = 1;
	while (sz > 0)
	{
		sz = read(fd, buf->str + buf->len, 1024);
		if (sz < 0)
			perror("read fail");
		buf->len += sz;
		stringbuf_reserve(buf, buf->len + 1024);
	}
}

static int
	run_test_parent(t_eval_test *test, pid_t pid, int *fds)
{
	int				waitst;
	int				status;
	t_string_buffer	stdout;
	t_string_buffer	stderr;

	stringbuf_init(&stdout, 1024);
	stringbuf_init(&stderr, 1024);
	(void)(close(fds[0]) || close(fds[3]) || close(fds[5]));
	write(fds[1], test->stdin.str, test->stdin.len);
	close(fds[1]);
	waitst = 0;
	while (waitst != pid)
	{
		read_incoming(fds[2], &stdout);
		read_incoming(fds[4], &stderr);
		waitst = waitpid(pid, &status, WNOHANG);
		if (waitst == -1)
			perror("waitpid() failed");
	}
	read_incoming(fds[2], &stdout);
	read_incoming(fds[4], &stderr);
	(void)(close(fds[2]) || close(fds[4]));
	if (waitst != pid)
		waitpid(pid, &status, 0);
	return (test_check(test, WEXITSTATUS(status), &stdout, &stderr));
}

int
	test_run(t_eval_test *test)
{
	pid_t	pid;
	int		fds[6];

	pipe(fds);
	pipe(fds + 2);
	pipe(fds + 4);
	pid = fork();
	if (!pid)
		run_test_child(test, fds);
	return (run_test_parent(test, pid, fds));
}

void
	test_free(t_eval_test *test)
{
	size_t	i;

	stringbuf_free(&test->expr);
	stringbuf_free(&test->stdin);
	stringbuf_free(&test->stdout);
	stringbuf_free(&test->stderr);
	i = 0;
	while (test->envp[i])
		free(test->envp[i++]);
	free(test->envp);
}
