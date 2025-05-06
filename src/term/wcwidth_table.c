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

void	mk_wcwidth1(struct s_codepoint_range *r);
void	mk_wcwidth2(struct s_codepoint_range *r);
void	mk_wcwidth3(struct s_codepoint_range *r);
void	mk_wcwidth4(struct s_codepoint_range *r);
void	mk_wcwidth5(struct s_codepoint_range *r);

static void
	mk_wcwidth6(struct s_codepoint_range *r)
{
	r[125] = (struct s_codepoint_range){0xFE00, 0xFE0F};
	r[126] = (struct s_codepoint_range){0xFE20, 0xFE23};
	r[127] = (struct s_codepoint_range){0xFEFF, 0xFEFF};
	r[128] = (struct s_codepoint_range){0xFFF9, 0xFFFB};
	r[129] = (struct s_codepoint_range){0x10A01, 0x10A03};
	r[130] = (struct s_codepoint_range){0x10A05, 0x10A06};
	r[131] = (struct s_codepoint_range){0x10A0C, 0x10A0F};
	r[132] = (struct s_codepoint_range){0x10A38, 0x10A3A};
	r[133] = (struct s_codepoint_range){0x10A3F, 0x10A3F};
	r[134] = (struct s_codepoint_range){0x1D167, 0x1D169};
	r[135] = (struct s_codepoint_range){0x1D173, 0x1D182};
	r[136] = (struct s_codepoint_range){0x1D185, 0x1D18B};
	r[137] = (struct s_codepoint_range){0x1D1AA, 0x1D1AD};
	r[138] = (struct s_codepoint_range){0x1D242, 0x1D244};
	r[139] = (struct s_codepoint_range){0xE0001, 0xE0001};
	r[140] = (struct s_codepoint_range){0xE0020, 0xE007F};
	r[141] = (struct s_codepoint_range){0xE0100, 0xE01EF};
}

static struct s_codepoint_range*
	mk_wcwidth(void)
{
	static int						init;
	static struct s_codepoint_range	ranges[142];

	if (!init)
	{
		mk_wcwidth1(ranges);
		mk_wcwidth2(ranges);
		mk_wcwidth3(ranges);
		mk_wcwidth4(ranges);
		mk_wcwidth5(ranges);
		mk_wcwidth6(ranges);
		init = 1;
	}
	return (ranges);
}

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

int
	codepoint_width(const char *str)
{
	t_u8_iterator	it;
	uint32_t		cp;

	it = it_new((t_string){str, ft_strnlen(str, 5)});
	it_next(&it);
	cp = u8_to_cp(it.codepoint);
	if (cp == 0)
		return (0);
	else if (cp < 32 || (cp >= 0x7f && cp < 0xa0))
		return (-1);
	if (bfind(cp, mk_wcwidth(), 142))
		return (0);
	return (1 + (cp >= 0x1100
			&& (cp <= 0x115f || cp == 0x2329 || cp == 0x232a
				|| (cp >= 0x2e80 && cp <= 0xa4cf && cp != 0x303f)
				|| (cp >= 0xac00 && cp <= 0xd7a3)
				|| (cp >= 0xf900 && cp <= 0xfaff)
				|| (cp >= 0xfe10 && cp <= 0xfe19)
				|| (cp >= 0xfe30 && cp <= 0xfe6f)
				|| (cp >= 0xff00 && cp <= 0xff60)
				|| (cp >= 0xffe0 && cp <= 0xffe6)
				|| (cp >= 0x20000 && cp <= 0x2fffd)
				|| (cp >= 0x30000 && cp <= 0x3fffd))));
}
