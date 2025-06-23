/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cowsay.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/13 13:58:46 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>
#include <builtins/cowsay/cowsay.h>

static void
	append_message(t_string_buffer *out, const char *msg, size_t msg_width)
{
	size_t	i;

	i = 0;
	stringbuf_append(out, (t_string){" ", 1});
	stringbuf_append_n(out, (t_string){"_", 1}, msg_width);
	stringbuf_append(out, (t_string){"\n<", 2});
	while (msg[i++])
	{
		if (msg[i - 1] != '\n')
			stringbuf_append(out, (t_string){&msg[i - 1], 1});
		else if (msg[i])
			stringbuf_append(out, (t_string){" ", 1});
	}
	stringbuf_append(out, (t_string){">\n", 2});
	stringbuf_append(out, (t_string){" ", 1});
	stringbuf_append_n(out, (t_string){"_", 1}, msg_width);
	stringbuf_append(out, (t_string){"\n", 1});
}

static void
	append_cow_top(
	t_string_buffer *out,
	const t_cowsay_args *args,
	size_t msg_width)
{
	stringbuf_append_n(out, (t_string){" ", 1}, msg_width + 2);
	stringbuf_append(out, (t_string){"\\", 1});
	stringbuf_append_n(out, (t_string){" ", 1}, 3);
	stringbuf_append(out, (t_string){"^__^\n", 5});
	stringbuf_append_n(out, (t_string){" ", 1}, msg_width + 3);
	stringbuf_append(out, (t_string){"\\", 1});
	stringbuf_append_n(out, (t_string){" ", 1}, 2);
	stringbuf_append(out, (t_string){"(", 1});
	stringbuf_append(out, (t_string){args->eyes, ft_strlen(args->eyes)});
	stringbuf_append(out, (t_string){")\\_______\n", 10});
}

static void
	append_cow_bot(
	t_string_buffer *out,
	const t_cowsay_args *args,
	size_t msg_width)
{
	stringbuf_append_n(out, (t_string){" ", 1}, msg_width + 6);
	stringbuf_append(out, (t_string){"(", 1});
	stringbuf_append(out, (t_string){args->mouth, ft_strlen(args->mouth)});
	stringbuf_append(out, (t_string){")\\       )\\/\\\n", 14});
	stringbuf_append_n(out, (t_string){" ", 1}, msg_width + 7);
	stringbuf_append(out, (t_string){args->tongue, ft_strlen(args->tongue)});
	stringbuf_append(out, (t_string){" ||----", 7});
	stringbuf_append(out, (t_string){args->udder, ft_strlen(args->udder)});
	stringbuf_append(out, (t_string){"|\n", 2});
	stringbuf_append_n(out, (t_string){" ", 1}, msg_width + 10);
	stringbuf_append(out, (t_string){"||     ||\n", 10});
}

void
	cowsay_print(const t_cowsay_args *args, const char *msg)
{
	const size_t	msg_width = string_width(msg);
	t_string_buffer	out;

	stringbuf_init(&out, 256);
	append_message(&out, msg, msg_width);
	append_cow_top(&out, args, msg_width);
	append_cow_bot(&out, args, msg_width);
	write(STDOUT_FILENO, out.str, out.len);
	stringbuf_free(&out);
}
