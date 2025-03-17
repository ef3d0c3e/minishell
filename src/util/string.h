/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef UTIL_INTERNAL
# error "Include <util/util.h> instead."
#endif // UTIL_INTERNAL

#include <stdlib.h>

/* ************************************************************************** */
/* String wrappers                                                            */
/* ************************************************************************** */

/**
 * @brief Non-owning string slice
 */
typedef struct s_string
{
	const char	*s;
	size_t		len;
}	t_string;

/**
 * @brief Performs string comparisons
 *
 * @param sv String to compare with
 * @param token Token to compare to (NUL Terminated)
 *
 * @returns
 *  * =0 If equal
 *  * >0 If `sv` > `token`
 *  * <0 If `sv` < `token`
 */
int
str_cmp(t_string sv, const char *token);

/**
 * @brief Owning string
 */
typedef struct s_string_buffer
{
	char	*s;
	size_t	len;
	size_t	capacity;
}	t_string_buffer;

/**
 * @brief Creates a new empty string
 */
t_string_buffer
stringbuf_new(void);

/**
 * @brief Frees a string
 */
void
stringbuf_free(t_string_buffer *buf);

/**
 * @brief Appends to the string buffer
 */
void
stringbuf_append(t_string_buffer *buf, t_string cp);

/* ************************************************************************** */
/* Unicode utilities                                                          */
/* ************************************************************************** */


