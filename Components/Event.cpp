#include "Event.h"

#include <iostream>

namespace Mio {
    void Event::AddListener(std::string name) {
        if (std::ranges::find(funcs, name) == funcs.end())
            funcs.push_back(name);
    }

    void Mio::Event::RemoveListener(std::string name) {
        std::erase(funcs, name);
        Events.erase(name);
    }

    void Mio::Event::BeginFrame() {
        for (const auto&name: funcs) {
            if (!Events.contains(name)) {
                std::cerr << "There is no implementation for event <" << name << "> in the event system" << std::endl;
                continue;
            }
            Events[name]();
        }
    }
}
