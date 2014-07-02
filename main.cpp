#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <string>

#include "utf8-util/utf8-util.h"

void examples ()
{
    std::string in = "hello world 你好世界";
    std::string out;

    // iterate over each codepoint in the source utf8 string
    for (char32_t codepoint: utf8::CodepointIterator(in))
    {
        // re-encode the 32 bit codepoint back to utf8
        utf8::encodeCodepoint(codepoint, out);
        // add a space after the character
        out += ' ';
    }

    std::cout << out << std::endl;

    // the same thing can be done with a std::foreach algorithm
    // but its a bit more involved
    auto cpi = utf8::CodepointIterator(in);
    std::for_each(cpi.begin(), cpi.end(), [&](char32_t codepoint)
    {
        utf8::encodeCodepoint(codepoint, out);
        out += ' ';
    });

    std::cout << out << std::endl;
}

int main()
{
    examples();
    return 0;
}
