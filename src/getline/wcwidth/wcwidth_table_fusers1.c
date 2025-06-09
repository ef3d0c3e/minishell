/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wcwidth_table_fusers1.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

/**
 * @brief Table for 0-width characters, reference:
 * https://www.unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
 */

/**
 * Generated via the following script:
#!/usr/bin/env python3
def parse_unicode_data(file_path):
    zero_width = []

    with open(file_path, encoding="utf-8") as f:
        for line in f:
            if not line.strip():
                continue
            fields = line.split(";")
            codepoint = int(fields[0], 16)
            category = fields[2]
            if category in ("Mn", "Me", "Cf"):
                zero_width.append(codepoint)

    zero_width.sort()
    return merge_ranges(zero_width)

def merge_ranges(codepoints):
    if not codepoints:
        return []

    ranges = []
    start = prev = codepoints[0]
    for cp in codepoints[1:]:
        if cp == prev + 1:
            prev = cp
        else:
            ranges.append((start, prev))
            start = prev = cp
    ranges.append((start, prev))
    return ranges

def format_c_array(ranges):
    return [f"{{0x{start:X}, 0x{end:X}}}," for start, end in ranges]

def main():
    import sys
    if len(sys.argv) != 2:
        print("Usage: python fuser_table.py UnicodeData.txt")
        return

    ranges = parse_unicode_data(sys.argv[1])
    for line in format_c_array(ranges):
        print(line)

if __name__ == "__main__":
    main()
*/

void
	wcwidth_table_fusers1(struct s_codepoint_range *r)
{
	r[0] = (struct s_codepoint_range){0xAD, 0xAD};
	r[1] = (struct s_codepoint_range){0x300, 0x36F};
	r[2] = (struct s_codepoint_range){0x483, 0x489};
	r[3] = (struct s_codepoint_range){0x591, 0x5BD};
	r[4] = (struct s_codepoint_range){0x5BF, 0x5BF};
	r[5] = (struct s_codepoint_range){0x5C1, 0x5C2};
	r[6] = (struct s_codepoint_range){0x5C4, 0x5C5};
	r[7] = (struct s_codepoint_range){0x5C7, 0x5C7};
	r[8] = (struct s_codepoint_range){0x600, 0x605};
	r[9] = (struct s_codepoint_range){0x610, 0x61A};
	r[10] = (struct s_codepoint_range){0x61C, 0x61C};
	r[11] = (struct s_codepoint_range){0x64B, 0x65F};
	r[12] = (struct s_codepoint_range){0x670, 0x670};
	r[13] = (struct s_codepoint_range){0x6D6, 0x6DD};
	r[14] = (struct s_codepoint_range){0x6DF, 0x6E4};
	r[15] = (struct s_codepoint_range){0x6E7, 0x6E8};
	r[16] = (struct s_codepoint_range){0x6EA, 0x6ED};
	r[17] = (struct s_codepoint_range){0x70F, 0x70F};
	r[18] = (struct s_codepoint_range){0x711, 0x711};
	r[19] = (struct s_codepoint_range){0x730, 0x74A};
	r[20] = (struct s_codepoint_range){0x7A6, 0x7B0};
	r[21] = (struct s_codepoint_range){0x7EB, 0x7F3};
	r[22] = (struct s_codepoint_range){0x7FD, 0x7FD};
	r[23] = (struct s_codepoint_range){0x816, 0x819};
	r[24] = (struct s_codepoint_range){0x81B, 0x823};
}

void
	wcwidth_table_fusers2(struct s_codepoint_range *r)
{
	r[25] = (struct s_codepoint_range){0x825, 0x827};
	r[26] = (struct s_codepoint_range){0x829, 0x82D};
	r[27] = (struct s_codepoint_range){0x859, 0x85B};
	r[28] = (struct s_codepoint_range){0x890, 0x891};
	r[29] = (struct s_codepoint_range){0x897, 0x89F};
	r[30] = (struct s_codepoint_range){0x8CA, 0x902};
	r[31] = (struct s_codepoint_range){0x93A, 0x93A};
	r[32] = (struct s_codepoint_range){0x93C, 0x93C};
	r[33] = (struct s_codepoint_range){0x941, 0x948};
	r[34] = (struct s_codepoint_range){0x94D, 0x94D};
	r[35] = (struct s_codepoint_range){0x951, 0x957};
	r[36] = (struct s_codepoint_range){0x962, 0x963};
	r[37] = (struct s_codepoint_range){0x981, 0x981};
	r[38] = (struct s_codepoint_range){0x9BC, 0x9BC};
	r[39] = (struct s_codepoint_range){0x9C1, 0x9C4};
	r[40] = (struct s_codepoint_range){0x9CD, 0x9CD};
	r[41] = (struct s_codepoint_range){0x9E2, 0x9E3};
	r[42] = (struct s_codepoint_range){0x9FE, 0x9FE};
	r[43] = (struct s_codepoint_range){0xA01, 0xA02};
	r[44] = (struct s_codepoint_range){0xA3C, 0xA3C};
	r[45] = (struct s_codepoint_range){0xA41, 0xA42};
	r[46] = (struct s_codepoint_range){0xA47, 0xA48};
	r[47] = (struct s_codepoint_range){0xA4B, 0xA4D};
	r[48] = (struct s_codepoint_range){0xA51, 0xA51};
	r[49] = (struct s_codepoint_range){0xA70, 0xA71};
	r[50] = (struct s_codepoint_range){0xA75, 0xA75};
}

