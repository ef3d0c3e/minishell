#include "ft_printf.h"
#include "tester.h"
#include "util/util.h"
#include <string.h>

static void
	print_test_string(t_string_buffer *buf)
{
	static const char	*mapping[] = {
		"\\0", "^A", "^B", "^C", "^D", "^E", "^F", "\\a", "\\b", "\\t", "\\n",
		"\\v", "\\f", "\\r", "^N", "^O", "^P", "^Q", "^R", "^T", "^U", "^V",
		"^W", "^X", "^Y", "^Z", "^[", "^\\", "^]", "^^", "^_", [0x7F] = "^?"
	};
	size_t				i;
	int					printing;

	printing = 1;
	i = 0;
	while (i < buf->len)
	{
		if (buf->str[i] < ' ' || buf->str[i] == 0x7f)
		{
			if (printing)
				write(2, "\033[0;32m", 7);
			printing = 0;
		}
		else
		{
			if(!printing)
				write(2, "\033[0m", 4);
			printing = 1;
		}

		if (printing)
			write(2, &buf->str[i], 1);
		else
			write(2, mapping[(int)buf->str[i]],
				ft_strlen(mapping[(int)buf->str[i]]));
		++i;
	}
	if (!printing)
		write(2, "\033[0m", 4);
}

static int
	stringbuf_compare(
	const char *label,
	t_string_buffer *left,
	t_string_buffer *right)
{
	if (left->len != right->len)
	{
		ft_dprintf(2, "%s length does not match! (%zu != %zu)\n", label,
			left->len, right->len);
		ft_dprintf(2, "Expected: `");
		print_test_string(left);
		ft_dprintf(2, "`\nGot     : `");
		print_test_string(right);
		ft_dprintf(2, "`\n");
		return (0);
	}
	if (memcmp(left->str, right->str, left->len))
	{
		ft_dprintf(2, "%s content does not match!\n", label);
		ft_dprintf(2, "Expected: `");
		print_test_string(left);
		ft_dprintf(2, "`\nGot     : `");
		print_test_string(right);
		ft_dprintf(2, "`\n");
		return (0);
	}
	return (1);
}

int
	check_test(
	t_eval_test *test,
	int status,
	t_string_buffer *stdout,
	t_string_buffer *stderr)
{
	int	success;

	ft_dprintf(2, "[%.*s]: ", (int)test->expr.len, test->expr.str);
	success = 0;
	if (stringbuf_compare("stdout", &test->stdout, stdout)
		&& stringbuf_compare("stderr", &test->stderr, stderr))
	{
		success = 1;
		ft_dprintf(2, "OK\n");
	}
	stringbuf_free(stdout);
	stringbuf_free(stderr);
	if (test->status != status)
	{
		ft_dprintf(2, "Exit status does not match! (%d != %d)\n",
			test->status, status);
		success = 0;
	}
	return (success);
}
