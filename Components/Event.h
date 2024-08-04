//
// Created by 92703 on 2024/7/31.
//

#ifndef EVENT_H
#define EVENT_H
#include <functional>
#include <utility>
#include <vector>

#include "Component.h"
#include "yaml-cpp/yaml.h"
using CallbackId = size_t;

namespace Mio {
    class Button;
    class ImageButton;

    class Event final : public Component {
    public:
        friend class YAML::convert<Event>;

        void AddListener(std::string name);

        void RemoveListener(std::string name);

        void BeginFrame() override;

        void SetFuncs(std::vector<std::string> funcs) {
            this->funcs = std::move(funcs);
        }

        std::vector<std::string> GetFuncs() {
            return funcs;
        }

        inline static std::unordered_map<std::string, std::function<void()>> Events = {};

        inline static void Declare(const std::string&name) {
            Events[name] = []() {
            };
        }

        inline static void Register(const std::string&name, std::function<void()> callback) {
            Events[name] = std::move(callback);
        }

        inline static void Unregister(const std::string&name) {
            Events.erase(name);
        }

        inline static void Modify(const std::string&name, std::function<void()> callback) {
            Events[name] = std::move(callback);
        }

    protected:
        std::vector<std::string> funcs;
    };
}


#endif //EVENT_H
