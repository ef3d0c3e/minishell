/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef AST_H
# define AST_H

# include <parser/words/words.h>
# include <parser/redirs/redir_parser.h>

/******************************************************************************/
/* The AST                                                                    */
/******************************************************************************/

typedef struct s_ast_node	t_ast_node;

/** @brief Type for nodes */
enum e_node_type
{
	/** @brief Block node, delimited by `{` and `}` */
	NODE_BLOCK,
	/** @brief Expression that should run in a sub-shell, delimited by `{` and
	 * `}` */
	NODE_SUBSHELL,
	/** @brief Command */
	NODE_COMMAND,
	/** @brief Command list */
	NODE_LIST,
	/** @brief Binary logic operators, e.g `||`, `&&` */
	NODE_BINARY,
	/** @brief Pipe logic operators: `|`, `|&` */
	NODE_PIPE,
	/** @brief Function definition node */
	NODE_FUNCTION,
	/** @brief If statement node */
	NODE_IF,
	/** @brief While or until loop statement node */
	NODE_LOOP,
	/** @brief For statement node */
	NODE_FOR,
};

/** @brief A block node */
struct s_node_block
{
	/** @brief Subshell inner */
	t_ast_node		*inner;
};

/** @brief Creates a block node */
t_ast_node
*make_block_node(t_ast_node *inner);
/** @brief Frees a block node */
void
free_block_node(t_ast_node *node);
/** @brief Prints a block node */
void
print_block_node(size_t depth, const t_ast_node *node);

/** @brief Sub expression data */
struct s_subshell_node
{
	/** @brief Subshell inner */
	t_ast_node		*head;
	/** @brief Redirections */
	t_redirections	redirs;
};

/** @brief Creates a subshell node */
t_ast_node
*make_subshell_node(t_ast_node *inner);
/** @brief Frees a subshell node */
void
free_subshell_node(t_ast_node *node);
/** @brief Prints a subshell node */
void
print_subshell_node(size_t depth, const t_ast_node *node);

/** @brief Command name and arguments */
struct s_cmd_node
{
	/** @brief Program arguments */
	t_wordlist			args;
	/** @brief Assignments for this command */
	struct s_assignment	*assigns;
	/** @brief Number of assignments */
	size_t				nassigns;
	/** @brief Redirections */
	t_redirections		redirs;
};

/** @brief Creates a command node */
t_ast_node
*make_cmd_node(void);
/** @brief Frees a command node */
void
free_cmd_node(t_ast_node *node);
/** @brief Prints a command node */
void
print_cmd_node(size_t depth, const t_ast_node *node);

/** @brief Command list node */
struct s_cmdlist_node
{
	/** @brief The commands */
	t_ast_node	**cmds;
	/** @brief Separator tokens */
	char		*seps;
	/** @brief Number of commands */
	size_t		ncmds;
};

t_ast_node
*make_list_node(void);
/** @brief Frees a command list node */
void
free_list_node(t_ast_node *node);
/** @brief Prints a command list node */
void
print_list_node(size_t depth, const t_ast_node *node);
/**
 * @brief Pushes a command to a command list
 *
 * @param list The command list to push to
 * @param cmd Command to push to the command list
 * @param sep Separator
 */
void
list_node_push(t_ast_node *list, t_ast_node *cmd, char sep);

/** @brief Data for logic (binary) nodes */
struct s_logic_node
{
	/** @brief Operator token */
	t_token		token;
	/** @brief Left operand */
	t_ast_node	*left;
	/** @brief Right operand */
	t_ast_node	*right;
};

/** @brief Creates a logic node */
t_ast_node
*make_logic_node(t_token op, t_ast_node *left, t_ast_node *right);
/** @brief Frees a logic node */
void
free_logic_node(t_ast_node *node);
/** @brief Prints a logic node */
void
print_logic_node(size_t depth, const t_ast_node *node);

/** @brief A node for function definition */
struct s_function_node
{
	/** @brief Function's name */
	t_string_buffer	name;
	/** @brief The function's body */
	t_ast_node		*body;
	/** @brief Set to 1 when the function is registered. This prevent the
	 * function from being freed when the ast is freed. */
	int				registered;
};

/** @brief Creates a function node */
t_ast_node
*make_function_node(t_string_buffer name, t_ast_node *body);
/** @brief Frees a function node */
void
free_function_node(t_ast_node *node);
/** @brief Prints a function node */
void
print_function_node(size_t depth, const t_ast_node *node);

/** @brief If statement node */
struct s_if_node
{
	/** @brief Conditions, at most `nbodies` */
	t_ast_node	**conds;
	size_t		nconds;
	/** @brief Bodies, at most `nconds + 1` */
	t_ast_node	**bodies;
	size_t		nbodies;
};

/** @brief Creates a if node */
t_ast_node
*make_if_node(void);
/** @brief Frees a if node */
void
free_if_node(t_ast_node *node);
/** @brief Prints a if node */
void
print_if_node(size_t depth, const t_ast_node *node);

/** @brief Loop statement node */
struct s_loop_node
{
	/** @brief Set to 1 for until node, 0 for while */
	int			until;
	/** @brief Loop condition */
	t_ast_node	*cond;
	/** @brief Loop body */
	t_ast_node	*body;
};

/** @brief Creates a loop node */
t_ast_node
*make_loop_node(t_ast_node *cond, t_ast_node *body, int until);
/** @brief Frees a loop node */
void
free_loop_node(t_ast_node *node);
/** @brief Prints a loop node */
void
print_loop_node(size_t depth, const t_ast_node *node);

/** @brief While statement node */
struct s_for_node
{
	/** @brief Loop identifier, must be valid */
	char				*ident;
	/** @brief Word list (must undergo expansion) */
	t_wordlist			args;
	/** @brief For body */
	t_ast_node			*body;
};

/** @brief Creates a for node */
t_ast_node
*make_for_node(char *ident);
/** @brief Frees a for node */
void
free_for_node(t_ast_node *node);
/** @brief Prints a for node */
void
print_for_node(size_t depth, const t_ast_node *node);

/** @brief AST Node type */
typedef struct s_ast_node
{
	/** @brief Node type */
	enum e_node_type			type;
	/** @brief Node-specific data */
	union {
		/** @brief Block expression AST */
		struct s_node_block		block;
		/** @brief Sub expressions AST */
		struct s_subshell_node	sub;
		/** @brief Atom, for commands (name & parameters) */
		struct s_cmd_node		cmd;
		/** @brief Command list token */
		struct s_cmdlist_node	list;
		/** @brief Binary nodes, e.g `|` or `&&` */
		struct s_logic_node		logic;
		/** @brief Function definition node */
		struct s_function_node	function;
		/** @brief If node */
		struct s_if_node		st_if;
		/** @brief Loop (until or while) node */
		struct s_loop_node		st_loop;
		/** @brief For node */
		struct s_for_node		st_for;
	};
}	t_ast_node;

/**
 * @brief Frees the AST
 *
 * @param node Node to free
 * @param cleanup Forces cleanup of functions
 */
void
ast_free(t_ast_node *node, int cleanup);
/** @brief Prints debug info for the AST */
void
ast_print(size_t depth, t_ast_node *node);


#endif // AST_H
