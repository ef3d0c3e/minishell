/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXPANSION_H
# define  EXPANSION_H

# include <tokenizer/tokenizer.h>
# include <shell/eval.h>

int
expand_tilde(t_environ *env, t_token *token, t_token_list *result);

/**
 * @brief Performs parameter expansion, e.g `$VAR` or `${VAR}`
 *
 * On failure, an error token is inserted and it's error message should be
 * displayed by iterating through the resulting token list.
 *
 * @param env Shell session
 * @param token Parameter token
 * @param result Resulting token list
 *
 * @returns 1 on success, 0 on failure
 */
int
expand_param(t_environ *env, t_token *token, t_token_list *result);

/**
 * @brief Performs word expansion according to bash rules
 * See https://www.gnu.org/software/bash/manual/bash.html#Shell-Expansions
 * for more informations
 *
 * Expansion is performed in the following order:
 *  1. Brace expansion (`test_{a,b}` -> `test_a test_b`)
 *  2. Tilde expansion (`~`, `~user`, etc...)
 *  3. Parameter and variable (`${var}`, `$var`)
 *  4. Command substitution `$(cmd)` -> replace by it's `stdout`
 *  5. Arithmetic expansion
 *  6. Process substitution
 *  7. Word splitting (TODO: rules)
 *  8. Filename expansion
 *
 * @note The caller is expected to check the environment for potential errors
 * On errors, the interpreter should stop.
 *
 * @param env The shell session
 * @param list Token list to expand
 * @return The expanded token list
 */
t_token_list
token_expand(t_environ *env, t_token_list list);

#endif // EXPANSION_H
