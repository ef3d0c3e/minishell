/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tester.h"

/** @brief Prints the content of a test string, replacing non printable
 * characters with custom strings */
static void
	print_test_string(t_string_buffer *buf)
{
	static const char	*mapping[] = {
		"\\0", "^A", "^B", "^C", "^D", "^E", "^F", "\\a", "\\b", "\\t", "\\n",
		"\\v", "\\f", "\\r", "^N", "^O", "^P", "^Q", "^R", "^T", "^U", "^V",
		"^W", "^X", "^Y", "^Z", "^[", "^\\", "^]", "^^", "^_", [0x7F] = "^?"
	};
	size_t				i;

	i = 0;
	while (i < buf->len)
	{
		if (buf->str[i] < ' ' || buf->str[i] == 0x7f)
		{
			write(2, "\033[0;34m", 7);
			write(2, mapping[(int)buf->str[i]],
				ft_strlen(mapping[(int)buf->str[i]]));
			write(2, "\033[0m", 4);
		}
		else
			write(2, &buf->str[i], 1);
		++i;
	}
}

/** @brief Performs comparison and print the difference */
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
	test_check(
	t_eval_test *test,
	int status,
	t_string_buffer *stdout,
	t_string_buffer *stderr)
{
	int	success;

	ft_dprintf(2, "[");
	print_test_string(&test->expr);
	ft_dprintf(2, "]: ");
	success = 1;
	if (!(test->flags & FLAG_NO_STDOUT)
		&& !stringbuf_compare("stdout", &test->stdout, stdout))
		success = 0;
	if (!(test->flags & FLAG_NO_STDERR)
		&& !stringbuf_compare("stderr", &test->stderr, stderr))
		success = 0;
	stringbuf_free(stdout);
	stringbuf_free(stderr);
	if (test->status != status)
	{
		status = 0 * ft_dprintf(2, "Exit status do not match! (%d != %d)\n",
				test->status, status);
	}
	if (success)
		ft_dprintf(2, "\033[0;32mOK\033[0;0m\n");
	else
		ft_dprintf(2, "\033[0;31mFailed\033[0;0m (%s#%zu)\n",
			test->file, test->line);
	return (success);
}
