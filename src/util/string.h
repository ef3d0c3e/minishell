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

# include "util/string_view.h"

/**
 * @brief Owning string
 */
typedef struct s_string
{
	char	*s;
	size_t	len;
	size_t	capacity;
}	t_string;

/**
 * @brief Creates a new empty string
 */
t_string
string_new(void);

/**
 * @brief Frees a string
 */
void
string_free(t_string *s);

/**
 * @brief Pushes `cp` to the string's end
 */
void
string_push(t_string *s, t_string_view cp);

#endif // STRING_H
