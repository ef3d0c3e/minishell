/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arith.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util.h"
#include <limits.h>

size_t
	min_sz(size_t x, size_t y)
{
	if (x <= y)
		return (x);
	return (y);
}

size_t
	max_sz(size_t x, size_t y)
{
	if (x >= y)
		return (x);
	return (y);
}

int
	muladd_10s_overflow(int a, int sign, int b)
{
	const int	mul10 = a * 10;
	const int	sb = sign * b;

	if (a > INT_MAX / 10 || a < INT_MIN / 10)
		return (1);
	if ((sb > 0 && mul10 > INT_MAX - sb) ||
			(sb < 0 && mul10 < INT_MIN - sb))
		return (1);
	return (0);
}

int
	atoi_checked(const char *s, int *value)
{
	int	sign;

	sign = 1;
	if (*s == '+' && ++s)
		sign = 1;
	else if (*s == '-' && ++s)
		sign = -1;
	*value = 0;
	while (*s >= '0' && *s <= '9')
	{
		if (muladd_10s_overflow(*value, sign, *s - '0'))
			return (0);
		*value = *value * 10 + sign * (*s - '0');
		++s;
	}
	return (!*s);
}

int
	atoi_range_checked(const char *s, const char *e, int *value)
{
	int	sign;

	sign = 1;
	if (s == e)
		return (0);
	if (*s == '+' && ++s)
		sign = 1;
	else if (*s == '-' && ++s)
		sign = -1;
	if (s == e)
		return (0);
	*value = 0;
	while (s != e && *s >= '0' && *s <= '9')
	{
		if (muladd_10s_overflow(*value, sign, *s - '0'))
			return (0);
		*value = *value * 10 + sign * (*s - '0');
		++s;
	}
	return (s == e);
}
