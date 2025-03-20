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
#include "syntax/syntax.h"
#include "tokenizer.h"
#include "util/util.h"
#include <stddef.h>
#include <stdio.h>

enum e_node_type
{
	/** @brief Command */
	NODE_COMMAND,
	/** @brief Binary logic operator, e.g `||`, `|&`, `>` */
	NODE_LOGIC,
	/** @brief Unary operator (unhandled currently) e.g: `&`, `!` */
	NODE_UNARY,
};

typedef struct s_ast_node	t_ast_node;

/** @brief Command name and arguments */
struct s_node_cmd
{
	// argv
	t_token	*args;
	// argc
	size_t	nargs;
};

struct s_logic_node
{
	t_token		token;
	t_ast_node	*left;
	t_ast_node	*right;
};

typedef struct s_ast_node
{
	enum e_node_type		type;
	union {
		struct s_node_cmd	cmd;
		struct s_logic_node	logic;
	};
}	t_ast_node;

typedef struct s_parser
{
	t_token_list	list;
	t_string_buffer	*errors;
	size_t			errors_size;
	size_t			errors_cap;
}	t_parser;

t_parser
parser_init(t_token_list list)
{

	return ((t_parser){
		.list = list,
		.errors = xmalloc(sizeof(t_string_buffer) * 16),
		.errors_size = 0,
		.errors_cap = 16,
	});
}

/** @brief Appends error to parser */
void
parser_error(t_parser *parser, t_string_buffer msg)
{
	size_t	new_cap;

	new_cap = parser->errors_cap + !parser->errors_cap * 16;
	while (new_cap < parser->errors_size + 1)
		new_cap *= 2;
	parser->errors = ft_realloc(parser->errors,
			parser->errors_cap * sizeof(t_string_buffer),
			new_cap * sizeof(t_string_buffer));
	parser->errors_cap = new_cap;
	parser->errors[parser->errors_size++] = msg;
}

t_ast_node	*parse_cmd(t_parser *parser, size_t start, size_t end)
{
	const t_token		*tok;
	struct s_node_cmd	cmd;
	t_ast_node			*node;
	size_t				i;

	cmd.nargs = end - start;
	cmd.args = xmalloc(sizeof(t_token) * cmd.nargs);
	i = 0;
	while (i != cmd.nargs)
	{
		tok = &parser->list.tokens[start + i];
		cmd.args[i] = *tok;
		++i;
	}
	node = xmalloc(sizeof(t_ast_node));
	node->type = NODE_COMMAND;
	node->cmd = cmd;
	return (node);
}

size_t
next_operator(t_parser *parser, size_t start, size_t end)
{
	const t_token	*tok;
	size_t			i;

	i = start;
	while (i < end)
	{
		tok = &parser->list.tokens[i];
		if (tok->type == TOK_PIPELINE || tok->type == TOK_REDIR)
			return (i);
		++i;
	}
	return ((size_t)-1);
}

t_ast_node	*parse(t_parser *parser, size_t start, size_t end)
{
	t_ast_node		*node;
	const t_token	*tok;
	size_t			i;
	size_t			next;

	i = start;
	while (i < end)
	{
		next = next_operator(parser, i, end);
		printf("next = %zu\n\n", next);
		if (next == (size_t)-1)
		{
			node = parse_cmd(parser, i, end);
			i = end;
		}
		else
		{
			node = xmalloc(sizeof(t_ast_node));
			node->type = NODE_LOGIC;
			node->logic.token = parser->list.tokens[next];
			node->logic.left = parse_cmd(parser, start, next);
			node->logic.right = parse(parser, next + 1, end);
			i = end;
		}
	}
	return (node);
}

void ast_free(t_ast_node *head)
{
	if (!head)
		return ;
	if (head->type == NODE_LOGIC)
	{
		ast_free(head->logic.left);
		ast_free(head->logic.right);
	}
	else if (head->type == NODE_UNARY)
	{
		// TODO
	}
	else if (head->type == NODE_COMMAND)
	{
		free(head->cmd.args);
	}
	free(head);
}

void
parser_free(t_parser *parser)
{
	free(parser->errors);
}

void ast_print_debug(t_string input, t_ast_node *head, size_t depth)
{
	for (size_t i = 0; i < depth; ++i)
		write(2, "\t", 1);
	if (head->type == NODE_COMMAND)
	{
		dprintf(2, "COMMAND [%zu]: ", head->cmd.nargs);
		for (size_t i = 0; i < head->cmd.nargs; ++i)
			token_print_debug(2, input, &head->cmd.args[i]);
		dprintf(2, "\n");
	}
	else if (head->type == NODE_LOGIC)
	{
		dprintf(2, "LOGIC `%s`\n", head->logic.token.reserved_word);
		ast_print_debug(input, head->logic.left, depth + 1);
		ast_print_debug(input, head->logic.right, depth + 1);
	}
}

int	main(int argc, char **argv)
{
	t_tokenizer	t;
	const		t_string input = {
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
	// TODO: Process expansion

	printf("\n-- Parsing --\n");
	t_parser parser = parser_init(list);
	t_ast_node *head = parse(&parser, 0, list.size);

	// Print AST
	ast_print_debug(input, head, 0);

	parser_free(&parser);
	ast_free(head);

	token_list_free(&list);
	tokenizer_free(&t);
}

void
	tokenizer_init(t_tokenizer *t)
{
	static const t_tokenizer_fn	munchers[] = {
		token_space,
		token_newline,
		token_digit,
		token_redir,
		token_sequence,
		token_pipeline,
		token_grouping,
		token_keyword,
		token_double_quote,
		token_single_quote,
		token_arith,
		token_cmd_sub,
		token_param,
		token_param_simple,
		token_word,
		NULL,
	};

	t->munchers = munchers;
}

void
	tokenizer_free(t_tokenizer *t)
{
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
			dprintf(2, "Leftover tokens: `%.*s`\n",
				(int)(prompt.len - it.byte_pos), prompt.str + it.byte_pos);
			break ;
		}
	}
	return (list);
}
