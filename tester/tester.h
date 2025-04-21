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

typedef struct s_eval_test
{
	/** @brief Expression to evaluate */
	t_string_buffer	expr;
	/** @brief Input stdin (empty for none) */
	t_string_buffer	stdin;
	/** @brief Expected stdout (empty for none) */
	t_string_buffer	stdout;
	/** @brief Expected stderr (empty for none) */
	t_string_buffer	stderr;
	/** @brief Environment (empty for none) */
	char			**envp;
	/** @brief Expected exit status */
	int		status;
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
