#include "Event.h"

#include <iostream>

#include "Utils.h"

namespace Mio {
    void Event::AddListener(std::string name) {
        if (std::find(funcs.begin(), funcs.end(), name) == funcs.end())
            funcs.push_back(name);
    }

    void Mio::Event::RemoveListener(std::string name) {
        funcs.erase(std::remove(funcs.begin(), funcs.end(), name), funcs.end());
        Events.erase(name);
    }

    void Mio::Event::BeginFrame() {
        for (const auto&name: funcs) {
            if (!contains(Events, name)) {
                std::cerr << "There is no implementation for event <" << name << "> in the event system" << std::endl;
                continue;
            }
            Events[name]();
        }
    }
}
