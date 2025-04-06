#include <expansion/expansion.h>
#include <shell/eval.h>
#include <stdio.h>
#include <util/util.h>

void
	environ_init(t_environ *env)
{
	env->env = rb_new((int (*)(const void *, const void *))ft_strcmp,
			free, free);
}

/**
 * @brief Parses environment in the form of `key=value`
 *
 * @param env Environment structure
 * @param s String to parse
 *
 * @returns 1 if correctly parsed, 0 if an error happened
 */
int
	parse_environ(t_environ *env, const char *s)
{
	const char		*sep = ft_strchr(s, '=');
	t_string_buffer	key;
	t_string_buffer	value;

	if (!sep)
		return (0);
	key = stringbuf_from_range(s, sep);
	value = stringbuf_from(sep + 1);
	rb_insert(&env->env, stringbuf_cstr(&key), stringbuf_cstr(&value));
	return (1);
}

void	print_fn(size_t depth, t_rbnode *node, void* cookie)
{
	for (size_t i = 0; i < depth; ++i)
		printf(" | ");
	printf("%s: %s\n", (char *)node->key, (char *)node->data);
}

int main(int ac, char **av, char **envp)
{
	// BUILD Environment
	t_environ	environ;

	environ_init(&environ);
	char **env = envp;
	while (*env)
	{
		parse_environ(&environ, *env);
		++env;
	}
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

	parser_free(&parser);
	ast_free(head);

	// Free environment
	rb_free(&environ.env);

	return (0);
}
