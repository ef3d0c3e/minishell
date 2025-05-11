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
 * @brief Attempts to get the grapheme width of text `str`
 *
 * This function is to be given a string before being displayed in order to
 * get the width of the text once displayed.
 *
 * @param str Text to compute width of
 * @param expensive Perform expensive test, if the shell supports it, this
 * function will measure the actual width of the text, otherwise, heuristic
 * will be used to attempt to determine the width.
 * @param skip_ansi Attempts to skip ansi escape codes
 *
 * @returns The width of `str`
 */
size_t
grapheme_width(t_shell *shell, const char *str, int expensive, int skip_ansi);

#endif // GEOMETRY_H
