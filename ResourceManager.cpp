#include "ResourceManager.h"
#include "Encryption.h"
#include "Library/RyoCompression/Utils.h"

namespace Mio {
    void ResourcePackage::AddFiles(std::vector<std::string> files) {
        for (auto&file: files) {
            if (std::ranges::find(files, file) == files.end()) {
                this->files.emplace_back(file);
            }
        }
    }

    std::string ResourcePackage::AddFile(std::string file) {
        if (std::ranges::find(files, file) == files.end()) {
            this->files.emplace_back(file);
        }
        return (ResourcePath / file).string();
    }

    bool ResourcePackage::Pack() {
        if (!RC::Utils::Directory::Exists(ResourcePath.string())) {
            RC::Utils::Directory::Create(ResourcePath.string());
        }
        for (auto&file: files) {
            if (file != ResourcePath) {
                RC::Utils::Directory::Copy(file, (ResourcePath / file).string());
                file = (ResourcePath / file).string();
            }
        }
        if (!RC::Compression::Compress(files, path, ComressionType)) {
            return false;
        }
        Encryption::AES::Encrypt(path, PASSWORD, IV, finalpath, true);
        return true;
    }

    bool ResourcePackage::UnpackAll() {
        if (!RC::Utils::File::Exists(finalpath)) {
            return false;
        }
        Encryption::AES::Decrypt(finalpath, PASSWORD, IV, path);
        if (!RC::Compression::Extract(path, "./", ComressionType)) {
            RC::Utils::File::Remove(path);
            return false;
        }
        RC::Utils::File::Remove(path);
        return true;
    }

    std::string ResourcePackage::Unpack(std::string selectFile) {
        Encryption::AES::Decrypt(finalpath, PASSWORD, IV, path);
        if (!RC::Compression::ExtractSelectedFile(path, selectFile, "./")) {
            RC::Utils::File::Remove(path);
            return "";
        }
        RC::Utils::File::Remove(path);
        return (ResourcePath / selectFile).string();
    }

    std::shared_ptr<GUIManifest> ResourceManager::LoadManifest(const std::string&name) {
        auto manifest = std::make_shared<GUIManifest>(Load<GUIManifest>(name + ".manifest"));
        for (auto&manager: manifest->sManager) {
            UIResources[manifest][manager->cName] = manager;
            for (auto&ui: manager->uiElements) {
                UIResources[manifest][ui->cName] = ui;
            }
        }
        Manifests[name] = manifest;
        return std::move(manifest);
    }

    void ResourceManager::LoadManifest(const std::string&name, std::shared_ptr<GUIManifest> manifest) {
        auto tmp = Load<GUIManifest>(manifest->SavePath + ".manifest");
        manifest->sName = tmp.sName;
        manifest->sManager = tmp.sManager;
        for (auto&manager: tmp.sManager) {
            UIResources[manifest][tmp.sName] = manager;
            for (auto&ui: manager->uiElements) {
                UIResources[manifest][ui->cName] = ui;
            }
        }
        Manifests[name] = manifest;
    }

    bool ResourceManager::SaveManifest(const std::shared_ptr<GUIManifest>&manifest) {
        return Save<GUIManifest>(*manifest, manifest->SavePath + ".manifest");
    }

    Manifests ResourceManager::LoadManifests(std::string path) {
        return Load<Mio::Manifests>(path);
    }
}
