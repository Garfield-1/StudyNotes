#include <stdio.h>

/*
 * 跳过字符串前方的无用字符，即ACSII码小于32的字符
 */
static char *skip(const char *in) 
{
	while (in && *in && (unsigned char)*in <= 32) {
		in++;
	}

	return in;
}

int main()
{
    char *str = " 1245 ";
    char *str_1 = NULL;

    str_1 = skip(str);

    printf("str_1 = %s\n",str_1);
    return 0;
}

