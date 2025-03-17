/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef UTIL_H
# define UTIL_H

# define UTIL_INTERNAL
# include "string.h"
# include "memory.h"
# undef UTIL_INTERNAL

# include <unistd.h>

size_t
ft_strlen(const char *s);
/**
 * @brief Allocates memory by calling to malloc, `exit(1)` if malloc returns NULL
 *
 * @param size Bytes to allocate
 */
void
*xmalloc(size_t size);

#endif // UTIL_H
