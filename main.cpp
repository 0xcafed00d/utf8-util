#include <iostream>
#include <vector>
#include <algorithm>
#include "utf8-util/utf8-util.h"

void testUTF8 ()
{
    std::string s = "hello world 你好世界";
    std::string s2 = "你好世界";
    std::vector<unsigned char> vs = {0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x20, 0xe4, 0xbd, 0xa0, 0xe5, 0xa5, 0xbd, 0xe4, 0xb8, 0x96, 0xe7, 0x95, 0x8c};

    auto c = utf8::CodepointIterator(s2).begin();
    std::cout << *c << std::endl;
    std::cout << *c << std::endl;
    std::cout << *c << std::endl;

    std::cout << utf8::isValidUTF8(vs) << std::endl;

    for (auto cp: utf8::CodepointIterator(s))
    {
        std::cout << cp << std::endl;
    }

    auto cpi = utf8::CodepointIterator(s);
    std::for_each(cpi.begin(), cpi.end(), [](char32_t cp)
    {
        std::cout << cp << std::endl;
    });
}

int main()
{
    testUTF8();
    return 0;
}
