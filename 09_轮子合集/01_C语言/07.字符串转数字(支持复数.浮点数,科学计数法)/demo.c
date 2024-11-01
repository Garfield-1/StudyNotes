#include <stdio.h>
#include <math.h>

/*
 * 将字符串解析为一个数字，同时支持复数，浮点数，可解析科学记数法的特性
 */
static const double parse_number(const char *num)
{
	double n = 0;
    double sign = 1;
    double scale = 0;
    int subscale = 0;
    int signsubscale = 1;

    /* Has sign? */
	if (*num == '-') {
        sign = -1,num++;
    }

    /* is zero */
	if (*num == '0') {
        num++;
    }

    /* Number? */
	if (*num >= '1' && *num <= '9')	{
        do {
            n = (n*10.0) + (*num++ -'0');
        } while (*num >= '0' && *num <= '9');
    }

    /* Fractional part? */
	if (*num == '.' && num[1] >= '0' && num[1] <= '9') {
        num++;
        do {
            n = (n * 10.0) + (*num++ -'0'),scale--;
        } while (*num >= '0' && *num <= '9');
    }

    /* Exponent? */
	if (*num == 'e' || *num == 'E') {
		num++;
        if (*num == '+') {
            num++;
        } else if (*num == '-') {
            signsubscale = -1,num++;
        }
        /* Number? */
		while (*num >= '0' && *num <= '9') {
            subscale = (subscale*10)+(*num++ - '0');	
        }
	}

    /* number = +/- number.fraction * 10^+/- exponent */
	n = sign * n * pow(10.0, (scale+subscale * signsubscale));	

	return n;
}

int main()
{
    char *str = "-1.23e10";
    double num = -1;

    num = parse_number(str);

    printf("num = %f\n", num);

    return 0;
}