void
	wcwidth_table_fusers3(struct s_codepoint_range *r)
{
	r[51] = (struct s_codepoint_range){0xA81, 0xA82};
	r[52] = (struct s_codepoint_range){0xABC, 0xABC};
	r[53] = (struct s_codepoint_range){0xAC1, 0xAC5};
	r[54] = (struct s_codepoint_range){0xAC7, 0xAC8};
	r[55] = (struct s_codepoint_range){0xACD, 0xACD};
	r[56] = (struct s_codepoint_range){0xAE2, 0xAE3};
	r[57] = (struct s_codepoint_range){0xAFA, 0xAFF};
	r[58] = (struct s_codepoint_range){0xB01, 0xB01};
	r[59] = (struct s_codepoint_range){0xB3C, 0xB3C};
	r[60] = (struct s_codepoint_range){0xB3F, 0xB3F};
	r[61] = (struct s_codepoint_range){0xB41, 0xB44};
	r[62] = (struct s_codepoint_range){0xB4D, 0xB4D};
	r[63] = (struct s_codepoint_range){0xB55, 0xB56};
	r[64] = (struct s_codepoint_range){0xB62, 0xB63};
	r[65] = (struct s_codepoint_range){0xB82, 0xB82};
	r[66] = (struct s_codepoint_range){0xBC0, 0xBC0};
	r[67] = (struct s_codepoint_range){0xBCD, 0xBCD};
	r[68] = (struct s_codepoint_range){0xC00, 0xC00};
	r[69] = (struct s_codepoint_range){0xC04, 0xC04};
	r[70] = (struct s_codepoint_range){0xC3C, 0xC3C};
	r[71] = (struct s_codepoint_range){0xC3E, 0xC40};
	r[72] = (struct s_codepoint_range){0xC46, 0xC48};
	r[73] = (struct s_codepoint_range){0xC4A, 0xC4D};
	r[74] = (struct s_codepoint_range){0xC55, 0xC56};
	r[75] = (struct s_codepoint_range){0xC62, 0xC63};
}

void
	wcwidth_table_fusers4(struct s_codepoint_range *r)
{
	r[76] = (struct s_codepoint_range){0xC81, 0xC81};
	r[77] = (struct s_codepoint_range){0xCBC, 0xCBC};
	r[78] = (struct s_codepoint_range){0xCBF, 0xCBF};
	r[79] = (struct s_codepoint_range){0xCC6, 0xCC6};
	r[80] = (struct s_codepoint_range){0xCCC, 0xCCD};
	r[81] = (struct s_codepoint_range){0xCE2, 0xCE3};
	r[82] = (struct s_codepoint_range){0xD00, 0xD01};
	r[83] = (struct s_codepoint_range){0xD3B, 0xD3C};
	r[84] = (struct s_codepoint_range){0xD41, 0xD44};
	r[85] = (struct s_codepoint_range){0xD4D, 0xD4D};
	r[86] = (struct s_codepoint_range){0xD62, 0xD63};
	r[87] = (struct s_codepoint_range){0xD81, 0xD81};
	r[88] = (struct s_codepoint_range){0xDCA, 0xDCA};
	r[89] = (struct s_codepoint_range){0xDD2, 0xDD4};
	r[90] = (struct s_codepoint_range){0xDD6, 0xDD6};
	r[91] = (struct s_codepoint_range){0xE31, 0xE31};
	r[92] = (struct s_codepoint_range){0xE34, 0xE3A};
	r[93] = (struct s_codepoint_range){0xE47, 0xE4E};
	r[94] = (struct s_codepoint_range){0xEB1, 0xEB1};
	r[95] = (struct s_codepoint_range){0xEB4, 0xEBC};
	r[96] = (struct s_codepoint_range){0xEC8, 0xECE};
	r[97] = (struct s_codepoint_range){0xF18, 0xF19};
	r[98] = (struct s_codepoint_range){0xF35, 0xF35};
	r[99] = (struct s_codepoint_range){0xF37, 0xF37};
	r[100] = (struct s_codepoint_range){0xF39, 0xF39};
}

void
	wcwidth_table_fusers5(struct s_codepoint_range *r)
{
	r[101] = (struct s_codepoint_range){0xF71, 0xF7E};
	r[102] = (struct s_codepoint_range){0xF80, 0xF84};
	r[103] = (struct s_codepoint_range){0xF86, 0xF87};
	r[104] = (struct s_codepoint_range){0xF8D, 0xF97};
	r[105] = (struct s_codepoint_range){0xF99, 0xFBC};
	r[106] = (struct s_codepoint_range){0xFC6, 0xFC6};
	r[107] = (struct s_codepoint_range){0x102D, 0x1030};
	r[108] = (struct s_codepoint_range){0x1032, 0x1037};
	r[109] = (struct s_codepoint_range){0x1039, 0x103A};
	r[110] = (struct s_codepoint_range){0x103D, 0x103E};
	r[111] = (struct s_codepoint_range){0x1058, 0x1059};
	r[112] = (struct s_codepoint_range){0x105E, 0x1060};
	r[113] = (struct s_codepoint_range){0x1071, 0x1074};
	r[114] = (struct s_codepoint_range){0x1082, 0x1082};
	r[115] = (struct s_codepoint_range){0x1085, 0x1086};
	r[116] = (struct s_codepoint_range){0x108D, 0x108D};
	r[117] = (struct s_codepoint_range){0x109D, 0x109D};
	r[118] = (struct s_codepoint_range){0x135D, 0x135F};
	r[119] = (struct s_codepoint_range){0x1712, 0x1714};
	r[120] = (struct s_codepoint_range){0x1732, 0x1733};
	r[121] = (struct s_codepoint_range){0x1752, 0x1753};
	r[122] = (struct s_codepoint_range){0x1772, 0x1773};
	r[123] = (struct s_codepoint_range){0x17B4, 0x17B5};
	r[124] = (struct s_codepoint_range){0x17B7, 0x17BD};
	r[125] = (struct s_codepoint_range){0x17C6, 0x17C6};
}
