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
void handleRm(const std::string&arg, NodePtr& currentDir) {
    if (arg.empty()) {
        std::cout<<"Error: Name required\n";
        return;
    }
    if (currentDir->children.contains(arg)) {
        currentDir->children.erase(arg);
    }else {
        std::cout <<"Error: No such file or directory\n";
    }
}
void handleClear(const std::string&, NodePtr&) {
    std::cout << "\033[2J\033[1;1H";
}
void handleStat(const std::string& arg, NodePtr& currentDir) {
    if (arg.empty() || !currentDir->children.contains(arg)) {
        std::cout<<"Error: Invalid node\n";
        return;
    }
    auto node = currentDir->children[arg];
    std::cout << " File: " << node->name <<"\n";
    std::cout << " Type: " << (node->isDirectory ? "Directory" : "Regular File") << "\n";
    if (!node->isDirectory) {
        std::cout <<" Size: "<<node->content.length()<<" bytes\n";
    }
}
void searchRecursive(NodePtr node, const std::string& target, const std::string& currentPath) {
    if (node->name == target) {
        std::cout<<currentPath << (currentPath == "/" ? "": "/") << node->name <<"\n";
    }
    for (auto const& [name, child] : node->children) {
        searchRecursive(child, target, currentPath == "/" ? "/" : currentPath + "/" + node->name);
    }
}
void handleFind(const std::string& arg, NodePtr& currentDir) {
    if (arg.empty()) return;
    searchRecursive(currentDir, arg, currentDir->getFullPath());
}

void showHelp(const CommandMap& commands) {
    std::cout<<"Available commands: ";
    for (auto it = commands.begin(); it != commands.end(); ++it) {
        std::cout << it->first << (std::next(it) == commands.end() ? "" : ", ");
    }
    std::cout<<", exit\n";
}

int main() {
    const auto root = std::make_shared<VFS::Node>("/", true);
    NodePtr currentDir = root;

    CommandMap commands = {
        {"mkdir",handleMkdir},
        {"touch", handleTouch},
        {"ls", handleLs},
        {"cd", handleCd},
        {"pwd", handlePwd},
        {"rm", handleRm},
        {"clear", handleClear},
        {"stat", handleStat},
        {"find", handleFind}
    };

    std::string line;
    std::cout<<"VFS Simulator (Refactored)\n";
    showHelp(commands);

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