#include "Component.h"

#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

namespace Mio {
    UUid UUid::New() {
        UUid uuid;
        static std::mt19937 generator([] {
            std::random_device rd;
            return rd(); // Ensure we get a non-deterministic random seed
        }());

        std::uniform_int_distribution<uint32_t> distribution(0, UINT32_MAX);
        for (auto&part: uuid.data) {
            part = distribution(generator);
        }

        uuid.data[1] &= 0x0FFF;
        uuid.data[1] |= 0x4000; // version 4
        uuid.data[2] &= 0x3FFF;
        uuid.data[2] |= 0x8000; // variant bits


        while (uuids.find(uuid) != uuids.end()) {
            std::cout << "UUID collision detected. Regenerating..." << std::endl;
            uuid = New(); // 如果发生冲突，重新生成
        }
        uuids.insert(uuid);
        return uuid;
    }

    std::string UUid::toString() const {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');

        for (size_t i = 0; i < data.size(); ++i) {
            ss << std::setw(8) << data[i];
            if (i == 3 || i == 5 || i == 7) {
                ss << '-';
            }
        }

        return ss.str();
    }

    bool UUid::operator==(const UUid&other) const {
        return data == other.data;
    }

    bool UUid::operator<(const UUid&other) const {
        return data < other.data;
    }

    std::ostream& UUid::operator<<(std::ostream&os) const {
        return os << toString();
    }

    void Component::Start() {
    }

    void Component::BeginFrame() {
    }

    void Component::Update() {
    }

    void Component::EndFrame() {
    }

    void Component::ShoutDown() {
    }
} // Mio
