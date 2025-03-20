#include "parser.h"
#include "expansion.h"

t_ast_node	*parse(t_parser *parser, size_t start, size_t end)
{
	t_ast_node		*node;
	size_t			i;
	size_t			next;

	i = start;
	while (i < end)
	{
		next = parser_next_operator(parser, i, end);
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


int	main(int argc, char **argv)
{
	t_tokenizer	t;
	const		t_string input = {
		.str = argv[1],
		.len = ft_strlen(argv[1]),
	};
	tokenizer_init(&t);
	printf("prompt=`%s`\n", argv[1]);

	dprintf(2, " -- Raw tokens --\n");
	t_token_list list = tokenizer_tokenize(&t, input);
	size_t i = 0;
	while (i < list.size)
	{
		token_print_debug(2, input, &list.tokens[i]);
		++i;
	}
	dprintf(2, "\n -- Post expansion --\n");
	list = token_expand(list);
	i = 0;
	while (i < list.size)
	{
		token_print_debug(2, input, &list.tokens[i]);
		++i;
	}
	printf("\n-- Parsing --\n");
	t_parser parser = parser_init(list);
	t_ast_node *head = parse(&parser, 0, list.size);

	// Print AST
	ast_print_debug(input, head, 0);

	parser_free(&parser);
	ast_free(head);

	token_list_free(&list);
}
