/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tokenizer.h"
#include "util/util.h"
#include <stddef.h>
#include <stdio.h>

/*
static inline int	token_isspace(t_string codepoint)
{
	return codepoint.str[0] == ' ';
}

static inline int	token_newline(t_token_list *list, t_u8_iterator *it)
{
	if (it->codepoint.str[0] != '\n')
		return (0);
	token_list_push(list, (t_token){.token = TOK_NEWLINE, .str = it->codepoint,
			.pos = it->byte_pos});
	it_advance(it, 1);
	return (1);
}

static inline int	token_comment(t_token_list *list, t_u8_iterator *it)
{
	t_string	cp;
	size_t		start;

	if (it->codepoint.str[0] != '#')
		return (0);
	start = it->byte_pos;
	cp = it_next(it);
	while (cp.len && cp.str[0] != '\n')
		cp = it_next(it);
	token_list_push(list, (t_token){
		.token = TOK_COMMENT,
		.str = (t_string){.str = it->str.str + start,
							.len = it->byte_pos - start},
		.pos = start
	});
	return (1);
}

static int	token_substitution(t_token_list *list, t_u8_iterator *it)
{
	t_string	next;
	if (it->codepoint.str[0] != '<' && it->codepoint.str[0] != '>')
		return (0);
	next = it_substr(it, 2);
	if (!next.len || next.str[1] != '(')
		return (0);
	// TODO
	return (1);
}

static int token_heredoc(t_token_list *list, t_u8_iterator *it)
{
	if (str_cmp(it_substr(it, 3), "<<") && str_cmp(it_substr(it, 3), "<<-"))
		return (0);
	// TODO
	return (1);
}

static int token_herestring(t_token_list *list, t_u8_iterator *it)
{
	if (str_cmp(it_substr(it, 3), "<<<"))
		return (0);
	// TODO
	return (1);
}

static inline int	token_redir(t_token_list *list, t_u8_iterator *it)
{
	static const char	*redirs[] = {"2>&1", "<<<", "<<-", ">>", "<<", ">&",
		"<&", ">|",  ">", "<", NULL};
	const char			*redir;

	if (it->codepoint.str[0] != '2' && it->codepoint.str[0] != '<'
		&& it->codepoint.str[0] != '>')
		return (0);
	redir = str_alternatives(it_substr(it, 4), redirs);
	if (!redir)
		return (0);
	token_list_push(list, (t_token){
		.token = TOK_REDIR,
		.str = it_substr(it, ft_strlen(redir)),
		.pos = it->byte_pos,
	});
	it_advance(it, ft_strlen(redir));
	return (1);
}

static inline int	token_operator(t_token_list *list, t_u8_iterator *it)
{
	static const char	*operators[] = {"&&", "||", ";", "&", "|", NULL};
	const char			*operator;

	if (it->codepoint.str[0] != '&' && it->codepoint.str[0] != '|'
		&& it->codepoint.str[0] != ';')
		return (0);
	operator = str_alternatives(it_substr(it, 2), operators);
	if (!operator)
		return (0);
	token_list_push(list, (t_token){
		.token = TOK_OPERATOR,
		.str = it_substr(it, ft_strlen(operator)),
		.pos = it->byte_pos,
	});
	it_advance(it, ft_strlen(operator));
	return (1);
}

static inline int	read_matching(t_u8_iterator *it, const char *delim)
{
	t_string		next;
	const size_t	delim_len = ft_strlen(delim);

	next = it->codepoint;
	while (next.len)
	{
		if (!str_cmp(it_substr(it, delim_len), delim))
			return (1);
		if (next.str[0] == '(' && !read_matching(it, ")"))
			return (0);
		else if (next.str[0] == '{' && !read_matching(it, "}"))
			return (0);
		next = it_next(it);
	}
	return (0);
}

static inline t_string	read_identifier(t_u8_iterator *it)
{
	const size_t	start = it->byte_pos;
	t_string		next;

	next = it_next(it);
	while (next.len == 1)
	{
		if (next.str[0] != '_'
			&& !(next.str[0] >= 'A' && next.str[0] <= 'Z')
			&& !(next.str[0] >= 'a' && next.str[0] <= 'z'))
			break ;
		next = it_next(it);
	}
	return ((t_string){.str = it->str.str + start, .len = it->byte_pos - start});
}

static inline int	token_dollar(t_token_list *list, t_u8_iterator *it)
{
	const size_t	start = it->byte_pos;
	t_string		next;
	t_string		content;

	if (it->codepoint.str[0] != '$')
		return (0);
	next = it_substr(it, 3);
	if (next.len <= 1)
		return (it_advance(it, 1), error_token(list, it->byte_pos, "Unexpected EOF after '$'"), 1);
	if (str_starts_with(next, "$(("))
	{
		it_advance(it, 3);
		if (!read_matching(it, "))"))
		{
			error_token(list, start + 1, "Unclosed '((' delimiter");
			error_token(list, it->byte_pos, "Missing '))' delimiter");
			return (1);
		}
		token_list_push(list, (t_token){
			.token = TOK_ARITH,
			.str = {.str = it->str.str + start + 3, .len = it->byte_pos - start - 3},
			.pos = start,
		});
		it_advance(it, 2);
	}
	else if (str_starts_with(next, "$("))
	{
		it_advance(it, 2);
		if (!read_matching(it, ")"))
		{
			error_token(list, start + 1, "Unclosed '(' delimiter");
			error_token(list, it->byte_pos, "Missing ')' delimiter");
			return (1);
		}
		token_list_push(list, (t_token){
			.token = TOK_EVAL_PAR,
			.str = {.str = it->str.str + start + 2, .len = it->byte_pos - start - 2},
			.pos = it->byte_pos,
		});
		it_advance(it, 1);
	}
	else if (str_starts_with(next, "${"))
	{
		it_advance(it, 2);
		printf("HERE\n");
		if (!read_matching(it, "}"))
		{
			error_token(list, start + 1, "Unclosed '{' delimiter");
			error_token(list, it->byte_pos, "Missing '}' delimiter");
			return (1);
		}
		token_list_push(list, (t_token){
			.token = TOK_PARAM,
			.str = {.str = it->str.str + start + 2, .len = it->byte_pos - start - 2},
			.pos = it->byte_pos,
		});
		it_advance(it, 1);
	}
	else
	{
		content = read_identifier(it);
		if (!content.len)
		{
			error_token(list, start + 1, "Invalid variable expansion");
			return (1);
		}
		token_list_push(list, (t_token){
			.token = TOK_IDENT,
			.str = content,
			.pos = it->byte_pos,
		});
	}
	return (1);
}

static inline int	token_quoted(t_token_list *list, t_u8_iterator *it)
{
	const size_t	start = it->byte_pos;
	const char		quote = it->codepoint.str[0];
	t_string		next;
	t_string_buffer content;

	if (quote != '\'' && quote != '"')
		return (0);
	stringbuf_init(&content, 16);
	next = it_next(it);
	while (next.len)
	{
		if (next.str[0] == quote)
			break ;
		if (next.str[0] == '\\')
		{
			next = it_next(it);
			if (!next.len)
				return (free(content.str), error_token(list, it->byte_pos, "Incomplete escape sequence"), 1);
			stringbuf_append(&content, next);
		}
		else
			stringbuf_append(&content, next);
		next = it_next(it);
	}
	if (!next.len)
		return (free(content.str), error_token(list, start, "Unterminated quoted string"), 1);
	token_list_push(list, (t_token){
		.token = (enum e_token_type[2]){TOK_DOUBLE_QUOTED, TOK_SINGLE_QUOTED}
			[quote == '\''],
		.str = (t_string){.str = content.str, .len = content.len},
		.pos = start,
	});
	it_advance(it, 1);
	return (1);
}

static void read_word(t_token_list *list, t_u8_iterator *it)
{
	const size_t	start = it->byte_pos;
	char			ch;
	t_string_buffer	content;

	stringbuf_init(&content, 16);
	while (it->codepoint.len)
	{
		ch = it->codepoint.str[0];
		if (ch == ' ' || ch == '\n' || ch == '\t')
			break ;
		if (ch == '#' && !content.len)
			break ;
		if (ch == '\\')
		{
			it_advance(it, 1);
			if (!it->codepoint.len)
			{
				error_token(list, it->byte_pos, "Incomplete escape sequence");
				return ;
			}
		}
		else if (ch == '\'' || ch == '"' || ch == '$' || ch == '<' || ch == '>'
			|| ch == '&' || ch == '|' || ch == ';')
		{
			if (content.len)
				break;
			if (ch == '\'' || ch == '"')
			{
				token_quoted(list, it);
				return ;
			}
			else if (ch == '$')
			{
				token_dollar(list, it);
				return ;
			}
			break;
		}
		stringbuf_append(&content, it->codepoint);
		it_next(it);
	}
	token_list_push(list, (t_token){
		.token = TOK_WORD,
		.str = (t_string){.str = content.str, .len = content.len},
		.pos = start,
	});
}
*/

