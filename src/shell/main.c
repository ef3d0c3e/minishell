#include <expansion/expansion.h>
#include <shell/eval.h>

void	print_fn(size_t depth, t_rbnode *node, void* cookie)
{
	for (size_t i = 0; i < depth; ++i)
		printf(" | ");
	printf("%s: %s\n", (char *)node->key, (char *)node->data);
}

int main(int ac, char **av, char **envp)
{
	// BUILD Environment
	t_environ	environ = env_new(envp);

	rb_apply(&environ.env, print_fn, NULL);

	// Process input
	const		t_string input = {
		.str = av[1],
		.len = ft_strlen(av[1]),
	};
	printf("prompt=`%s`\n", av[1]);

	dprintf(2, " -- Raw tokens --\n");
	t_token_list list = tokenizer_tokenize(input);
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
	t_parser parser = parser_init(input, list);

	t_ast_node *head = parse(&parser, 0, parser.list.size);
	// Print AST
	ast_print_debug(input, head, 0);
	// TODO: Error checking
	
	// Eval
	eval(&environ, head);

	ast_free(head);
	token_list_free(&list);
	parser_free(&parser);

	// Free environment
	env_free(&environ);

	return (0);
}
