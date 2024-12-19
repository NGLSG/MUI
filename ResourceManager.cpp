#include "ResourceManager.h"

#include <Utils.h>

#include "Encryption.h"
#include "Library/RyoCompression/Utils.h"
#include "GUIManifest.h"

namespace Mio
{
    void ResourcePackage::AddFiles(std::vector<std::string> files)
    {
        for (auto& file : files)
        {
            if (std::find(files.begin(), files.end(), file) == files.end())
            {
                this->files.emplace_back(file);
            }
        }
    }

    std::string ResourcePackage::AddFile(std::string file)
    {
        if (file == rootPath)
        {
            return file;
        }
        if (std::find(files.begin(), files.end(), file) == files.end())
        {
            this->files.emplace_back((rootPath / file).string());
            if (RC::Utils::File::Exists(file))
                RC::Utils::Directory::Copy(file, (rootPath / file).string());
        }
        return (rootPath / file).string();
    }

    bool ResourcePackage::Pack()
    {
        if (!RC::Utils::Directory::Exists(rootPath.string()))
        {
            RC::Utils::Directory::Create(rootPath.string());
        }
        for (auto& file : files)
        {
            if (file.find(rootPath.string()) == std::string::npos)
            {
                if (RC::Utils::Directory::Exists(file))
                {
                    RC::Utils::Directory::Copy(file, (rootPath / file).string());
                    file = (rootPath / file).string();
                }
            }
        }
        if (!RC::Compression::Compress(files, path, ComressionType))
        {
            std::cerr << "Failed to compress files" << std::endl;
            return false;
        }
        Encryption::AES::Encrypt(path, PASSWORD, IV, finalpath, true);
        return true;
    }

    bool ResourcePackage::UnpackAll()
    {
        if (!RC::Utils::File::Exists(finalpath))
        {
            return false;
        }
        Encryption::AES::Decrypt(finalpath, PASSWORD, IV, path);
        if (!RC::Compression::Extract(path, "./", ComressionType))
        {
            RC::Utils::File::Remove(path);
            return false;
        }
        RC::Utils::File::Remove(path);
        return true;
    }

    std::string ResourcePackage::Unpack(std::string selectFile)
    {
        Encryption::AES::Decrypt(finalpath, PASSWORD, IV, path);
        if (!RC::Compression::ExtractSelectedFile(path, selectFile, "./"))
        {
            RC::Utils::File::Remove(path);
            return "";
        }
        RC::Utils::File::Remove(path);
        return (rootPath / selectFile).string();
    }

    std::shared_ptr<GUIManifest> ResourceManager::LoadManifest(const std::string& name)
    {
        auto manifest = std::make_shared<GUIManifest>(Load<GUIManifest>(name + ".manifest"));
        for (auto& manager : manifest->sManager)
        {
            UIResources[manifest][manager->cName] = manager;
            for (auto& ui : manager->uiElements)
            {
                UIResources[manifest][ui->cName] = ui;
            }
        }
        Manifests[name] = manifest;
        return std::move(manifest);
    }

    void ResourceManager::LoadManifest(const std::string& name, std::shared_ptr<GUIManifest> manifest)
    {
        auto tmp = Load<GUIManifest>(manifest->SavePath + ".manifest");
        manifest->sName = tmp.sName;
        manifest->sManager = tmp.sManager;
        for (auto& manager : tmp.sManager)
        {
            UIResources[manifest][tmp.sName] = manager;
            for (auto& ui : manager->uiElements)
            {
                UIResources[manifest][ui->cName] = ui;
            }
        }
        Manifests[name] = manifest;
    }

    bool ResourceManager::SaveManifest(const std::shared_ptr<GUIManifest>& manifest)
    {
        return Save<GUIManifest>(*manifest, manifest->SavePath + ".manifest");
    }
}
