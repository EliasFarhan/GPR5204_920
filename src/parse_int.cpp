#include "parse_int.h"

int parse_int(const char* p)
{

    int sign = 1;
    if (*p == '-')
    {
        sign = -1;
        p++;
    }
    int val = 0;
    while (true)
    {
        const int n = *p;
        if (n >= '0' && n <= '9')
        {
            val *= 10;
            val += n - '0';
            p++;
        }
        else
            break;
    }
    return val * sign;
}

#define code 3
int parse_int_branchless(const char* p)
{
#if code == 1
    const int conditional = *p == '-';
    const int sign = -conditional*2+1;
    p = p + conditional;
#elif code == 2
    static constexpr int sign_array[2] = { -1, 1 };
    static constexpr unsigned ptr_offset[2] = { 0, 1 };
    const int conditional = *p == '-';
    const int sign = sign_array[conditional];
    p = p + ptr_offset[conditional];
#elif code == 3
    static constexpr int sign_array[2] = { -1, 1 };
    const int conditional = *p == '-';
    const int sign = sign_array[conditional];
    if (sign == -1)
    {
        p++;
    }
#elif code == 4
    static constexpr int sign_array[2] = { -1, 1 };
    const int conditional = *p == '-';
    const int sign = sign_array[conditional];
    p = p + conditional;
#endif
    
    int val = 0;
    int is_digit = 1;
    int new_val = 0;
    while (is_digit)
    {
        val = new_val;
        const int n = *p;
        new_val = val * 10 + (n - '0');
        p++;
        is_digit = (n >= '0') && (n <= '9');
    }
    return sign * val;
}