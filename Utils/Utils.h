#ifndef UTILS_H
#define UTILS_H
#include <codecvt>
#include <cstdint>

namespace Mio {
    inline static std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    class Utils {
    public:
        static uint32_t LoadTexture(const char* filename);

        static void* LoadTextureToRAM(const char* filename);
    };
}

#endif //UTILS_H
