#ifndef UIELEMENTS_H
#define UIELEMENTS_H

#include "Components/Component.h"
#include "Components/Style.h"
#include "Components/Transform.h"
#include "Components/UIBase.h"
#include "Components/UIManager.h"
#include <imgui_node_editor.h>
#include <SDL3/SDL_opengl.h>
#include <yaml-cpp/node/convert.h>

namespace ed = ax::NodeEditor;

namespace Mio {
    struct Node {
        enum class PinType {
            Flow,
            Bool,
            Int,
            Float,
            String,
            Object,
            Function,
            Delegate,
        };

        enum class PinKind {
            Output,
            Input
        };

        enum class NodeType {
            Blueprint,
            Simple,
            Tree,
            Comment,
            Houdini
        };

        struct Pin {
            ed::PinId ID;
            Node *Node;
            std::string Name;
            PinType Type;
            PinKind Kind;

            Pin(int id, const char *name, PinType type) : ID(id), Node(nullptr), Name(name),
                                                          Type(type),
                                                          Kind(PinKind::Input) {
            }
        };

        ed::NodeId ID;
        std::string Name;
        std::vector<Pin> Inputs;
        std::vector<Pin> Outputs;
        ImColor Color;
        NodeType Type;
        ImVec2 Size;

        std::string State;
        std::string SavedState;

        Node(int id, const char *name, ImColor color = ImColor(255, 255, 255)) : ID(id), Name(name),
                                                                                 Color(color),
                                                                                 Type(NodeType::Blueprint),
                                                                                 Size(0, 0) {
        }

        inline static std::map<std::string, std::vector<Node>> Nodes;

        void Build() {
            for (auto &pin: Inputs) {
                pin.Node = this;
                pin.Kind = PinKind::Input;
            }
            for (auto &pin: Outputs) {
                pin.Node = this;
                pin.Kind = PinKind::Output;
            }
        }

        void Update() {
            ed::BeginNode(ID);
            ed::PushStyleColor(ed::StyleColor_NodeBg, Color);
            for (auto &pin: Inputs) {
                ed::BeginPin(pin.ID, ed::PinKind::Input);
                ed::PinPivotAlignment(ImVec2(0.0f, 0.5f));
                ImGui::Text("%s", pin.Name.c_str());
                ed::PopStyleColor();
                ed::EndPin();
            }
            for (auto &pin: Outputs) {
                ed::BeginPin(pin.ID, ed::PinKind::Output);
                ed::PinPivotAlignment(ImVec2(1.0f, 0.5f));
                ImGui::Text("%s", pin.Name.c_str());
                ed::PopStyleColor();
                ed::EndPin();
            }
            ed::PopStyleColor();
            ed::EndNode();
        }
    };

    struct Link {
        ed::LinkId ID;

        ed::PinId StartPinID;
        ed::PinId EndPinID;

        ImColor Color;

        Link(ed::LinkId id, ed::PinId startPinId, ed::PinId endPinId) : ID(id),
                                                                        StartPinID(startPinId),
                                                                        EndPinID(endPinId),
                                                                        Color(255, 255, 255) {
        }
    };

    class NodeEditor : public UIBase {
    public:
        struct Data {
            std::string name;
            std::string savePath;
        };

        static std::shared_ptr<NodeEditor> Create(Data data) {
            return std::make_shared<NodeEditor>(data);
        }

        explicit NodeEditor(Data data);

        void Update() override;

        void Save(const std::string &path);

        void Load(const std::string &path);

    private:
        std::vector<Link> links;
        std::vector<Node> nodes;
        ed::EditorContext *context;
        std::set<ed::NodeId> selectedNodes;
        ed::Config config;
        Data cData;
    };

    class Button : public UIBase {
    public:
        friend class YAML::convert<std::shared_ptr<UIBase>>;

        struct Data {
            std::string name;
            bool autoSize = false;
        };

        static std::shared_ptr<Button> Create(const Data &data) {
            return std::make_shared<Button>(data, data.name);
        }

        Button(Data data, std::string name);

        Data &GetData() {
            return cData;
        }

        void Modify(const Data &data);

        void Update() override;

    protected:
        Data cData;
    };

    class ImageButton : public UIBase {
    public:
        friend class YAML::convert<std::shared_ptr<UIBase>>;

        struct Data {
            std::string imagePath;
            ImVec2 uv0 = ImVec2(0, 0);
            ImVec2 uv1 = ImVec2(1, 1);
            ImVec2 size = ImVec2(0, 0);
            ImVec4 bg_col = ImVec4(0, 0, 0, 0);
            ImVec4 tint_col = ImVec4(1, 1, 1, 1);
        };

