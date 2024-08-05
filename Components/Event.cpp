#include "Event.h"

namespace Mio {
    void Event::AddListener(std::string name) {
        funcs.push_back(name);
    }

    void Mio::Event::RemoveListener(std::string name) {
        std::erase(funcs, name);
        Events.erase(name);
    }

    void Mio::Event::BeginFrame() {
        for (const auto&name: funcs) {
            Events[name]();
        }
    }
}
