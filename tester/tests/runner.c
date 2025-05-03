/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static void
	evaluator(void *data)
{
	t_evaluator_data *const	eval = data;

	eval->test->eval(eval->runner, eval->seed);
}

static void
	run_tests(const char *filter, size_t id_filter, uint32_t seed)
{
	static const t_unit_test	tests[] = {
		test_echo,
		test_grammar,
		test_param,
		test_pipe,
		test_sub,
		test_redir,
		test_expand,
	};
	size_t						i;
	t_evaluator_data			eval;
	t_runner_data				runner;

	runner.total = 0;
	runner.passed = 0;
	runner.test_id = 1;
	runner.id_filter = id_filter;
	i = 0;
	while (i < sizeof(tests) / sizeof(tests[0]))
	{
		eval.seed = seed;
		eval.test = &tests[i];
		eval.runner = &runner;
		if (ft_strstr(tests[i].name, filter))
			eval_in_tempdir(tests[i].name, evaluator, &eval);
		++i;
	}
	if (!runner.total)
		return ;
	ft_printf("-- Passed %d/%d tests (%d%%) --\n", runner.passed, runner.total,
		(int)(100.f * runner.passed / (float)runner.total));
}

int	main(int argc, char **argv)
{
	const char	*filter = "";
	size_t		id_filter;
	uint32_t	seed;

	id_filter = 0;
	seed = 1;
	if (argc > 1)
		filter = argv[1];
	if (argc > 2)
		id_filter = atosz(argv[2]);
	ft_printf("Running tests with filter `%s` with random seed=%u\n",
		filter, seed);
	run_tests(filter, id_filter, seed);
}
