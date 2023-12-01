#include <windows.h>

namespace sare {

//-----------------------------------------------------------------------------
void ErrorMessageToUser(const char* text) {
    ::MessageBoxA(0, text, "Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
}

} // end of sare
