#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm> // Добавляем этот заголовок для std::remove_if

class FileSystemComponent {
public:
    virtual ~FileSystemComponent() = default;
    virtual void Display(int indent = 0) const = 0;
    virtual int GetSize() const = 0;
    virtual std::string GetName() const = 0;
};

class File : public FileSystemComponent {
    std::string name;
    int size;

public:
    File(const std::string& name, int size) : name(name), size(size) {}

    void Display(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "File: " << name << " (" << size << " KB)" << std::endl;
    }

    int GetSize() const override {
        return size;
    }

    std::string GetName() const override {
        return name;
    }
};

class Directory : public FileSystemComponent {
    std::string name;
    std::vector<std::shared_ptr<FileSystemComponent>> components;

public:
    Directory(const std::string& name) : name(name) {}

    void Display(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "Directory: " << name << std::endl;
        for (const auto& component : components) {
            component->Display(indent + 2);
        }
    }

    int GetSize() const override {
        int totalSize = 0;
        for (const auto& component : components) {
            totalSize += component->GetSize();
        }
        return totalSize;
    }

    std::string GetName() const override {
        return name;
    }

    void Add(const std::shared_ptr<FileSystemComponent>& component) {
        if (!Contains(component->GetName())) {
            components.push_back(component);
        } else {
            std::cout << "Component " << component->GetName() << " already exists in " << name << std::endl;
        }
    }

    void Remove(const std::string& componentName) {
        auto it = std::remove_if(components.begin(), components.end(),
                                 [&componentName](const std::shared_ptr<FileSystemComponent>& component) {
                                     return component->GetName() == componentName;
                                 });
        if (it != components.end()) {
            components.erase(it, components.end());
        } else {
            std::cout << "Component " << componentName << " not found in " << name << std::endl;
        }
    }

    bool Contains(const std::string& componentName) const {
        for (const auto& component : components) {
            if (component->GetName() == componentName) {
                return true;
            }
        }
        return false;
    }
};

// Клиентский код
int main() {
    auto root = std::make_shared<Directory>("root");
    auto docs = std::make_shared<Directory>("Documents");
    auto pics = std::make_shared<Directory>("Pictures");

    auto file1 = std::make_shared<File>("file1.txt", 10);
    auto file2 = std::make_shared<File>("file2.txt", 20);
    auto pic1 = std::make_shared<File>("pic1.jpg", 50);
    auto pic2 = std::make_shared<File>("pic2.jpg", 100);

    docs->Add(file1);
    docs->Add(file2);
    pics->Add(pic1);
    pics->Add(pic2);

    root->Add(docs);
    root->Add(pics);

    std::cout << "File System Structure:\n";
    root->Display();
    std::cout << "\nTotal Size of 'root': " << root->GetSize() << " KB\n";

    return 0;
}

