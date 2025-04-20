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
# include "string/string.h"
# include "memory/memory.h"
# include "rbtree/rbtree.h"
# undef UTIL_INTERNAL

/* ************************************************************************** */
/* Utility defines                                                            */
/* ************************************************************************** */

/**
 * @brief Source location function
 */
# define __STRINGIFY(__x) #__x
# define STRINGIFY(__x) __STRINGIFY(__x)
# define SRC_LOCATION __FILE__ "#" STRINGIFY(__LINE__)
//# define SRC_LOCATION __func__

/* ************************************************************************** */
/* Arithmetic utils                                                           */
/* ************************************************************************** */

/**
 * @brief Returns the minimum between x and y
 *
 * @param x First parameter
 * @param y Second parameter
 *
 * @returns x if `x <= y`, y otherwise
 */
size_t
min_sz(size_t x, size_t y);

#endif // UTIL_H