        static std::shared_ptr<ImageButton> Create(const Data &data, std::string name) {
            return std::make_shared<ImageButton>(data, name);
        }

        ImageButton(Data data, std::string name);

        Data &GetData() {
            return cData;
        }

        void Modify(const Data &data) {
            cData = data;
        }

        void Update() override;

    private:
        Data cData;
        GLuint textureId;
    };

    class CheckBox : public UIBase {
    public:
        friend class YAML::convert<std::shared_ptr<UIBase>>;

        struct Data {
            std::string name;
            std::shared_ptr<bool> value;
        };

        static std::shared_ptr<CheckBox> Create(const Data &data, std::string name) {
            return std::make_shared<CheckBox>(data, name);
        }

        CheckBox(Data data, std::string name);

        Data &GetData() {
            return cData;
        }

        void Update() override;

        bool GetValue() const;

    private:
        Data cData;
    };

    class Text : public UIBase {
    public:
        friend class YAML::convert<std::shared_ptr<UIBase>>;

        struct Data {
            std::string text;
            ImVec2 text_size = {18, 18};
            ImVec2 outline_thickness = {1, 1};
            ImU32 fill_color = IM_COL32(255, 255, 255, 255);
            ImU32 outline_color = IM_COL32(0, 0, 0, 255);
        };

        static std::shared_ptr<Text> Create(const Data &data, std::string name) {
            return std::make_shared<Text>(data, name);
        }

        Text(Data data, std::string name);

        void Modify(Data data) {
            cData = std::move(data);
        }

        Data &GetData() {
            return cData;
        }

        void Update() override;

    private:
        Data cData;
    };

    class InputText : public UIBase {
    public:
        friend class YAML::convert<std::shared_ptr<UIBase>>;

        struct Data {
            const char *label = "";
            char *buf = new char[8192];
            size_t buf_size = 8192;
            const char *hint = "";
            bool Multiline = false;
            ImGuiInputTextFlags flags = 0;
            void *user_data = nullptr;
        };

        static std::shared_ptr<InputText> Create(const Data &data, std::string name) {
            return std::make_shared<InputText>(data, name);
        }

        InputText(const Data &data, std::string name);

        void Modify(const Data &data);

        Data &GetData() {
            return cData;
        }

        std::string GetValue() const;

        void Update() override;

    private:
        Data cData;
    };

    class Image : public UIBase {
    public:
        friend class YAML::convert<std::shared_ptr<UIBase>>;

        struct Data {
            std::string imagePath;
            ImVec2 uv0 = ImVec2(0, 0);
            ImVec2 uv1 = ImVec2(1, 1);
            int frame_padding = -1;
            ImVec4 bg_col = ImVec4(0, 0, 0, 0);
            ImVec4 tint_col = ImVec4(1, 1, 1, 1);
            ImVec4 border_col = ImVec4(0, 0, 0, 0);
        };

        static std::shared_ptr<Image> Create(const Data &data, std::string name) {
            return std::make_shared<Image>(data, name);
        }

        Image(Data data, std::string name);

        void Modify(const Data &data);

        void Update() override;

        Data &GetData() {
            return cData;
        }

    private:
        Data cData;
        GLuint textureId;
    };

    class RadioButton : public UIBase {
    public:
        friend class YAML::convert<std::shared_ptr<UIBase>>;

        struct Data {
            std::string name;
            std::shared_ptr<int> v = std::make_shared<int>();
            bool autoSize = false;
            int v_button = 0;
        };

        static std::shared_ptr<RadioButton> Create(const Data &data, std::string name) {
            return std::make_shared<RadioButton>(data, name);
        }

        RadioButton(Data data, std::string name);

        void Modify(const Data &data);

        void Update() override;

        Data &GetData() {
            return cData;
        }

    private:
        Data cData;
    };

    class Combo : public UIBase {
    public:
        friend class YAML::convert<std::shared_ptr<UIBase>>;

        struct Data {
            std::string name;
            const char *preview_value = "";
            std::vector<std::string> items;
            int *current_item;
            ImGuiComboFlags flags = 0;
        };

        static std::shared_ptr<Combo> Create(const Data &data, std::string name) {
            return std::make_shared<Combo>(data, name);
        }

        Combo(Data data, std::string name);

        void Modify(const Data &data);

        void Update() override;

        int GetValue() const {
            return *cData.current_item;
        }

        Data &GetData() {
            return cData;
        }

    private:
        Data cData;
    };

    class Slider : public UIBase {
    public:
        friend class YAML::convert<std::shared_ptr<UIBase>>;

        enum class Type {
            Float,
            Int,
            Angle,
        };