int	main(int argc, char **argv)
{
	/*
	static int(*tokenizers[])(t_token_list *, t_u8_iterator *) = {
		token_newline,
		token_comment,
		token_substitution,
		token_herestring,
		token_heredoc,
		token_redir,
		token_operator,
		token_dollar,
		token_quoted,
	};

	*/
	t_tokenizer	t;

	const t_string input = {
		.str = argv[1],
		.len = ft_strlen(argv[1]),
	};
	tokenizer_init(&t);
	printf("prompt=`%s`\n", argv[1]);

	t_token_list list = tokenizer_tokenize(&t, input);
	size_t i = 0;
	while (i < list.size)
	{
		token_print_debug(2, input, &list.tokens[i]);
		++i;
	}
	token_list_free(&list);
	tokenizer_free(&t);
}

int	token_space(t_token_list *list, t_u8_iterator *it);
int	token_newline(t_token_list *list, t_u8_iterator *it);
int	token_redir(t_token_list *list, t_u8_iterator *it);

void
	tokenizer_init(t_tokenizer *t)
{
	t->munchers = malloc(sizeof(t_tokenizer_fn) * 8);
	t->munchers[0] = token_space;
	t->munchers[1] = token_newline;
	t->munchers[2] = token_redir;
	t->munchers[3] = NULL;
}

void
	tokenizer_free(t_tokenizer *t)
{
	free(t->munchers);
}

t_token_list
	tokenizer_tokenize(const t_tokenizer *t, t_string prompt)
{
	t_token_list	list;
	t_u8_iterator	it;
	size_t			i;

	list.tokens = xmalloc(16 * sizeof(t_token));
	list.size = 0;
	list.capacity = 16;
	it = it_new(prompt);
	it_next(&it);
	while (it.codepoint.len)
	{
		i = 0;
		while (t->munchers[i])
		{
			if (t->munchers[i](&list, &it))
			{
				i = 0;
				break ;
			}
			++i;
		}
		if (i)
		{
		}
	}
	return (list);
}
