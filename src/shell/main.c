#include "parser/parser.h"
#include "shell/opts.h"
#include "tokenizer/tokenizer.h"
#include <expansion/expansion.h>
#include <shell/eval.h>
#include <stdio.h>

static void
	token_list_debug(t_string input, const t_token_list *list)
{
	size_t	i;

	i = 0;
	while (i < list->size)
	{
		token_print_debug(2, input, &list->tokens[i]);
		++i;
	}
	ft_dprintf(2, "\n");
}

int main(int ac, char **av, char **envp)
{
	// BUILD Environment
	t_environ		environ = env_new(envp);
	t_token_list	list;
	t_parser		parser;
	environ.prompt = ft_strdup(av[1]);
	t_string input = {
		.str = environ.prompt,
		.len = ft_strlen(environ.prompt),
	};
	printf("prompt=`%s`\n", av[1]);

	environ.token_list = &list;
	*environ.token_list = tokenizer_tokenize(input);
	if (option_get(&environ, "dbg_token"))
	{
		dprintf(2, " -- Raw tokens --\n");
		token_list_debug(input, &list);
	}

	*environ.token_list = token_expand(&environ, *environ.token_list);
	shell_error_flush(&environ);
	if (option_get(&environ, "dbg_token"))
	{
		dprintf(2, " -- Post expansion --\n");
		token_list_debug(input, environ.token_list);
	}

	parser = parser_init(input, *environ.token_list);
	environ.parser = &parser;

	environ.program = parse(&parser, 0, parser.list.size);
	// Print AST
	if (option_get(&environ, "dbg_ast"))
	{
		printf(" -- Parsing --\n");
		ast_print_debug(input, environ.program, 0);
	}
	// TODO: Error checking
	
	// Eval
	eval(&environ, environ.program);

	//ast_free(head);

	// Free environment
	env_free(&environ);

	return (0);
}
