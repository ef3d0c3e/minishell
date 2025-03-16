#ifndef STRING_VIEW_H
# define STRING_VIEW_H

# include <stdlib.h>

/**
 * @brief Non-owning string slice
 */
typedef struct s_string_view
{
	const char	*s;
	size_t		len;
}	t_string_view;

/**
 * @brief Performs string comparisons
 *
 * @param sv String to compare with
 * @param token Token to compare to (NUL Terminated)
 *
 * @returns 0 If equal
 */
int
sv_cmp(t_string_view sv, const char *token);

#endif // STRING_VIEW_H
