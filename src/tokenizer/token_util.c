/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <tokenizer/tokenizer.h>

int
	token_isword(enum e_token_type type)
{
	return (type == TOK_WORD
		|| type == TOK_SINGLE_QUOTE
		|| type == TOK_DIGIT
		|| type == TOK_MINUS
		|| type == TOK_ASSIGN
		|| type == TOK_KEYWORD);
}

int
	token_wordcontent(t_string_buffer *buf, const t_token *tok)
{
	if (tok->type == TOK_WORD
		|| tok->type == TOK_SINGLE_QUOTE
		|| tok->type == TOK_DIGIT)
	{
		stringbuf_append(buf, (t_string){.str = tok->word.str,
				.len = tok->word.len });
		return (1);
	}
	else if (tok->type == TOK_KEYWORD || tok->type == TOK_MINUS)
	{
		stringbuf_append(buf, (t_string){.str = tok->reserved_word,
				.len = ft_strlen(tok->reserved_word) });
		return (1);
	}
	else if (tok->type == TOK_ASSIGN)
	{
		stringbuf_append(buf, (t_string){tok->word.str, tok->word.len});
		stringbuf_append(buf, (t_string){"=", 1});
		return (1);
	}
	return (0);
}

int
	token_precedence(const t_token *tok)
{
	if (tok->type == TOK_PIPELINE)
		return (2);
	else if (tok->type == TOK_OPERATOR)
		return (1);
	else if (tok->type == TOK_SEQUENCE)
		return (0);
	return (-1);
}

static void
	filter_escaped(
	t_string_buffer *result,
	size_t *escaped,
	t_u8_iterator *it,
	const char **filter)
{
	size_t	i;
	size_t	len;

	it_next(it);
	*escaped += 1;
	if (*escaped % 2 == 0)
		return ;
	i = 0;
	while (filter[i])
	{
		len = ft_strlen(filter[i]);
		if (!str_cmp(it_substr(it, len), filter[i]))
		{
			stringbuf_append_n(result, str_new("\\", 1), *escaped / 2);
			stringbuf_append(result, str_new(filter[i], len));
			*escaped = 0;
			it_advance(it, len);
			return ;
		}
		++i;
	}
}

void
	escape_filter(t_string_buffer *buf, const char **filter)
{
	t_string_buffer	result;
	size_t			escaped;
	t_u8_iterator	it;

	stringbuf_init(&result, buf->len);
	it = it_new(str_new(buf->str, buf->len));
	it_next(&it);
	escaped = 0;
	while (it.codepoint.len)
	{
		if (it.codepoint.str[0] == '\\')
			filter_escaped(&result, &escaped, &it, filter);
		else
		{
			stringbuf_append_n(&result, str_new("\\", 1), escaped / 2);
			escaped = 0;
			stringbuf_append(&result, it.codepoint);
			it_next(&it);
		}
	}
	stringbuf_free(buf);
	*buf = result;
}
