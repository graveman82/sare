#include "assert.h"

#include <windows.h>
#include <stdio.h>

namespace boost {

//-----------------------------------------------------------------------------
void assertion_failed(char const * expr, char const * function,
    char const * file, long line) {
    char message[1024] = {};
    sprintf(message, "In \"%s\"(%d)\n"
            "Exp: %s\n"
            "Function: %s\n",
            file, (int)line, expr, function);
    ::MessageBoxA(0, message, "Assert failed", MB_OK | MB_ICONERROR | MB_TOPMOST);
}


//-----------------------------------------------------------------------------
void assertion_failed_msg(char const * expr, char const * msg,
    char const * function, char const * file, long line) {
    char message[1024] = {};
    sprintf(message, "In \"%s\"(%d)\n"
            "Exp: %s\n"
            "Function: %s\n"
             "Comment: %s",
            file, (int)line, expr, function, msg);
    ::MessageBoxA(0, message, "Assert failed", MB_OK | MB_ICONERROR | MB_TOPMOST);
}

} // end of boost
