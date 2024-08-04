#include "Event.h"

namespace Mio {
    void Event::AddListener(std::string name) {
        funcs.push_back(name);
    }

    void Mio::Event::RemoveListener(std::string name) {
        std::erase(funcs, name);
        Events.erase(name); // 可选：从全局事件映射中移除回调
    }

    void Mio::Event::BeginFrame() {
        for (const auto&name: funcs) {
            Events[name]; // 调用每个注册的回调函数
        }
    }
}
