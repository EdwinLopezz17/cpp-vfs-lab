#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <map>
#include <functional>
#include "Node.h"

using NodePtr = std::shared_ptr<VFS::Node>;
using CommandMap = std::map<std::string, std::function<void(const std::string&, NodePtr&)>>;

void handleMkdir(const std::string& arg, NodePtr& currentDir){
    if (arg.empty()) {
        std::cout<<"Error: Directory name is empty\n";
        return;
    }
    auto newDir = std::make_shared<VFS::Node>(arg, true, currentDir);
    currentDir->addChild(newDir);
}
void handleTouch(const std::string&arg, NodePtr& currentDir) {
    if (arg.empty()) {
        std::cout<<"Error: file name is empty\n";
        return;
    }
    auto newFile = std::make_shared<VFS::Node>(arg, false, currentDir);
    currentDir->addChild(newFile);
}
void handleLs(const std::string&, const NodePtr& currentDir) {
    for (auto const& [name, node] : currentDir->children) {
        std::cout<<(node->isDirectory ? "[DIR] ":"[FILE] ")<<name<<"\n";
    }
}
void handleCd(const std::string&arg, NodePtr& currentDir) {
    if (arg.empty()) {
        std::cout<<"Error: Directory name is empty\n";
    }
    if (arg == "..") {
        if (!currentDir->parent.expired()) {
            currentDir = currentDir->parent.lock();
        }
    }else if (currentDir->children.contains(arg) && currentDir->children[arg]->isDirectory){
        currentDir = currentDir->children[arg];
    }else {
        std::cout<<"Error: Directory not found\n";
    }
}
void handlePwd(const std::string&, const NodePtr& currentDir) {
    std::cout<<currentDir->getFullPath()<<"\n";
}

int main() {
    auto root = std::make_shared<VFS::Node>("/", true);
    NodePtr currentDir = root;

    CommandMap commands = {
        {"mkdir",handleMkdir},
        {"touch", handleTouch},
        {"ls", handleLs},
        {"cd", handleCd},
        {"pwd", handlePwd}
    };

    std::string line;
    std::cout<<"VFS Simulator (Refactored)\n";
    std::cout<<"Commands: mkdir, touch, ls, cd, pwd, exit\n";

    while (true) {
        std::cout << "\033[1;32muser@vfs\033[0m:\033[1;34m" << currentDir->getFullPath() << "\033[0m$ ";

        if (!std::getline(std::cin, line) || line=="exit") break;
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string cmdName, arg;
        ss >> cmdName >> arg;

        if (commands.contains(cmdName)) {
            commands[cmdName](arg, currentDir);
        }else {
            std::cout<<"Command not flound: "<<cmdName<<"\n";
        }
    }

    return 0;
}