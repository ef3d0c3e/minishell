/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wcwidth.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef WCWIDTH_H
# define WCWIDTH_H

# include <util/util.h>

typedef struct s_shell	t_shell;

/** @brief A range of codepoints */
struct s_codepoint_range
{
	uint32_t	start;
	uint32_t	end;
};

/**
 * @brief Checks if given codepoint is wide
 *
 * @param cp The codepoint
 *
 * @returns 1 if `cp` is wide.
 */
int
codepoint_is_wide(uint32_t cp);
/**
 * @brief Checks if given codepoint is a fuser
 *
 * @param cp The codepoint
 *
 * @returns 1 if `cp` is a fuser.
 */
int
codepoint_is_fuser(uint32_t cp);
/**
 * @brief Gets the width of a single codepoint. This function is not equivalent
 * to wcwidth. It returns 0 for 0-width codepoints
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

#endif // WCWIDTH_H
