/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wcwidth_table_wide.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

/**
 * @brief Table for wide characters, reference:
 * https://www.unicode.org/Public/UCD/latest/ucd/EastAsianWidth.txt
 */

void
	wcwidth_table_wide1(struct s_codepoint_range *r)
{
	r[0] = (struct s_codepoint_range){0x1100, 0x115F};
	r[1] = (struct s_codepoint_range){0x231A, 0x231B};
	r[2] = (struct s_codepoint_range){0x2329, 0x232A};
	r[3] = (struct s_codepoint_range){0x23E9, 0x23EC};
	r[4] = (struct s_codepoint_range){0x23F0, 0x23F0};
	r[5] = (struct s_codepoint_range){0x23F3, 0x23F3};
	r[6] = (struct s_codepoint_range){0x25FD, 0x25FE};
	r[7] = (struct s_codepoint_range){0x2614, 0x2615};
	r[8] = (struct s_codepoint_range){0x2630, 0x2637};
	r[9] = (struct s_codepoint_range){0x2648, 0x2653};
	r[10] = (struct s_codepoint_range){0x267F, 0x267F};
	r[11] = (struct s_codepoint_range){0x268A, 0x268F};
	r[12] = (struct s_codepoint_range){0x2693, 0x2693};
	r[13] = (struct s_codepoint_range){0x26A1, 0x26A1};
	r[14] = (struct s_codepoint_range){0x26AA, 0x26AB};
	r[15] = (struct s_codepoint_range){0x26BD, 0x26BE};
	r[16] = (struct s_codepoint_range){0x26C4, 0x26C5};
	r[17] = (struct s_codepoint_range){0x26CE, 0x26CE};
	r[18] = (struct s_codepoint_range){0x26D4, 0x26D4};
	r[19] = (struct s_codepoint_range){0x26EA, 0x26EA};
	r[20] = (struct s_codepoint_range){0x26F2, 0x26F3};
	r[21] = (struct s_codepoint_range){0x26F5, 0x26F5};
	r[22] = (struct s_codepoint_range){0x26FA, 0x26FA};
	r[23] = (struct s_codepoint_range){0x26FD, 0x26FD};
	r[24] = (struct s_codepoint_range){0x2705, 0x2705};
}

void
	wcwidth_table_wide2(struct s_codepoint_range *r)
{
	r[25] = (struct s_codepoint_range){0x270A, 0x270B};
	r[26] = (struct s_codepoint_range){0x2728, 0x2728};
	r[27] = (struct s_codepoint_range){0x274C, 0x274C};
	r[28] = (struct s_codepoint_range){0x274E, 0x274E};
	r[29] = (struct s_codepoint_range){0x2753, 0x2755};
	r[30] = (struct s_codepoint_range){0x2757, 0x2757};
	r[31] = (struct s_codepoint_range){0x2795, 0x2797};
	r[32] = (struct s_codepoint_range){0x27B0, 0x27B0};
	r[33] = (struct s_codepoint_range){0x27BF, 0x27BF};
	r[34] = (struct s_codepoint_range){0x2B1B, 0x2B1C};
	r[35] = (struct s_codepoint_range){0x2B50, 0x2B50};
	r[36] = (struct s_codepoint_range){0x2B55, 0x2B55};
	r[37] = (struct s_codepoint_range){0x2E80, 0x2E99};
	r[38] = (struct s_codepoint_range){0x2E9B, 0x2EF3};
	r[39] = (struct s_codepoint_range){0x2F00, 0x2FD5};
	r[40] = (struct s_codepoint_range){0x2FF0, 0x303E};
	r[41] = (struct s_codepoint_range){0x3041, 0x3096};
	r[42] = (struct s_codepoint_range){0x3099, 0x30FF};
	r[43] = (struct s_codepoint_range){0x3105, 0x312F};
	r[44] = (struct s_codepoint_range){0x3131, 0x318E};
	r[45] = (struct s_codepoint_range){0x3190, 0x31E5};
	r[46] = (struct s_codepoint_range){0x31EF, 0x321E};
	r[47] = (struct s_codepoint_range){0x3220, 0x3247};
	r[48] = (struct s_codepoint_range){0x3250, 0xA48C};
	r[49] = (struct s_codepoint_range){0xA490, 0xA4C6};
}

