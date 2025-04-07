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

# ifndef UTIL_INTERNAL
#  error "Include <util/util.h> instead."
# endif // UTIL_INTERNAL

# include "../includes.h"

/**
 * @file String utilities
 */

/* ************************************************************************** */
/* String wrappers                                                            */
/* ************************************************************************** */

/** @brief A non-owning string slice */
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

int
str_starts_with(t_string str, const char *token);

/**
 * @brief Finds if `alternatives` contains `str`
 *
 * @param str String to find alternative of
 * @param alternatives Alternatives to search for (NULL-terminated)
 *
 * @returns The member of `alternatives` equal to `str`
 */
const char
*str_alternatives(t_string str, const char **alternatives);

/** @brief String buffer, for performing string operations */
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
 * @brief Creates a new string buffer from a literal
 *
 * @param msg Literal message for the stringbuf
 *
 * @returns A `t_string_buffer` containing a copy of `msg`
 */
t_string_buffer
stringbuf_from(const char *msg);

/**
 * @brief Creates a new string buffer from an owned `char*` buffer
 *
 * @param msg String for the stringbuf
 *
 * @returns A `t_string_buffer` containing `msg`
 */
t_string_buffer
stringbuf_from_owned(char *msg);

/**
 * @brief Creates a new string buffer from a string range
 *
 * @param start Range start
 * @param end Renge end
 *
 * @returns A stringbuf constructed from `[start; end]`
 */
t_string_buffer
stringbuf_from_range(const char *start, const char *end);

/**
 * @brief Consumes a string buffer and returns a NULL-terminated C string
 *
 * @param buf Buffer to consume
 *
 * @returns The constructed NULL-terminated string from `buf`
 */
char
*stringbuf_cstr(t_string_buffer *buf);

/** @brief Frees a string */
void
stringbuf_free(t_string_buffer *buf);

/** @brief Appends to the string buffer */
void
stringbuf_append(t_string_buffer *buf, t_string str);

/**
 * @brief Creates a stringbuffer from a string
 *
 * @param buf The stringbuf to create a substring from
 * @param start Start offset in `buf`
 * @param end End offset in `buf`
 * @return A stringubffer created from [start, end] in `buf`
 */
t_string_buffer
stringbuf_substr(t_string str, size_t start, size_t end);


/**
 * @brief Replaces range in string buffer
 *
 * Replaces `[start; end]` in `buf` with the content of `replace`
 *
 * @param buf String buffer to replacer int
 * @param start Replace start range
 * @param end Replace end range
 * @param replace Content to replace with
 */
void
stringbuf_replace(
	t_string_buffer *buf,
	size_t start,
	size_t end,
	const char *replace);

/**
 * @brief Internally reserve memory to hold at least `new_capacity` bytes
 *
 * @param buf String buffer to reserve space for
 * @param new_capacity The requested byte capacity.
 * In case `buf->capacity >= new_capacity` this function does nothing
 */
void
stringbuf_reserve(t_string_buffer *buf, size_t new_capacity);

/**
 * @brief Appends integer value (base 10) to the string buffer
 *
 * @param buf String buffer to append integer to
 * @param number Integer to append to string buffer `buf`
 */
void
stringbuf_itoa(t_string_buffer *buf, int number);

/* ************************************************************************** */
/* Unicode utilities                                                          */
/* ************************************************************************** */

/** @brief Unicode iterator */
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
 *
 * @return The next codepoint, or an empty string if the content is invalid
 */
t_string
it_next(t_u8_iterator *it);

/**
 * @brief Advances the iterator by `num` bytes
 *
 * @param it Iterator over UTF-8 string
 * @param num Number of bytes to advace `it` by
 */
void
it_advance(t_u8_iterator *it, size_t num);

/**
 * @brief Reads until a delimiter
 *
 * @param it Iterator over UTF-8 string
 * @param delim Delimiter to stop reading at
 *
 *
 * @return The content until the delimiter (delimiter included)
 * An empty string is returned if the delimiter cannot be found
 */
t_string
it_until(t_u8_iterator *it, const char *delim);

/**
 * @brief Gets the iterator's leftover content
 *
 * @param it The iterator
 * @param len The maximum length
 *
 * @returns The content from `[str + it->byte_pos;
 * str + min(it->byte_pos + len, str.len)]`
 */
t_string
it_substr(const t_u8_iterator *it, size_t len);

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

/* ************************************************************************** */
/* String utilities                                                           */
/* ************************************************************************** */

/**
 * @brief Creates a string from a range
 *
 * @param s String
 * @param start Start position in string
 * @param len Length of the created string
 *
 * @returns The created NULL-terminated string
 */
char
*ft_substr(const char *s, size_t start, size_t len);
/**
 * @brief Strlen function
 *
 * @param s String to get length of
 *
 * @returns Number of bytes until a `0x0` is encountered in `s`
 */
size_t
ft_strlen(const char *s);
/**
 * @brief Strcmp function
 *
 * @param first First parameter
 * @param second Second parameter
 * @return The following value are returned by this function:
 * 	- 0 if `first == second`,
 *  - >0 if `first > second`
 *  - <0 if `first < second`
 */
int
ft_strcmp(const char *first, const char *second);
/**
 * @brief Strncmp function
 *
 * @param first First parameter
 * @param second Second parameter
 * @param number of bytes to compare
 * @return The following value are returned by this function:
 * 	- 0 if `first == second`,
 *  - >0 if `first > second`
 *  - <0 if `first < second`
 */
int
ft_strncmp(const char *first, const char *second, size_t n);
/**
 * @brief Searches for character `c` in string `s`
 *
 * @param s The string to search
 * @param c The character to search in `s`
 *
 * @returns The substring starting from the first occurence of `c` in `s`
 * NULL if `s` does not contain `c`.
 */
char
*ft_strchr(const char *s, int c);
/**
 * @brief Strdup function
 *
 * @param s The string to duplicate
 *
 * @returns A string allocated from copying s
 */
char
*ft_strdup(const char *s);

#endif // STRING_H
