//Due to solve the problem of some variable can not be saved in the manifest

#ifndef VARIABLES_H
#define VARIABLES_H
#include <map>
#include <memory>
#include <string>

#include "Utils.h"

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif


namespace Mio {
    class Variables {
    public:
        static void Register(const std::string&name, std::shared_ptr<void> variable);

        static void Modify(const std::string&name, std::shared_ptr<void> variable);

        static void Declare(const std::string&name);

        static void Unregister(std::shared_ptr<void> variable);

        static void Unregister(const std::string&name);

        template<typename T>
        static std::shared_ptr<T> Get(std::string name) {
            return std::static_pointer_cast<T>(variables[name]);
        }

    private:
        inline static std::map<std::string, std::shared_ptr<void>> variables;
    };
} // Mio

inline static std::string GetClassVarFullName(const char* prettyFunction, const char* varName, const char* name) {
    if (prettyFunction == nullptr) return "";

    std::string funcStr(prettyFunction);
    size_t lastColonPos = funcStr.rfind("::");
    if (lastColonPos == std::string::npos) return "";

    size_t classStartPos = funcStr.rfind("::", lastColonPos - 2);
    if (classStartPos == std::string::npos) classStartPos = 0;

    size_t classNameLength = lastColonPos - classStartPos - 2;
    auto className = funcStr.substr(classStartPos + 2, classNameLength);
    std::string str(varName);
    size_t lastDotPos = str.find_last_of('.');
    if (lastDotPos == std::string::npos) {
        lastDotPos = 0;
    }
    size_t lastArrowPos = str.find_last_of("->");
    if (lastArrowPos == std::string::npos) {
        lastArrowPos = 0;
    }
    size_t lastPos = max(lastDotPos, lastArrowPos);
    auto newVarName = str.substr(lastPos + 1);

    return className + "<" + name + ">" + "::" + newVarName;
}

#define DECLAREVAR(x,arry) arry.emplace_back(x);\
    Mio::Variables::Declare(x);

#define REGISTERVAR(name,var,arry) arry.emplace_back(GetClassVarFullName(__FUNCTION__,#var,name));\
    Mio::Variables::Register(GetClassVarFullName(__FUNCTION__,#var,name),var);

#define GETVARFULLNAME(name,var) GetClassVarFullName(__FUNCTION__,#var,name)
#endif //VARIABLES_H
