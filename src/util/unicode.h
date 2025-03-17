/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unicode.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef UNICODE_H
# define UNICODE_H

# include "string_view.h"

/**
 * @brief Utf-8 iterator
 */
typedef struct s_u8_iterator
{
	/**
	 * @brief Iterated string
	 */
	t_string_view	sv;
	/**
	 * @brief Byte position in the string
	 */
	size_t			byte_pos;
	/**
	 * @brief Position in codepoints
	 */
	size_t			cp_pos;
	/**
	 * @brief Length in bytes of codepoint under cursor
	 *
	 * > 0: Actual length
	 * = 0: End of string
	 * < 0: Malformed utf-8
	 */
	ssize_t			cp_len;
}	t_u8_iterator;

/**
 * @brief Constructs unicode iterator from NULL-terminated string
 *
 * @param s NULL-terminated string
 */
t_u8_iterator
iterator_new(t_string_view sv);

/**
 * @brief Advances the iterator, returning the current codepoint
 */
t_string_view
iterator_next(t_u8_iterator *it);

/**
 * @brief Gets byte length of the current codepoint in the string
 *
 * @param s Current utf-8 iterator
 */
ssize_t	u8_codelen(const char *s);


#endif // UNICODE_H
