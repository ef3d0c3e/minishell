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

int
run_test(t_eval_test *test);

#endif // TESTER_H
