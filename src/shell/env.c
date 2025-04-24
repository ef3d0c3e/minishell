/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/eval.h>

/**
 * @brief Parses environment in the form of `key=value`
 *
 * @param env Environment structure
 * @param s String to parse
 *
 * @returns 1 if correctly parsed, 0 if an error happened
 */
static int
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

t_environ
	env_new(const char **envp)
{
	const char	**e;
	t_environ	env;

	env.env = rb_new((int (*)(const void *, const void *))ft_strcmp,
			free, free);
	env.path_program = rb_new((int (*)(const void *, const void *))ft_strcmp,
			free, free);
	env.errors_capacity = 0;
	env.errors_size = 0;
	env.errors = NULL;
	env.last_status = 0;
	env.is_child = 0;
	env.prompt = NULL;
	env.token_list = NULL;
	env.parser = NULL;
	env.program = NULL;

	env.is_interactive = 1;
	e = envp;
	while (*e)
		parse_environ(&env, *(e++));
	init_options(&env);
	init_builtin(&env);
	path_populate(&env);
	shell_error_flush(&env);
	return (env);
}

void
	env_parser_free(t_environ *env)
{
	free(env->prompt);
	if (env->token_list)
		token_list_free(env->token_list);
	if (env->parser)
		parser_free(env->parser);
	ast_free(env->program);
	env->prompt = NULL;
	env->token_list = NULL;
	env->parser = NULL;
	env->program = NULL;
}

void
	env_free(t_environ *env)
{
	shell_error_flush(env);
	rb_free(&env->env);
	rb_free(&env->opts);
	rb_free(&env->builtins);
	rb_free(&env->path_program);
	env_parser_free(env);
	free(env->errors);
}
