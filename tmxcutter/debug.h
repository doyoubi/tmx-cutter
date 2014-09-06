#ifndef DYB_DEBUG
#define DYB_DEBUG

#include <iostream>
#include "point2d.h"

namespace dyb
{
    using std::endl;
    using std::cout;
    using std::cerr;

    
    inline void echoivec2(const point2d<int> & p)
    {
        cout << p.x << ' ' << p.y << endl;
    }

    template<class MSG>
    void debugCheck(bool checkedExpression,
        const char * filename,
        int line,
        MSG errorMsg)
    {
        if (checkedExpression) return;
        std::cerr << filename << " : " << line << endl
            << errorMsg << endl;
        exit(1);
    }

#define DEBUGCHECK(checkedExpression, errMsg) \
    debugCheck(checkedExpression, __FILE__, __LINE__, errMsg)

#define CHECKNULL(ptr) DEBUGCHECK(ptr != nullptr, "null pointer")

#define ERRORMSG(errorMsg) DEBUGCHECK(true, errorMsg) 

}

#endif