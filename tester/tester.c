#include "tester.h"
#include "shell/eval.h"
#include "shell/opts.h"
#include "util/util.h"
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

static void
	test_free(t_eval_test *test)
{
	size_t	i;

	stringbuf_free(&test->stdin);
	stringbuf_free(&test->stdout);
	stringbuf_free(&test->stderr);
	i = 0;
	while (test->envp[i])
		free(test->envp[i++]);
	//free(test->envp);
}

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
	repl(&env, stringbuf_cstr(&test->expr));
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

int
	check_test(
	t_eval_test *test,
	int status,
	t_string_buffer *stdout,
	t_string_buffer *stderr);

void
	run_test_parent(t_eval_test *test, pid_t pid, int *fds)
{
	int				waitst;
	int				status;
	t_string_buffer	stdout;
	t_string_buffer	stderr;

	stringbuf_init(&stdout, 1024);
	stringbuf_init(&stderr, 1024);
	close(fds[0]);
	close(fds[3]);
	close(fds[5]);
	if (test->stdin.len)
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
	close(fds[2]);
	close(fds[4]);
	if (waitst != pid)
		waitpid(pid, &status, 0);
	check_test(test, status, &stdout, &stderr);
}


void
	run_test(t_eval_test *test)
{
	pid_t	pid;
	int		fds[6];

	pipe(fds);
	pipe(fds + 2);
	pipe(fds + 4);
	pid = fork();
	if (pid)
		run_test_parent(test, pid, fds);
	else
		run_test_child(test, fds);
}

int main(void)
{
	t_eval_test	test = {
		.expr = stringbuf_from("/usr/bin/cat -"),
		.stdin = stringbuf_from("HELLO"),
		.stdout = stringbuf_from("HELLO"),
		.stderr = stringbuf_from(""),
		.envp = (char*[]){NULL},
		.status = 0,
	};
	run_test(&test);
	return (0);
}
