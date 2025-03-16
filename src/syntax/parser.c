#include "token.h"


#include <string.h>

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

int utf8size(const char *str)
{
    int utf8charcount = 0;
    int size = strlen(str);

    for (int i = 0; i < size; utf8charcount++ )
    {
        if ( ~(str[i]) & 0x80 )
        {
            i+=1;
        } 
        else if ( ~(str[i] & 0xE0) & 0x20 )
        {
            if ( ~(str[i + 1] & 0xB0) & 0x40 )
                i+=2;
            else
                break; /* bad character */
        }
        else if ( ~(str[i] & 0xF0) & 0x10 )
        {
            if ( i + 2 < size )
            {
                if ( ( ~(str[i + 1] & 0xB0) & 0x40 ) && ( ~(str[i + 2] & 0xB0) & 0x40 ) )
                    i+=3;
                else
                    break; /* bad char */
            }
            else
            {
                break; /* past \x0 */
            }
        }
        else if ( ~(str[i] & 0xF8) & 0x08 )
        {
            if ( i + 3 < size )
            {
                if ( ( ~(str[i + 1] & 0xB0) & 0x40 ) && ( ~(str[i + 2] & 0xB0) & 0x40 ) && ( ~(str[i + 3] & 0xB0) & 0x40 ))
                    i+=4;
                else
                    break; /* bad char */
            }
            else
            {
                break; /* past \x0 */
            }
        }
    }

    return utf8charcount;
}

t_ast_leaf ms_parse(const t_string_view sv)
{
	
}

int main()
{
	const char *s = "echo 's'";
	
	t_string_view tok = {
		.s = s,
		.len = strlen(s),
	};

	t_ast_leaf head = ms_parse(tok);
	asm("int $3");
}
