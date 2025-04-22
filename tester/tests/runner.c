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

static char
	*ft_strstr(const char *haystack, const char *needle)
{
	const size_t	n = ft_strlen(haystack);
	size_t			find_len;
	size_t			i;
	size_t			j;

	if (!*needle)
		return ((char *)haystack);
	if (n == 0)
		return ((char *)0);
	find_len = ft_strlen(needle);
	i = 0;
	while (haystack[i] && i + find_len <= n)
	{
		j = 0;
		while (haystack[i + j] == needle[j])
		{
			if (j + 1 == find_len)
				return (((char *)haystack + i));
			++j;
		}
		++i;
	}
	return (0);
}

static size_t
	atosz(const char *s)
{
	size_t	v;

	v = 0;
	while (*s >= '0' && *s <= '9')
		v = v * 10 + (*(s++) - '0');
	return (v);
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
	};
	size_t						i;
	t_runner_data				runner;

	runner.total = 0;
	runner.passed = 0;
	runner.test_id = 1;
	runner.id_filter = id_filter;
	i = 0;
	while (i < sizeof(tests) / sizeof(tests[0]))
	{
		if (ft_strstr(tests[i].name, filter))
			tests[i].eval(&runner, seed);
		++i;
	}
	if (!runner.total)
		return ;
	ft_printf("-- Passed %d/%d tests (%d%%) --\n", runner.total, runner.passed,
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
