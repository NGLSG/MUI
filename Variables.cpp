#include "Variables.h"

#include <utility>

namespace Mio {
    void Variables::Register(const std::string&name, std::shared_ptr<void> variable) {
        variables[name] = std::move(variable);
    }

    void Variables::Modify(const std::string&name, std::shared_ptr<void> variable) {
        Register(name, variable);
    }

    void Variables::Declare(const std::string&name) {
        if (!variables.contains(name))
            variables[name] = nullptr;
    }

    void Variables::Unregister(std::shared_ptr<void> variable) {
        erase_if(variables, ([&variable](const auto&pair) {
            return pair.second == variable;
        }));
    }

    void Variables::Unregister(const std::string&name) {
        variables.erase(name);
    }
} // Mio
