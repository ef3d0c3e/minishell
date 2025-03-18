# include "parser.h"

# include "syntax.h"


/*
void
parser_state_init(t_parser_state *state, t_string_view prompt)
{
	state->escape_len = 0;
	state->it = iterator_new(prompt);
	state->word = (t_string_view){.s = NULL, .len = 0};
	state->quote = (t_string_view){.s = NULL, .len = 0};
	state->escape_len = 0;
}

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

t_string
	parser_next_token(t_parser_state *state)
{
	t_string_view	cp;

	cp = iterator_next(&state->it);
	while (cp.len > 0)
	{
		if (state->quote.s)
		{
			
		}
		cp = iterator_next(&state->it);
	}
}

t_ast_leaf ms_parse(const t_string_view sv)
{
	t_parser_state	state;
	parser_state_init(&state, sv);
	t_u8_iterator it = iterator_new(sv);
	t_string_view cp;
	t_string_view word = {.s = sv.s, .len = 0};
	size_t	escaped = 0;
	char 	quote = 0;

	cp = iterator_next(&it);
	while (cp.len > 0)
	{
		if (!sv_cmp(cp, "\\"))
			++escaped;
		else if (!quote && (!sv_cmp(cp, "'") || !sv_cmp(cp, "\"")))
			quote = cp.s[0];
		//else if (token_word-(cp))
		{
			if (quote)
			{
				
			}
		}
		word.len += cp.len;
	}
	return (t_ast_leaf){};
}
*/
