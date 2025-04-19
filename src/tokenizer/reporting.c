/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reporting.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <tokenizer/tokenizer.h>

static void
	report_token(t_string input, const t_token *token)
{
	size_t	i;

	ft_dprintf(2, "[ Token error ]\n%.*s%s%.*s%s%.*s\n%",
		(int)(token->start), input.str,
		"\033[0;31m",
		(int)(token->end - token->start), input.str + token->start,
		"\033[0m",
		(int)(input.len - token->end), input.str + token->end
	);
	i = 0;
	while (i <= token->start)
	{
		ft_dprintf(2, "%c", "~^"[i >= token->start]);
		++i;
	}
	ft_dprintf(2, " %.*s\n", (int)token->err.len, token->err.str);
}

int
	report_tokens(t_string input, const t_token_list *list)
{
	size_t	i;
	int		has_error;

	i = 0;
	has_error = 0;
	while(i < list->size)
	{
		if (list->tokens[i].type == TOK_ERROR)
		{
			has_error = 1;
			report_token(input, &list->tokens[i]);
		}
		++i;
	}
	return (!has_error);
}