        struct Data {
            const char *label = "";
            Type type;
            std::shared_ptr<void> v = std::make_shared<float>();
            int components = 0;
            float v_min;
            float v_max;
            const char *format = "%.2f";
            ImGuiSliderFlags flags = 0;
        };

        static std::shared_ptr<Slider> Create(const Data &data, std::string name) {
            return std::make_shared<Slider>(data, name);
        }

        Slider(Data data, std::string name);

        void Modify(const Data &data);

        void Update() override;

        float GetValue() const;

        Data &GetData() {
            return cData;
        }

    private:
        Data cData;
    };

    class ColorPicker : public UIBase {
    public:
        friend class YAML::convert<std::shared_ptr<UIBase>>;

        struct Data {
            std::string name;
            std::shared_ptr<ImVec4> v = std::make_shared<ImVec4>();
            ImGuiColorEditFlags flags = 0;
            ImVec4 ref_col = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        };

        static std::shared_ptr<ColorPicker> Create(const Data &data, std::string name) {
            return std::make_shared<ColorPicker>(data, name);
        }

        ColorPicker(Data data, std::string name);

        void Modify(const Data &data);

        void Update() override;

        ImVec4 GetValue() const;

        Data &GetData() {
            return cData;
        }

    private:
        Data cData;
    };

    class ColorEditor : public UIBase {
    public:
        friend class YAML::convert<std::shared_ptr<UIBase>>;

        struct Data {
            std::string name;
            std::shared_ptr<ImVec4> v = std::make_shared<ImVec4>();
            ImGuiColorEditFlags flags = 0;
        };

        static std::shared_ptr<ColorEditor> Create(const Data &data, std::string name) {
            return std::make_shared<ColorEditor>(data, name);
        }

        ColorEditor(Data data, std::string name);

        void Modify(const Data &data);

        void Update() override;

        ImVec4 GetValue() const;

        Data &GetData() {
            return cData;
        }

    private:
        Data cData;
    };

    class ListBox : public UIBase {
    public:
        friend class YAML::convert<ListBox>;

        friend class YAML::convert<std::shared_ptr<UIBase>>;

        struct Data {
            std::string name;
            const char *preview_value = "";
            std::vector<std::string> items;
            int current_item = 0;
            ImGuiComboFlags flags = 0;
        };

        static std::shared_ptr<ListBox> Create(const Data &data, std::string name) {
            return std::make_shared<ListBox>(data, name);
        }

        ListBox(Data data, std::string name);

        void Modify(const Data &data);

        void Update() override;

        int GetValue() const {
            return cData.current_item;
        }

        std::string GetSelectedItem() const {
            return cData.items[cData.current_item];
        }

        Data &GetData() {
            return cData;
        }

    private:
        Data cData;
    };

    class ProgressBar : public UIBase {
    public:
        friend class YAML::convert<std::shared_ptr<UIBase>>;

        struct Data {
            std::shared_ptr<float> progress = std::make_shared<float>(0.f);
            const char *overlay = "";
        };

        static std::shared_ptr<ProgressBar> Create(const Data &data, std::string name) {
            return std::make_shared<ProgressBar>(data, name);
        }

        ProgressBar(Data data, std::string name);

        void Modify(const Data &data);

        void Update() override;

        Data &GetData() {
            return cData;
        }

    private:
        Data cData;
    };


    class MenuItem : public UIBase {
    public:
        friend class YAML::convert<std::shared_ptr<UIBase>>;

        friend class YAML::convert<std::shared_ptr<MenuItem>>;

        struct Data {
            std::string name;
            bool enabled = true;
            const char *shortcut = "";
        };

        static std::shared_ptr<MenuItem> Create(const Data &data, std::string name) {
            return std::make_shared<MenuItem>(data, name);
        }

        MenuItem(Data data, std::string name);

        void Modify(const Data &data);

        void Update() override;

        Data &GetData() {
            return cData;
        }

    private:
        Data cData;
    };

    class TreeNode : public UIManager {
    public:
        friend class YAML::convert<std::shared_ptr<UIBase>>;

        struct Data {
            std::string name;
            ImGuiTreeNodeFlags flags = 0;
        };

        static std::shared_ptr<TreeNode> Create(const Data &data, std::string name) {
            return std::make_shared<TreeNode>(data, name);
        }

        TreeNode(Data data, std::string name);

        void Modify(const Data &data);

        void Update() override;


        Data &GetData() {
            return cData;
        }

    private:
        Data cData;
    };

    class Group : public UIManager {
    public:
        friend class YAML::convert<std::shared_ptr<UIBase>>;

        struct Data {
            std::string name;
        };

