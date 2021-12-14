#ifndef STRINGUTILS_H

#define STRINGUTILS_H

#include <vector>
#include <string>
#include <sstream>

using namespace std;

class StringUtils
{
    public:
        StringUtils();
        virtual ~StringUtils();
        static std::vector<std::string> split(std::string& s, const char delimiter);
        static std::string Concatenate(std::vector<std::string> arrayToConcatenate, int startingPosition, const char glue);

    protected:

    private:
};

#endif // STRINGUTILS_H
