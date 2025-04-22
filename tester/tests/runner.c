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

int	main(int argc, char **argv)
{
	static const t_unit_test	tests[] = {
		test_echo,
		test_grammar,
		test_param,
		test_sub,
	};
	const char					*filter = "";
	size_t						i;
	int							passed;
	int							total;

	if (argc > 1)
		filter = argv[1];
	total = 0;
	passed = 0;
	i = 0;
	while (i < sizeof(tests) / sizeof(tests[0]))
	{
		if (ft_strstr(tests[i].name, filter))
			tests[i].eval(&passed, &total);
		++i;
	}
	ft_printf("-- Passed %d/%d tests (%d%%) --\n", total, passed,
			(int)(100.f * passed / (float)total ));
}
