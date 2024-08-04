#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <array>
#include <imgui.h>
#include <memory>
#include <set>
#include <string>
#include <entt/entt.hpp>
#include <optional>

namespace Mio {
    struct UUid {
    public:
        UUid() : data({0}) {
        }

        static UUid New();

        std::string toString() const;

        std::array<uint32_t, 4> GetData() const {
            return data;
        }

        void SetData(std::array<uint32_t, 4> data) {
                this->data = data;
        }

        bool operator==(const UUid&other) const;

        bool operator<(const UUid&other) const;

        std::ostream& operator<<(std::ostream&os) const;

    private:
        inline static std::set<UUid> uuids;
        std::array<uint32_t, 4> data;
    };

    class Component : std::enable_shared_from_this<Component> {
    public:
        friend class UI;

        Component() {
            uuid = UUid::New();
            entity = registry.create();
        }

        template<typename T, typename... Args>
        T& AddComponent(Args... args) {
            registry.emplace<T>(entity, args...);
            return registry.get<T>(entity);
        }

        template<typename T>
        T& GetComponent() {
            return *registry.try_get<T>(entity);
        }

        template<typename T>
        bool HasComponent() {
            return registry.try_get<T>(entity) != std::nullopt;
        }

        template<typename T>
        void RemoveComponent() {
            registry.remove<T>(entity);
        }

        virtual ~Component() = default;

        UUid UID() const { return uuid; }

        virtual void Update();

        virtual void Start();

        virtual void BeginFrame();

        virtual void EndFrame();

        virtual void ShoutDown();

    protected:
        UUid uuid;
        entt::entity entity;
        entt::registry registry;
    };
} // Mio

inline ImVec2 operator+(const ImVec2&lhs, const ImVec2&rhs) {
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

inline ImVec2 operator-(const ImVec2&lhs, const ImVec2&rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

inline ImVec2 operator+=(ImVec2&lhs, const ImVec2&rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

#endif //COMPONENTS_H
