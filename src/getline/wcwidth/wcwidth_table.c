/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wcwidth_table.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

/** @brief Utility function to perform binary search on the codepoint table */
static int
	bfind(uint32_t cp, const struct s_codepoint_range *table, size_t max)
{
	size_t	min;
	size_t	mid;

	min = 0;
	if (cp < table[0].start || cp > table[max].end)
		return (0);
	while (max >= min)
	{
		mid = (min + max) / 2;
		if (cp > table[mid].end)
			min = mid + 1;
		else if (cp < table[mid].start)
			max = mid - 1;
		else
			return (1);
	}
	return (0);
}

void	wcwidth_table_wide1(struct s_codepoint_range *r);
void	wcwidth_table_wide2(struct s_codepoint_range *r);
void	wcwidth_table_wide3(struct s_codepoint_range *r);
void	wcwidth_table_wide4(struct s_codepoint_range *r);
void	wcwidth_table_wide5(struct s_codepoint_range *r);

int
	codepoint_is_wide(uint32_t cp)
{
	static int						init;
	static struct s_codepoint_range	ranges[122];

	if (!init)
	{
		wcwidth_table_wide1(ranges);
		wcwidth_table_wide2(ranges);
		wcwidth_table_wide3(ranges);
		wcwidth_table_wide4(ranges);
		wcwidth_table_wide5(ranges);
		init = 1;
	}

	return (bfind(cp, ranges, 121));
}

void	wcwidth_table_fusers1(struct s_codepoint_range *r);
void	wcwidth_table_fusers2(struct s_codepoint_range *r);
void	wcwidth_table_fusers3(struct s_codepoint_range *r);
void	wcwidth_table_fusers4(struct s_codepoint_range *r);
void	wcwidth_table_fusers5(struct s_codepoint_range *r);
void	wcwidth_table_fusers6(struct s_codepoint_range *r);
void	wcwidth_table_fusers7(struct s_codepoint_range *r);
void	wcwidth_table_fusers8(struct s_codepoint_range *r);
void	wcwidth_table_fusers9(struct s_codepoint_range *r);
void	wcwidth_table_fusers10(struct s_codepoint_range *r);
void	wcwidth_table_fusers11(struct s_codepoint_range *r);
void	wcwidth_table_fusers12(struct s_codepoint_range *r);
void	wcwidth_table_fusers13(struct s_codepoint_range *r);
void	wcwidth_table_fusers14(struct s_codepoint_range *r);
void	wcwidth_table_fusers15(struct s_codepoint_range *r);

int
	codepoint_is_fuser(uint32_t cp)
{
	static int						init;
	static struct s_codepoint_range	ranges[368];

	if (!init)
	{
		wcwidth_table_fusers1(ranges);
		wcwidth_table_fusers2(ranges);
		wcwidth_table_fusers3(ranges);
		wcwidth_table_fusers4(ranges);
		wcwidth_table_fusers5(ranges);
		wcwidth_table_fusers6(ranges);
		wcwidth_table_fusers7(ranges);
		wcwidth_table_fusers8(ranges);
		wcwidth_table_fusers9(ranges);
		wcwidth_table_fusers10(ranges);
		wcwidth_table_fusers11(ranges);
		wcwidth_table_fusers12(ranges);
		wcwidth_table_fusers13(ranges);
		wcwidth_table_fusers14(ranges);
		wcwidth_table_fusers15(ranges);
		init = 1;
	}

	return (bfind(cp, ranges, 367));
}

int
	codepoint_width(uint32_t cp)
{
	if (cp == 0)
		return (0);
	else if (cp < 32 || (cp >= 0x7f && cp < 0xa0))
		return (-1);
	if (codepoint_is_wide(cp))
		return (2);
	if (codepoint_is_fuser(cp))
		return (0);
	return (1);
}
