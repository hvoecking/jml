/* string_functions.cc
   Jeremy Barnes, 7 February 2005
   Copyright (c) 2005 Jeremy Barnes.  All rights reserved.
   
   This file is part of "Jeremy's Machine Learning Library", copyright (c)
   1999-2005 Jeremy Barnes.
   
   This program is available under the GNU General Public License, the terms
   of which are given by the file "license.txt" in the top level directory of
   the source code distribution.  If this file is missing, you have no right
   to use the program; please contact the author.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   ---
   
   String manipulation functions.
*/

#include "string_functions.h"
#include <stdarg.h>
#include <stdio.h>
#include "jml/arch/exception.h"
#include <sys/errno.h>
#include <stdlib.h>

using namespace std;


namespace ML {

struct va_ender {
    va_ender(va_list & ap)
        : ap(ap)
    {
    }

    ~va_ender()
    {
        va_end(ap);
    }

    va_list & ap;
};

std::string format(const char * fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    try {
        string result = vformat(fmt, ap);
        va_end(ap);
        return result;
    }
    catch (...) {
        va_end(ap);
        throw;
    }
}

std::string vformat(const char * fmt, va_list ap)
{
    char * mem;
    string result;
    int res = vasprintf(&mem, fmt, ap);
    if (res < 0)
        throw Exception(errno, "vasprintf", "format()");

    try {
        result = mem;
        free(mem);
        return result;
    }
    catch (...) {
        free(mem);
        throw;
    }
}

std::vector<std::string> split(const std::string & str, char c)
{
    vector<string> result;
    size_t start = 0;
    size_t pos = 0;
    while (pos < str.size()) {
        if (str[pos] == c) {
            result.push_back(string(str, start, pos - start));
            start = pos + 1;
        }
        ++pos;
    }

    if (start < str.size())
        result.push_back(string(str, start, pos - start));

    return result;
}

std::string lowercase(const std::string & str)
{
    string result = str;
    for (unsigned i = 0;  i < str.size();  ++i)
        result[i] = tolower(result[i]);
    return result;
}

bool removeIfEndsWith(std::string & str, const std::string & ending)
{
    if (str.rfind(ending) == str.size() - ending.length()) {
        str = string(str, 0, str.size() - ending.length());
        return true;
    }
    
    return false;
}

} // namespace ML