void
	wcwidth_table_wide3(struct s_codepoint_range *r)
{
	r[50] = (struct s_codepoint_range){0xA960, 0xA97C};
	r[51] = (struct s_codepoint_range){0xAC00, 0xD7A3};
	r[52] = (struct s_codepoint_range){0xF900, 0xFAFF};
	r[53] = (struct s_codepoint_range){0xFE10, 0xFE19};
	r[54] = (struct s_codepoint_range){0xFE30, 0xFE52};
	r[55] = (struct s_codepoint_range){0xFE54, 0xFE66};
	r[56] = (struct s_codepoint_range){0xFE68, 0xFE6B};
	r[57] = (struct s_codepoint_range){0xFF01, 0xFF60};
	r[58] = (struct s_codepoint_range){0xFFE0, 0xFFE6};
	r[59] = (struct s_codepoint_range){0x16FE0, 0x16FE4};
	r[60] = (struct s_codepoint_range){0x16FF0, 0x16FF1};
	r[61] = (struct s_codepoint_range){0x17000, 0x187F7};
	r[62] = (struct s_codepoint_range){0x18800, 0x18CD5};
	r[63] = (struct s_codepoint_range){0x18CFF, 0x18D08};
	r[64] = (struct s_codepoint_range){0x1AFF0, 0x1AFF3};
	r[65] = (struct s_codepoint_range){0x1AFF5, 0x1AFFB};
	r[66] = (struct s_codepoint_range){0x1AFFD, 0x1AFFE};
	r[67] = (struct s_codepoint_range){0x1B000, 0x1B122};
	r[68] = (struct s_codepoint_range){0x1B132, 0x1B132};
	r[69] = (struct s_codepoint_range){0x1B150, 0x1B152};
	r[70] = (struct s_codepoint_range){0x1B155, 0x1B155};
	r[71] = (struct s_codepoint_range){0x1B164, 0x1B167};
	r[72] = (struct s_codepoint_range){0x1B170, 0x1B2FB};
	r[73] = (struct s_codepoint_range){0x1D300, 0x1D356};
	r[74] = (struct s_codepoint_range){0x1D360, 0x1D376};
}

void
	wcwidth_table_wide4(struct s_codepoint_range *r)
{
	r[75] = (struct s_codepoint_range){0x1F004, 0x1F004};
	r[76] = (struct s_codepoint_range){0x1F0CF, 0x1F0CF};
	r[77] = (struct s_codepoint_range){0x1F18E, 0x1F18E};
	r[78] = (struct s_codepoint_range){0x1F191, 0x1F19A};
	r[79] = (struct s_codepoint_range){0x1F200, 0x1F202};
	r[80] = (struct s_codepoint_range){0x1F210, 0x1F23B};
	r[81] = (struct s_codepoint_range){0x1F240, 0x1F248};
	r[82] = (struct s_codepoint_range){0x1F250, 0x1F251};
	r[83] = (struct s_codepoint_range){0x1F260, 0x1F265};
	r[84] = (struct s_codepoint_range){0x1F300, 0x1F320};
	r[85] = (struct s_codepoint_range){0x1F32D, 0x1F335};
	r[86] = (struct s_codepoint_range){0x1F337, 0x1F37C};
	r[87] = (struct s_codepoint_range){0x1F37E, 0x1F393};
	r[88] = (struct s_codepoint_range){0x1F3A0, 0x1F3CA};
	r[89] = (struct s_codepoint_range){0x1F3CF, 0x1F3D3};
	r[90] = (struct s_codepoint_range){0x1F3E0, 0x1F3F0};
	r[91] = (struct s_codepoint_range){0x1F3F4, 0x1F3F4};
	r[92] = (struct s_codepoint_range){0x1F3F8, 0x1F43E};
	r[93] = (struct s_codepoint_range){0x1F440, 0x1F440};
	r[94] = (struct s_codepoint_range){0x1F442, 0x1F4FC};
	r[95] = (struct s_codepoint_range){0x1F4FF, 0x1F53D};
	r[96] = (struct s_codepoint_range){0x1F54B, 0x1F54E};
	r[97] = (struct s_codepoint_range){0x1F550, 0x1F567};
	r[98] = (struct s_codepoint_range){0x1F57A, 0x1F57A};
	r[99] = (struct s_codepoint_range){0x1F595, 0x1F596};
}

void
	wcwidth_table_wide5(struct s_codepoint_range *r)
{
	r[100] = (struct s_codepoint_range){0x1F5A4, 0x1F5A4};
	r[101] = (struct s_codepoint_range){0x1F5FB, 0x1F64F};
	r[102] = (struct s_codepoint_range){0x1F680, 0x1F6C5};
	r[103] = (struct s_codepoint_range){0x1F6CC, 0x1F6CC};
	r[104] = (struct s_codepoint_range){0x1F6D0, 0x1F6D2};
	r[105] = (struct s_codepoint_range){0x1F6D5, 0x1F6D7};
	r[106] = (struct s_codepoint_range){0x1F6DC, 0x1F6DF};
	r[107] = (struct s_codepoint_range){0x1F6EB, 0x1F6EC};
	r[108] = (struct s_codepoint_range){0x1F6F4, 0x1F6FC};
	r[109] = (struct s_codepoint_range){0x1F7E0, 0x1F7EB};
	r[110] = (struct s_codepoint_range){0x1F7F0, 0x1F7F0};
	r[111] = (struct s_codepoint_range){0x1F90C, 0x1F93A};
	r[112] = (struct s_codepoint_range){0x1F93C, 0x1F945};
	r[113] = (struct s_codepoint_range){0x1F947, 0x1F9FF};
	r[114] = (struct s_codepoint_range){0x1FA70, 0x1FA7C};
	r[115] = (struct s_codepoint_range){0x1FA80, 0x1FA89};
	r[116] = (struct s_codepoint_range){0x1FA8F, 0x1FAC6};
	r[117] = (struct s_codepoint_range){0x1FACE, 0x1FADC};
	r[118] = (struct s_codepoint_range){0x1FADF, 0x1FAE9};
	r[119] = (struct s_codepoint_range){0x1FAF0, 0x1FAF8};
	r[120] = (struct s_codepoint_range){0x20000, 0x2FFFD};
	r[121] = (struct s_codepoint_range){0x30000, 0x3FFFD};
}
