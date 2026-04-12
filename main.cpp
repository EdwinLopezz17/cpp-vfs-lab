#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include "Node.h"

int main() {

    auto root = std::make_shared<VFS::Node>("/", true);

    std::shared_ptr<VFS::Node> currentDir = root;

    std::string line;
    std::cout<<"VFS simulator in c++\n";
    std::cout<<"Commands: mkdir, touch, ls, cd, pwd, exit\n";

    while (true) {
        std::cout<<"\033[1;32muser@vfs\033[0m:\033[1;34m" << currentDir->getFullPath() << "\033[0m$ ";

        if (!std::getline(std::cin, line) || line == "exit") break;

        std::stringstream ss(line);
        std::string command, arg;
        ss >> command >> arg;

        if (command == "mkdir") {
            if (arg.empty()) {
                std::cout<<"Error: Directory name is empty\n";
            }else {
                auto mewDir = std::make_shared<VFS::Node>(arg, true, currentDir);
                currentDir->addChild(mewDir);
            }
        }
        else if (command == "touch") {
            if (arg.empty()) {
                std::cout<<"Error: File name is empty\n";
            }else {
                auto newFile = std::make_shared<VFS::Node>(arg, false, currentDir);
                currentDir->addChild(newFile);
            }
        }else if (command == "ls") {
            for (auto const& [name, node]: currentDir->children) {
                if (node->isDirectory)std::cout<<"[DIR] "<<name<<"\n";
                else std::cout<<"[FILE] "<<name<<"\n";
            }
        }
        else if (command == "cd") {
            if (arg == "..") {
                if (!currentDir->parent.expired()) {
                    currentDir = currentDir->parent.lock();
                }
            }else if (currentDir->children.count(arg)&& currentDir->children[arg]->isDirectory) {
                currentDir = currentDir->children[arg];
            }else {
                std::cout<<"Error: Directory not found\n";
            }
        }
        else if (command == "pwd") {
            std::cout<<currentDir->getFullPath()<<"\n";
        }
        else if (!command.empty()) {
            std::cout<<"Command not found\n";
        }
    }
    return 0;
}
