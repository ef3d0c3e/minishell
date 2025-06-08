/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef GEOMETRY_H
# define GEOMETRY_H

# include <util/util.h>

typedef struct s_shell	t_shell;

/** @brief A range of codepoints */
struct s_codepoint_range
{
	uint32_t	start;
	uint32_t	end;
};

int
codepoint_is_wide(uint32_t cp);
int
codepoint_is_fuser(uint32_t cp);

/**
 * @brief Gets the width of a single codepoint, wcwidth implementation based on
 * this one: https://www.cl.cam.ac.uk/~mgk25/ucs/wcwidth.c
 *
 * The computed width cannot be guaranteed to match the width once printed to a
 * terminal. Terminals may handle glyphs differently (grapheme, combining
 * chacaters, ligatures etc..).
 *
 * @param cp The codepoint to get the width of
 *
 * @returns The width of cp
 */
int
codepoint_width(uint32_t cp);
/**
 * @brief Repeatedly apply `codepoint_width` over string `str`
 *
 * @param str String to measure the width of
 *
 * @returns The width of str
 */
int
string_width(const char *str);
/**
 * @brief Gets byte position in `str` after which the width of `str` is larger
 * than `max`
 *
 * @param str String to truncate
 * @param max Maximum width to display
 *
 * @returns Byte position in str to truncate, or SIZE_MAX
 */
size_t
string_width_truncate(const char *str, int max);

#endif // GEOMETRY_H
