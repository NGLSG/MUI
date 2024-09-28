#ifndef MUTILS_H
#define MUTILS_H
#include <codecvt>
#include <cstdint>
#include <map>
#include <unordered_map>
#include <utility>

namespace Mio {
    inline static std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    class Utils {
    public:
        static uint32_t LoadTexture(const char* filename);

        static void* LoadTextureToRAM(const char* filename);
    };
#if __cplusplus <= 201703L or _MSVC_LANG <= 201703L
    template<typename C, typename Pred>
    static void erase_if(C& c, Pred p) {
        for (auto it = c.begin(); it != c.end(); ) {
            if (p(*it)) {
                it = c.erase(it);
            } else {
                ++it;
            }
        }
    }

    template<typename K, typename V>
    static bool contains(const std::unordered_map<K, V>&map, const K&key) {
        return map.find(key) != map.end();
    }

    template<typename K, typename V>
    static bool contains(const std::map<K, V>&map, const K&key) {
        return map.find(key) != map.end();
    }

#elif __cplusplus > 201703L or _MSVC_LANG > 201703L

#endif
}

#endif //UTILS_H
