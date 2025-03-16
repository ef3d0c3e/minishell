#include "unicode.h"

#include <util/util.h>

/* Codepoint length in bytes */
static inline ssize_t
	cp_len(const char *s, const size_t len)
{
	if (!len)
		return (0);
	if (~(s[0]) & 0x80)
		return (1);
	else if (~(s[0] & 0xE0) & 0x20)
	{
		if (len < 1)
			return (0);
		else if (~(s[1] & 0xB0) & 0x40)
			return (2);
		else
			return (-1);
	}
	else if (~(s[0] & 0xF0) & 0x10)
	{
		if (len < 2)
			return (0);
		else if ((~(s[1] & 0xB0) & 0x40) && (~(s[2] & 0xB0) & 0x40))
			return (3);
		else
			return (-1);
	}
	else if (~(s[0] & 0xF8) & 0x08)
	{
		if (len < 3)
			return (0);
		else if ((~(s[1] & 0xB0) & 0x40) && (~(s[2] & 0xB0) & 0x40) && (~(s[3] & 0xB0) & 0x40))
			return (4);
		else
			return (-1);
	}
	return (-1);
}

t_u8_iterator
	iterator_new(t_string_view sv)
{
	return ((t_u8_iterator){
		.sv = sv,
		.byte_pos = 0,
		.cp_pos = 0,
		.cp_len = cp_len(sv.s, sv.len),
	});
}

t_string_view
	iterator_next(t_u8_iterator *it)
{
	const t_string_view	result = {
		.s = it->sv.s + it->byte_pos,
		.len = it->cp_len,
	};

	++it->cp_pos;
	it->byte_pos += it->cp_len;
	it->cp_len = cp_len(it->sv.s + it->byte_pos, it->sv.len - it->byte_pos);
	return (result);
}
