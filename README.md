# MUI

<div style="text-align: center;">
    <img src="Img/img.png" alt="Mio UI">
</div>

<div style="text-align: center; font-size: 28px;">Mio UI</div>
<div style="text-align: center;"> simple ECS UI architecture based on imgui</div>

# Features

- [x] Easy to use
- [x] Easy to integrate with other libraries
- [x] Based on ECS architecture
- [ ] Support multi-language
- [ ] Support Editor

# Quick start

1. Add `MUI` to your project

```bash
git submodule add https://github.com/NGLSG/MUI.git
git submodule update --init --recursive
```

```cmake
add_subdirectory(MUI)
target_link_libraries(<your_target> PRIVATE MUI)
```

2. Include Application.h to your project
3. Create a `Mio::Application` instance

```c++
Mio::Application app(windowName,iconPath(optional));
app.Initialize(); 
```

4. Create a `Mio::GUIManifest` object for your GUI

```c++
// if you dont have any saved manifest,you can call like this
auto manifest = Mio::GUIManifest::create(name,SavePathDir);
// if you have saved manifest,you can call like this
auto manifest = ResourceManager::LoadManifest(SavePath+name);
```

5. Create a `Mio::GUIManager` object to manage your GUI

```c++
//if you want to create a window,you should call like this
auto window=Mio::Window::Create({"WindowName",bool ,flags},"UniqueName")
manifest->TryAddManager(window);
//if you had saved manifest,you can call like this
auto window=manifest->GetUI<Mio::Window>("UniqueName");
```

6 Create GUI objects

```c++
//if your first create a GUI object, you should call like this
auto button = Mio::Button::Create({"BTNShowText"});
//if you want to add a callback,you should decalre its name first and then implement it
button->GetComponent<Mio::Event>()->AddListener("CallbackName");
//if you have saved manifest,you can call like this
auto button=manifest->GetUI<Mio::Button>("BTNShowText");
//or you can call like this
auto button=manifest->GetUI<Mio::Button>(UUID);

//implement your callback
Mio::Event::Modify("CallbackName",implementation);

//if you need change the GUI's position,you can call like this
button->transform->cPosition={100,100};
button->transform->cScale={1,1};//not recommend
button->transform->cSize={100,100};

//if you want to change the GUI's style,you can call like this
auto style=button->style->GetStyle();
style.cColor[ImGuiCol_Button]={0.5f,0.5f,0.5f,1.0f}
button->style->Modify(style);//it will save the different of style
//then add GUI to your GUI Manifest
window->AddUIElement(button);

//for more information,you can refer to Application.h and Mio::GUIManifest.h
```

7. Run your application

```c++
app.AddManifest(manifest);
while (!app.ShouldClose()){
    app.Update();
}   
```

8. after Application close

```c++
app.Shutdown();
Mio::ResourceManager::SaveManifest(manifest);
```

9. Use ResourceManager to manager your data

```c++
//Mio::Resource it is a static ResourcePackage instance
//if you need multiple pack you can create it by your self
ResourcePackage pack1("name");//will generate a pack name.pck in build directory when you pack it
//if you need add other files to the single file,you can call like this
Mio::Resource.AddFiles({filePaths});//it will be auto copy to Resources directory
//also you can use Marco
RESOURCING("PATH")//it will return Resources/PATH,and add to Resource pack
//if you need pack to a single file,you can call like this
Mio::Resource.Pack();//will pack Resources directory to a single file and output to build/Resources.pck

// if you want to use this Data
Resource.UnpackAll();//Will unpack all pack to Resources directory
//if you only need one single file
Resource.Unpack("RelativePathForYourFile");//it will unpack to Resources directory with the same name
```
# Example
[MioFramework](https://github.com/NGLSG/MioFramework)