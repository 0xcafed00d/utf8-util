
#include "utf8-util.h"

namespace utf8
{
    namespace impl
    {
        int processLeading (char c, Codepoint_t& initialcp)
        {
            if ((c & 0x80) == 0)
            {
                initialcp = c;
                return  0;
            }

            if ((c & 0xe0) == 0xc0)
            {
                initialcp = c & 0x1f;
                return 1;
            }
            else if ((c & 0xf0) == 0xe0)
            {
                initialcp = c & 0x0f;
                return 2;
            }
            else if ((c & 0xf8) == 0xf0)
            {
                initialcp = c & 0x07;
                return 3;
            }
            else
            {
                return -1;
            }
        }

        int countContinuations (char c)
        {
            if ((c & 0x80) == 0)
                return  0;
            else if ((c & 0xe0) == 0xc0)
                return 1;
            else if ((c & 0xf0) == 0xe0)
                return 2;
            else if ((c & 0xf8) == 0xf0)
                return 3;
            else
                return -1;
        }

        bool isContinuation (char c)
        {
            return (c & 0xc0) == 0x80;
        }

        bool isValidUTF8 (const char* str)
        {
            while (*str)
            {
                int cc = countContinuations(*str);
                if (cc < 0)
                    return false;

                str++;
                while (cc)
                {
                    if (*str && isContinuation(*str))
                    {
                        str++;
                        cc--;
                    }
                    else
                        return false;
                }
            }
            return true;
        }


        bool addContinuation (char c, Codepoint_t& cp)
        {
            if ((c & 0xc0) == 0x80)
            {
                cp = (cp << 6) + (c & 0x3f);
                return true;
            }
            return false;
        }        
    }
}

