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
# include <stdint.h>

enum e_eval_flags
{
	/** @brief Ignores stdout */
	FLAG_NO_STDOUT = (1<<0),
	/** @brief Ignores stderr */
	FLAG_NO_STDERR = (1<<1),
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

typedef struct s_file_test
{
	/** @brief Source file of the test */
	const char		*file;
	/** @brief Source line of the test */
	size_t			line;
	/** @brief File to check */
	char		*path;
	/** @brief Expected content */
	const char		*content;
	/** @brief Expected length */
	size_t			size;
}	t_file_test;

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
 * @param id ID of the test
 *
 * @returns 1 On success
 */
int
test_run(t_eval_test *test, size_t id);
/**
 * @brief Runs a file test
 *
 * @param test The test
 * @param id ID of the test
 *
 * @returns 1 On success
 */
int
test_file_run(t_file_test *test, size_t id);
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
/** @brief Performs comparison and print the difference */
int
stringbuf_compare(
	const char *label,
	t_string_buffer *left,
	t_string_buffer *right);

/**
 * @brief Generates a random string from a set
 *
 * @param set Set of ascii characters to use
 * @param length Length of the generated text
 * @param seed Seed for the random algorithm
 * @return The random ascii string
 */
char
*random_ascii(const char *set, size_t length, uint32_t *seed);
/**
 * @brief Generates random byte string
 *
 * @param length Length of the generated text
 * @param seed Seed for the random algorithm
 * @return The random byte string
 */
char
*random_bytes(size_t length, uint32_t *seed);
/**
 * @brief Gets a random positive integer
 *
 * @param seed Seed for the random algorithm
 */
uint32_t
random_int(uint32_t *seed);

/**
 * @brief Creates temporary directory and evaluate function inside of it
 *
 * @param name Temp directory to create
 * @param fn Function to execute
 * @param cookie Data passed to function
 */
void
eval_in_tempdir(const char *name, void (*fn)(void *), void *cookie);

char
*ft_strstr(const char *haystack, const char *needle);
size_t
atosz(const char *s);
void
read_incoming(int fd, t_string_buffer *buf);

#endif // TESTER_H