        static std::shared_ptr<Group> Create(const Data &data) {
            return std::make_shared<Group>(data, data.name);
        }

        Group(Data data, std::string name);

        void Modify(const Data &data);

        void Update() override;

        Data &GetData() {
            return cData;
        }

    private:
        Data cData;
    };

    class Console : public UIBase {
    public:
        friend class YAML::convert<std::shared_ptr<UIBase>>;

        struct LogData {
            enum Level {
                LogDebug,
                LogTrace,
                LogInfo,
                LogWarning,
                LogError,
                LogFatal
            };

            std::string message;
            Level level;
            std::string time;

            LogData(const std::string &message, const Level level = LogInfo) {
                this->message = message;
                this->level = level;
                time = GetCurrentTime();
            }

        private:
            inline static std::string GetCurrentTime() {
                std::time_t now = std::time(nullptr);
                std::tm *localTime = std::localtime(&now);
                char buffer[20];
                std::strftime(buffer, sizeof(buffer), "%H:%M:%S", localTime);
                return std::string(buffer);
            }
        };

        struct Data {
            std::string name;
            std::vector<LogData> items;
        };

        static std::shared_ptr<Console> Create(const Data &data, std::string name) {
            return std::make_shared<Console>(data, name);
        }

        Console(Data data, std::string name);

        void Modify(const Data &data);

        void Update() override;

        void AddLog(const LogData &log);

        Data &GetData() {
            return cData;
        }

    private:
        Data cData;
        std::vector<LogData> copied;
        char *buf = nullptr;
    };

    class Popup : public UIManager {
    public:
        friend class YAML::convert<std::shared_ptr<UIBase>>;

        struct Data {
            std::string name;
            ImGuiWindowFlags flags = 0;
        };

        static std::shared_ptr<Popup> Create(const Data &data) {
            return std::make_shared<Popup>(data, data.name);
        }

        Popup(Data data, std::string name);

        void Open() {
            open = true;
        }

        void Close() {
            open = false;
        }

        void Modify(const Data &data);

        void Update() override;


        Data &GetData() {
            return cData;
        }

    private:
        Data cData;
        bool open = false;
    };

    class Tooltip : public UIManager {
    public:
        friend class YAML::convert<std::shared_ptr<UIBase>>;

        struct Data {
            std::string name;
        };

        static std::shared_ptr<Tooltip> Create(const Data &data, std::string name) {
            return std::make_shared<Tooltip>(data, name);
        }

        Tooltip(Data data, std::string name);

        void Modify(const Data &data);

        void Update() override;


        Data &GetData() {
            return cData;
        }

    private:
        Data cData;
    };

    class SubWindow : public UIManager {
    public:
        friend class YAML::convert<std::shared_ptr<UIBase>>;

        struct Data {
            std::string name = "";
            ImVec2 size;
            ImGuiChildFlags child_flags;
            ImGuiWindowFlags window_flags;
        };

        static std::shared_ptr<SubWindow> Create(const Data &data, std::string name) {
            return std::make_shared<SubWindow>(data, name);
        }

        SubWindow(Data data, std::string name);

        void Modify(const Data &data);

        void Update() override;


        Data &GetData() {
            return cData;
        }

    private:
        Data cData;
    };

    class Window : public UIManager {
    public:
        friend class YAML::convert<std::shared_ptr<UIBase>>;

        struct Data {
            std::string name = "";
            bool p_open = NULL;
            ImGuiWindowFlags flags = 0;
            bool fullScreen = false;
        };

        static std::shared_ptr<Window> Create(const Data &data, std::string name) {
            return std::make_shared<Window>(data, name);
        }

        Window(Data data, std::string name);

        void Modify(const Data &data);

        void Update() override;

        Data &GetData() {
            return cData;
        }

    private:
        Data cData;
    };

    class MenuBar : public UIManager {
    public:
        friend class YAML::convert<MenuBar>;

        friend class YAML::convert<std::shared_ptr<UIBase>>;

        friend class YAML::convert<std::shared_ptr<UIManager>>;

        struct Data {
            std::string name;
            bool enabled = true;
        };

        static std::shared_ptr<MenuBar> Create(const Data &data, std::string name) {
            return std::make_shared<MenuBar>(data, name);
        }

        MenuBar(Data data, std::string name);

        void Modify(const Data &data);

        void Update() override;

        void AddMenuItem(const std::shared_ptr<MenuItem> &menuItem);

        void RemoveMenuItem(UUid uuid);

        Data &GetData() {
            return cData;
        }

    protected:
        Data cData;
        std::vector<std::shared_ptr<MenuItem>> menuItems;
    };
} // Mio

#endif //UIELEMENTS_H
