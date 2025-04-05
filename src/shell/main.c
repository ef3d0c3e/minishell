#include <stdio.h>
#include <util/util.h>

typedef struct s_environ
{
	/** @brief Stores all environment variables */
	t_rbtree	env;
}	t_environ;

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
	t_environ	environ;

	environ_init(&environ);
	char **env = envp;
	while (*env)
	{
		parse_environ(&environ, *env);
		++env;
	}
	rb_apply(&environ.env, print_fn, NULL);

	t_rbnode *node = rb_find_node(&environ.env, "I3SOCK");
	print_fn(0, node, NULL);

	return (0);
}
