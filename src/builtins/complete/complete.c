/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/13 13:58:46 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

t_cmd_completion
	*complete_new(t_shell *shell, const char *name)
{
	t_cmd_completion *const	comp = xmalloc(sizeof(t_cmd_completion));

	ft_memset(comp, 0, sizeof(t_cmd_completion));
	return (rb_insert(&shell->cmd_completion, ft_strdup(name), comp)->data);
}

void
	complete_add(t_shell *shell, const char *cmd, t_cmd_comp_opt opt)
{
	t_cmd_completion	*comp;
	size_t				new_cap;

	comp = rb_find(&shell->cmd_completion, cmd);
	if (!comp)
		comp = complete_new(shell, cmd);
	if (comp->opts_size >= comp->opts_cap)
	{
		new_cap = (comp->opts_cap + !comp->opts_cap) * 2;
		comp->opts = ft_realloc(comp->opts,
				sizeof(t_cmd_comp_opt) * comp->opts_size,
				sizeof(t_cmd_comp_opt) * new_cap);
		comp->opts_cap = new_cap;
	}
	comp->opts[comp->opts_size++] = opt;
}

void
	complete_free(void *ptr)
{
	t_cmd_completion *const	comp = ptr;
	size_t					i;

	i = 0;
	while (i < comp->opts_size)
	{
		free(comp->opts[i].description);
		free(comp->opts[i].longname);
		free(comp->opts[i].shortname);
		++i;
	}
	free(comp->opts);
	i = 0;
	while (comp->source_from && comp->source_from[i])
		free(comp->source_from[i++]);
	free(comp->source_from);
	free(comp);
}
