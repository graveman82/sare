#include <windows.h>
#include <vector>
#include <stdint.h>

namespace sare {

const char* Utf8ToAnsi(const char* cstr) {

    uint32_t length = ::MultiByteToWideChar(CP_UTF8, 0, cstr, strlen(cstr) + 1, nullptr, 0);
    static std::vector<wchar_t> wcbuffer;
    wcbuffer.clear();
    wcbuffer.resize(length);

    ::MultiByteToWideChar(CP_UTF8, 0, cstr, strlen(cstr) + 1, &wcbuffer[0], length);

    length = ::WideCharToMultiByte(CP_ACP, 0, &wcbuffer[0], -1, nullptr, 0, nullptr, 0);
    static std::vector<char> cbuffer;
    cbuffer.clear();
    cbuffer.resize(length);

    ::WideCharToMultiByte(CP_ACP, 0, &wcbuffer[0], -1, &cbuffer[0], length, nullptr, 0);
    return &cbuffer[0];
}

} // end of sare
