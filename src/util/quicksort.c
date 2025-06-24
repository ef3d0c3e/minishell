/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quicksort.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/24 10:24:15 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "getline/modes/modes.h"
#include <stddef.h>
#include <util/util.h>

struct s_qsort_param
{
	size_t		s;
	int			(*cmp)(const void *, const void *);
	char		*t;
};

static void
	_quicksort(const struct s_qsort_param *p, void *b, size_t n)
{
	char	*tmp;
	char	*b1;
	char	*b2;
	size_t	n1;
	size_t	n2;

	if (n <= 1)
		return ;
	n1 = n / 2;
	n2 = n - n1;
	b1 = b;
	b2 = (char *)b + (n1 * p->s);
	tmp  = p->t;
	_quicksort(p, b1, n1);
	_quicksort(p, b2, n2);
	while (n1  > 0 && n2 > 0)
	{
		if (p->cmp(b1, b2) <= 0)
		{
			tmp = (char *)ft_memcpy(tmp, b1, p->s) + p->s;
			b1 += p->s;
			--n1;
		}
		else
		{
			tmp = (char *)ft_memcpy(tmp, b2, p->s) + p->s;
			b2 += p->s;
			--n2;
		}
	}
	if (n1 > 0)
		ft_memcpy (tmp, b1, n1 * p->s);
	ft_memcpy (b, p->t, (n - n2) * p->s);
}

void
	quicksort(void *base, size_t nmemb, size_t size,
	int (*cmp)(const void *, const void *))
{
	 size_t	total_size;
	 char	*buf;
	 struct s_qsort_param qsort_param;

	 if (nmemb <= 1)
		 return ;
	 total_size = nmemb * size;
	 buf = xmalloc(total_size);
	 qsort_param.s = size;
	 qsort_param.cmp = cmp;
	 qsort_param.t = buf;
	 _quicksort(&qsort_param, base, nmemb);
	 free(buf);
}
