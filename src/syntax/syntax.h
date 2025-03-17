#ifndef SYNTAX_H
# define SYNTAX_H

# include <util/util.h>

typedef struct s_ast_leaf	t_ast_leaf;

typedef struct s_unary_operator
{
}	t_unary_operator;

typedef struct s_binary_operator
{
}	t_binary_operator;

typedef struct s_cmd
{
	t_string_view	program;
	t_ast_leaf		*args;
	size_t			nargs;
}	t_cmd;

typedef union u_token
{
	t_unary_operator	op_1;
	t_binary_operator	op_2;
	t_cmd				cmd;
}	t_token;


typedef struct s_ast_leaf
{
	t_token		token;
	t_ast_leaf	*left;
	t_ast_leaf	*right;
}	t_ast_leaf;

void ast_free(t_ast_leaf *head);

/*
`echo a | cat`:

        `|`
      /    \
 `echo a` `cat`


`cat in | grep s > out`:

         `>`
	   /     \
      `|`   'out'
    /    \
`cat in` `grep s`


`A && (B || C)`:
	
    `&&`
   /   \
 `A`  '||'
     /  \
   `B` `C`


`(A && B) || C`:

      `||`
     /   \
   `&&` 'C'
   / \
 `A` `B`


`A && B&`:

       `&`
	    |
      `&&`
	 /   \
   `A`   `B`
*/

#endif // SYNTAX_H
