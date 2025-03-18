#include "token.h"
#include "util/util.h"
#include <endian.h>
#include <stddef.h>
#include <stdio.h>
#include <uchar.h>

void
token_print_debug(int fd, const t_token *token)
{
	static const char	*names[] = {
	[TOK_IDENT] = "Identifier",
	[TOK_WORD] = "Word",
	[TOK_SINGLE_QUOTED] = "Single Quoted",
	[TOK_DOUBLE_QUOTED] = "Double Quoted",
	[TOK_PARAM] = "Param",
	[TOK_COMMAND] = "Command",
	[TOK_ARITH] = "Arith",
	[TOK_REDIR] = "Redir",
	[TOK_OPERATOR] = "Operator",
	[TOK_NEWLINE] = "Newline",
	[TOK_COMMENT] = "Comment",
	[TOK_HEREDOC] = "Heredoc",
	[TOK_HERESTRING] = "Herestring",
	[TOK_EOF] = "Eof",
	[TOK_ERROR] = "Error",
	};

	dprintf(2, "(%s, %zu, `%.*s`)",
			names[token->token],
			token->pos,
			(int)token->str.len,
			token->str.str);
}

void	error_token(t_token_list *list, size_t pos, const char *msg)
{
	token_list_push(list, (t_token){
		.token = TOK_ERROR,
		.str = (t_string){.str = msg, .len = ft_strlen(msg)},
		.pos = pos,
	});
}

void
	token_list_push(t_token_list *list, t_token token)
{
	token_print_debug(2, &token);
	if (list->size >= list->capacity)
	{
		list->capacity = ((list->capacity + !list->capacity) * 2);
		list->tokens = ft_realloc(list->tokens, list->size * sizeof(t_token),
			list->capacity * sizeof(t_token));
	}
	list->tokens[list->size++] = token;
}

void
	token_free(t_token *token)
{
	if (token->token == TOK_SINGLE_QUOTED)
		free(token->str.str);
	if (token->token == TOK_DOUBLE_QUOTED)
		free(token->str.str);
	if (token->token == TOK_WORD)
		free(token->str.str);
	// TODO
}

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

	next = it_next(it);
	while (next.len)
	{
		if (str_cmp(it_substr(it, delim_len), delim))
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
	it_advance(it, 1);
	next = it_substr(it, 2);
	if (!next.len)
		return (error_token(list, it->byte_pos, "Unexpected EOF after '$'"), 1);
	if (!str_cmp(next, "(("))
	{
		it_advance(it, 2);
		if (!read_matching(it, "))"))
		{
			error_token(list, start + 1, "Unclosed '((' delimiter");
			error_token(list, it->byte_pos, "Missing '))' delimiter");
			return (1);
		}
		token_list_push(list, (t_token){
			.token = TOK_ARITH,
			.str = {.str = it->str.str + start + 3, .len = it->byte_pos - start + 1},
			.pos = it->byte_pos,
		});
		it_advance(it, 2);
	}
	if (!str_cmp(next, "("))
	{
		it_advance(it, 1);
		if (!read_matching(it, ")"))
		{
			error_token(list, start + 1, "Unclosed '(' delimiter");
			error_token(list, it->byte_pos, "Missing ')' delimiter");
			return (1);
		}
		token_list_push(list, (t_token){
			.token = TOK_ARITH,
			.str = {.str = it->str.str + start + 2, .len = it->byte_pos - start + 2},
			.pos = it->byte_pos,
		});
		it_advance(it, 1);
	}
	if (!str_cmp(next, "{"))
	{
		it_advance(it, 1);
		if (!read_matching(it, "}"))
		{
			error_token(list, start + 1, "Unclosed '{' delimiter");
			error_token(list, it->byte_pos, "Missing '}' delimiter");
			return (1);
		}
		token_list_push(list, (t_token){
			.token = TOK_PARAM,
			.str = {.str = it->str.str + start + 2, .len = it->byte_pos - start + 2},
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

	stringbuf_init(&content, 16);
	if (quote != '\'' && quote != '"')
		return (0);
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

	stringbuf_init(&content, 256);
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
		printf("Adding: %.*s %d\n", it->codepoint.len, it->codepoint.str, it->codepoint.len);
		stringbuf_append(&content, it->codepoint);
		it_next(it);
	}
	token_list_push(list, (t_token){
		.token = TOK_WORD,
		.str = (t_string){.str = content.str, .len = content.len},
		.pos = start,
	});
}

int main(int argc, char **argv)
{
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
	t_string input = {
		.str = argv[1],
		.len = ft_strlen(argv[1]),
	};
	t_token_list list = {
		.tokens = xmalloc(16 * sizeof(t_token)),
		.size = 0,
		.capacity = 16,
	};
	size_t i;
	printf("prompt=`%s`\n", argv[1]);

	t_u8_iterator iter = it_new(input);
	it_next(&iter);
	while (iter.codepoint.len)
	{
		if (token_isspace(iter.codepoint))
		{
			it_next(&iter);
			continue;
		}

		i = 0;
		while (i < sizeof(tokenizers) / sizeof(tokenizers[0]))
		{
			if (tokenizers[i](&list, &iter))
			{
				i = 0;
				break;
			}
			++i;
		}
		if (i)
			read_word(&list, &iter);
	}
	i = 0;
	while (i < list.size)
	{
		token_free(&list.tokens[i]);
		++i;
	}
	free(list.tokens);
}
