/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_cmd_sub.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <tokenizer/tokenizer.h>

static int
	cmd_sub_dollar(t_token_list *list, t_u8_iterator *it)
{
	size_t	end;

	it_advance(it, 2);
	end = find_matching(it_substr(it, -1), "$(", ")", 1);
	if (end == (size_t)-1)
	{
		token_error(list, it->byte_pos - 2, it->byte_pos, "Unterminated `$(` token");
		return (1);
	}
	token_list_push(list, TOK_CMD_SUB, it->byte_pos,
		it->byte_pos + end)->word = stringbuf_from_range(it->str.str + it->byte_pos,
			it->str.str + it->byte_pos + end);
	it_advance(it, end + 1);
	return (1);
}

static int
	cmd_sub_backtick(t_token_list *list, t_u8_iterator *it)
{
	t_string_buffer	buf;
	size_t			end;

	it_advance(it, 1);
	end = find_unescaped(it_substr(it, -1), "`");
	if (end == (size_t)-1)
	{
		token_error(list, it->byte_pos - 1, it->byte_pos, "Unterminated '`' token");
		return (1);
	}
	buf = stringbuf_from_range(it->str.str + it->byte_pos,
			it->str.str + it->byte_pos + end);
	token_list_push(list, TOK_CMD_SUB, it->byte_pos,
		it->byte_pos + end)->word = buf;
	it_advance(it, end + 1);
	return (1);
}

int
	token_cmd_sub(t_token_list *list, t_u8_iterator *it)
{
	if (!str_cmp(it_substr(it, 2), "$("))
		return (cmd_sub_dollar(list, it));
	else if (!str_cmp(it_substr(it, 1), "`"))
		return (cmd_sub_backtick(list, it));
	return (0);
}
