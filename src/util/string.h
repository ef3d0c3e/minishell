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
#ifndef STRING_H
# define STRING_H

#include <stddef.h>
# ifndef UTIL_INTERNAL
#  error "Include <util/util.h> instead."
# endif // UTIL_INTERNAL

# include <stdlib.h>
# include <stdint.h>

/* ************************************************************************** */
/* String wrappers                                                            */
/* ************************************************************************** */

/**
 * @brief Non-owning string slice
 */
typedef struct s_string
{
	const char	*str;
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
 * @brief String buffer, for performing string operations
 */
typedef struct s_string_buffer
{
	char	*str;
	size_t	len;
	size_t	capacity;
}	t_string_buffer;

/**
 * @brief Creates a new string buffer with an initial capacity
 *
 * @param buf Buffer to initialize
 * @param initial_capacity Initial byte capacity
 */
void
stringbuf_init(t_string_buffer *buf, size_t initial_capacity);

/**
 * @brief Frees a string
 */
void
stringbuf_free(t_string_buffer *buf);

/**
 * @brief Appends to the string buffer
 */
void
stringbuf_append(t_string_buffer *buf, t_string str);

/* ************************************************************************** */
/* Unicode utilities                                                          */
/* ************************************************************************** */

/**
 * @brief Unicode iterator
 */
typedef struct u8_iterator
{
	/** @brief String iterated over */
	t_string	str;
	/** @brief Current coedpoint */
	t_string	codepoint;
	/** @brief Byte postition */
	size_t		byte_pos;
	/** @brief Byte next postition */
	size_t		byte_next;
	/** @brief Current codepoint position */
	size_t		cp_pos;
}	t_u8_iterator;

t_u8_iterator
it_new(t_string str);

/**
 * @brief Returns the next codepoint in the iterated UTF-8 string
 *
 * @param it Iterator over UTF-8 string
 * @return The next codepoint, or an empty string if the content is invalid
 */
t_string
it_next(t_u8_iterator *it);

/**
 * @brief Peeks the next codepoint in the iterator
 *
 * @param it Iterator over UTF-8 string
 * @return The next codepoint (empty string if invalid), without advancing
 * the iterator.
 */
t_string
it_peek(const t_u8_iterator *it);

/**
 * @brief Gets length of the utf-8 codepoint after this character
 *
 * This function will return `0` if the character is not a valid UTF-8 delimiter
 *
 * @param c UTF-8 start character
 *
 * @returns Length in bytes
 */
size_t
u8_length(char c);

#endif // STRING_H
