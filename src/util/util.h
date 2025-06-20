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

# ifndef SRC_LOCATION
#  define SRC_LOCATION __func__
# endif // SRC_LOCATION

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
/**
 * @brief Returns the maximum between x and y
 *
 * @param x First parameter
 * @param y Second parameter
 *
 * @returns x if `x >= y`, y otherwise
 */
size_t
max_sz(size_t x, size_t y);
/**
 * @brief Checks if `a * 10 + sign * b` will overflow
 *
 * @param a Integer value
 * @param sign The sign (-1 or +1)
 * @param b A value between 0 and 9
 *
 * @returns 1 If `a * 10 + sign * b` will overflow
 */
int
muladd_10s_overflow(int a, int sign, int b);
/**
 * @brief Converts string to integer, while checking for overflow and errors
 *
 * @param s String to convert
 * @param value Conversion result
 *
 * @returns 1 on success, 0 on error
 */
int
atoi_checked(const char *s, int *value);
/**
 * @brief Converts string to integer, while checking for overflow and errors
 *
 * This conversion will succeed if the number is valid and `s == e` after
 * reading the number.
 *
 * @param s String to convert
 * @param e String's end position
 * @param value Conversion result
 *
 * @returns 1 on success, 0 on error
 */
int
atoi_range_checked(const char *s, const char *e, int *value);

/* ************************************************************************** */
/* Misc                                                                       */
/* ************************************************************************** */

void
quicksort(void *base, size_t nmemb, size_t size,
	int (*cmp)(const void *, const void *));

#endif // UTIL_H
