/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef TESTER_H
# define TESTER_H

# include <shell/eval.h>

# include <string.h>

enum e_eval_flags
{
	/** @brief Ignores stdout */
	FLAG_NO_STDOUT = 0b0001,
	/** @brief Ignores stderr */
	FLAG_NO_STDERR = 0b0010,
};

typedef struct s_eval_test
{
	/** @brief Source file of the test */
	const char		*file;
	/** @brief Source line of the test */
	size_t			line;
	/** @brief Expression to evaluate */
	t_string_buffer	expr;
	/** @brief Input stdin (empty for none) */
	t_string_buffer	stdin;
	/** @brief Expected stdout (empty for none) */
	t_string_buffer	stdout;
	/** @brief Expected stderr (empty for none) */
	t_string_buffer	stderr;
	/** @brief Environment (empty for none) */
	const char		**envp;
	/** @brief Expected exit status */
	int				status;
	/** @brief Flags */
	int				flags;
}	t_eval_test;

/**
 * @brief Frees test-related data
 *
 * @param test The test
 */
void
test_free(t_eval_test *test);
/**
 * @brief Runs the test
 *
 * @param test The test
 *
 * @returns 1 On success
 */
int
test_run(t_eval_test *test);
/**
 * @brief Checks the test's result
 *
 * @param test The test
 * @param status The exit status
 * @param stdout Stdout buffer
 * @param stderr Stderr buffer
 *
 * @return 1 on success
 */
int
test_check(
	t_eval_test *test,
	int status,
	t_string_buffer *stdout,
	t_string_buffer *stderr);

#endif // TESTER_H
