#include "misc.h"
#include <python2.7/Python.h>

std::string misc::percentEncode(const std::string &s)
{
    std::string encoded("");
    for (int i = 0; i < s.length(); i++)
    {
        if ((48 <= s[i] && s[i] <= 57) ||//0-9
            (65 <= s[i] && s[i] <= 90) ||//ABC...XYZ
            (97 <= s[i] && s[i] <= 122) || //abc...xyz
            (s[i] == '~' || s[i] == '-' || s[i] == '_' || s[i] == '.'))
        {
            encoded.append(&s[i], 1);
        }
        else
        {
            encoded.append("%");
            encoded.append(char2hex(s[i]));//converts char 255 to string "FF"
        }
    }
    return encoded;
}

std::string misc::char2hex(char dec)
{
    char dig1 = (dec & 0xF0) >> 4;
    char dig2 = (dec & 0x0F);
    if (0 <= dig1 && dig1 <= 9) dig1 += 48;    //0,48 in ascii
    if (10 <= dig1 && dig1 <= 15) dig1 += 65 - 10; //A,65 in ascii
    if (0 <= dig2 && dig2 <= 9) dig2 += 48;
    if (10 <= dig2 && dig2 <= 15) dig2 += 65 - 10;

    std::string r;
    r.append(&dig1, 1);
    r.append(&dig2, 1);
    return r;
}
