#include "string_view.h"

int
	sv_cmp(t_string_view sv, const char *token)
{
	size_t	i;

	i = 0;
	while (i < sv.len && token[i] && sv.s[i] == token[i])
		++i;
	return (sv.s[i] - token[i]);
}
