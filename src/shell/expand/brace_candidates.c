/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brace_candidates.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	brace_candidate_free(t_brace_candidate *cand, int root)
{
	size_t	i;

	word_free(&cand->prefix);
	i = 0;
	while (i < cand->nalternatives)
		brace_candidate_free(&cand->alternatives[i++], 1);
	free(cand->alternatives);
	if (cand->next)
		brace_candidate_free(cand->next, 0);
	if (!root)
		free(cand);
}

void
	brace_candidate_print(size_t depth, const t_brace_candidate *cand)
{
	size_t	i;

	print_pad(" | ", depth);
	ft_dprintf(2, "PREFIX ");
	if (cand->prefix.natoms)
		word_print(0, &cand->prefix);
	else
		ft_dprintf(2, "\n");
	i = 0;
	while (i < cand->nalternatives)
	{
		print_pad(" | ", depth);
		ft_dprintf(2, "ALTERNATIVES %zu\n", i);
		brace_candidate_print(depth + 1, &cand->alternatives[i]);
		++i;
	}
	if (cand->next)
	{
		print_pad(" | ", depth);
		ft_dprintf(2, "NEXT\n");
		brace_candidate_print(depth + 1, cand->next);
	}
}
