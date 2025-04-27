/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_keyword.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <tokenizer/tokenizer.h>

int
	token_keyword(t_token_list *list, t_u8_iterator *it)
{
	static const char	*keywords[] = {
	"[[", "]]", "[", "]", "((", "))",
	"done",
	"if", "then", "elif", "else", "fi", "time",
	"for", "in", "until", "while", "do",
	"case", "esac", "coproc", "select", "function",
	NULL};
	const char			*keyword = str_alternatives(it_substr(it, 8), keywords);

	if (list->size
			&& list->tokens[list->size - 1].type != TOK_SPACE
			&& list->tokens[list->size - 1].type != TOK_SEQUENCE)
		return (0);
	if (!keyword)
		return (0);
	token_list_push(list, TOK_KEYWORD, it->byte_pos,
		it->byte_pos + ft_strlen(keyword))->reserved_word = keyword;
	it_advance(it, ft_strlen(keyword));
	return (1);
}
