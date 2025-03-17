/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_view.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef STRING_VIEW_H
# define STRING_VIEW_H

# include <stdlib.h>

/**
 * @brief Non-owning string slice
 */
typedef struct s_string_view
{
	const char	*s;
	size_t		len;
}	t_string_view;

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
sv_cmp(t_string_view sv, const char *token);
/**
 * @brief Returns 1 if `sv` is an ascii whitespace
 *
 * @note This function will return 0 on non-ascii unicode whitespaces
 *
 * @param sv String view
 *
 * @returns 1 if sv is a whitespace, 0 otherwise
 */
int
sv_whitespace(t_string_view sv);

#endif // STRING_VIEW_H
