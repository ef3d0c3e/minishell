/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete_filename.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/19 06:48:54 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

/** @brief Builds regex using Literal(filter) + Expr(*) */
static void
	build_regex(const char *filter, t_comp_file_tr *tr)
{
	t_regex_builder builder;
	t_globopts		opts;

	ft_memset(&opts, 0, sizeof(t_globopts));
	opts.dotglob = 1;
	opts.nocaseglob = 1;
	builder = regex_builder_new();
	if (filter && ft_strlen(filter))
		regex_builder_literal(&opts, &builder, filter);	
	regex_builder_expr(&opts, &builder, "*");
	tr->re = builder.regex;
	tr->opts = opts;
}

/** @brief Gets an entry's completion item */
static t_complete_item
	get_item(const char *path, const struct stat *sb)
{
	if (S_ISCHR(sb->st_mode))
		return ((t_complete_item){
			COMPLETE_FILE_SPC, ft_strdup(path), ft_strdup("Character Device")});
	else if (S_ISBLK(sb->st_mode))
		return ((t_complete_item){
			COMPLETE_FILE_SPC, ft_strdup(path), ft_strdup("Block Device")});
	else if (S_ISLNK(sb->st_mode))
		return ((t_complete_item){
			COMPLETE_FILE_SPC, ft_strdup(path), ft_strdup("Symbolic Link")});
	else if (S_ISFIFO(sb->st_mode))
		return ((t_complete_item){
			COMPLETE_FILE_SPC, ft_strdup(path), ft_strdup("FIFO")});
	else if (S_ISSOCK(sb->st_mode))
		return ((t_complete_item){
			COMPLETE_FILE_SPC, ft_strdup(path), ft_strdup("Unix Socket")});
	else if (S_ISDIR(sb->st_mode))
		return ((t_complete_item){
			COMPLETE_FILE_DIR, ft_strdup(path), ft_strdup("Directory")});
	else
		return ((t_complete_item){
			COMPLETE_FILE, ft_strdup(path), ft_strdup("File")});
}

/** @brief Apply to directories recursively */
static int
	collect_files(char *path, const struct stat *sb, void *ptr)
{
	t_comp_file_tr *const	tr = ptr;
	int						r;

	(void)sb;
	r = regex_match(&tr->opts, &tr->re, path);
	if (r == 0)
		return (1);
	if (r == 2)
		complete_buf_push(tr->items, get_item(path, sb));
	return (0);
}

void
	repl_complete_filename(t_complete_buf *items, const char *filter)
{
	t_comp_file_tr	tr;
	char			*path;
	size_t			recurse;

	tr.items = items;
	build_regex(filter, &tr);
	regex_print(0, tr.re.expr);
	recurse = regex_recurse_depth(tr.re.expr);
	path = regex_path(&tr.re);
	file_tree_walk(path, recurse + (recurse != (size_t)-1), collect_files, &tr);
	free(path);
	regex_free(tr.re.expr);
}
