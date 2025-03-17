#include "token.h"

void ast_free(t_ast_leaf *head)
{
	if (head->left)
	{
		ast_free(head->left);
		free(head->left);
	}
	if (head->right)
	{
		ast_free(head->right);
		free(head->right);
	}
}

t_ast_leaf ms_parse(const t_string_view sv)
{
	t_u8_iterator it = iterator_new(sv);
	t_string_view cp;
	size_t escaped = 0;

	cp = iterator_next(&it);
	while (cp.len > 0)
	{
		
	}
	return (t_ast_leaf){};
}

int main(int argc, char **argv)
{
	const char *s = argv[1];

	t_string_view tok = {
		.s = s,
		.len = ft_strlen(s),
	};

	t_ast_leaf head = ms_parse(tok);
	asm("int $3");
}
