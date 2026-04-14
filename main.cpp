#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <map>
#include <functional>
#include "Node.h"

using CommandMap = std::map<std::string, std::function<void(const std::string&, VFS::Node* &)>>;

void handleMkdir(const std::string& arg, VFS::Node*& currentDir){
    if (arg.empty()) {
        std::cout<<"Error: Directory name is empty\n";
        return;
    }
    VFS::Node* newDir = new VFS::Node(arg, true, currentDir);
    currentDir->addChild(newDir);
}
void handleTouch(const std::string&arg, VFS::Node*& currentDir) {
    if (arg.empty()) {
        std::cout<<"Error: file name is empty\n";
        return;
    }
    VFS::Node* newFile = new VFS::Node(arg, false, currentDir);
    currentDir->addChild(newFile);
}
void handleLs(const std::string&, VFS::Node*& currentDir) {
    for (auto const& [name, node] : currentDir->children) {
        std::cout<<(node->isDirectory ? "[DIR] ":"[FILE] ")<<name<<"\n";
    }
}
void handleCd(const std::string&arg, VFS::Node*& currentDir) {
    if (arg.empty()) {
        std::cout<<"Error: Directory name is empty\n";
    }
    if (arg == "..") {
        if (currentDir->parent != nullptr) {
            currentDir = currentDir->parent;
        }
    }else if (currentDir->children.contains(arg) && currentDir->children[arg]->isDirectory){
        currentDir = currentDir->children[arg];
    }else {
        std::cout<<"Error: Directory not found\n";
    }
}
void handlePwd(const std::string&, VFS::Node*& currentDir) {
    std::cout<<currentDir->getFullPath()<<"\n";
}
void handleRm(const std::string&arg, VFS::Node*& currentDir) {
    if (arg.empty()) {
        std::cout<<"Error: Name required\n";
        return;
    }
    if (currentDir->children.contains(arg)) {
        VFS::Node* toDelete = currentDir->children[arg];
        currentDir->children.erase(arg);
        delete toDelete;
    }else {
        std::cout <<"Error: No such file or directory\n";
    }
}
void handleClear(const std::string&, VFS::Node*&) {
    std::cout << "\033[2J\033[1;1H";
}
void handleStat(const std::string& arg, VFS::Node*& currentDir) {
    if (arg.empty() || !currentDir->children.contains(arg)) {
        std::cout<<"Error: Invalid node\n";
        return;
    }
    VFS::Node* node = currentDir->children[arg];
    std::cout << " Type: " << (node->isDirectory ? "Directory" : "Regular File") << "\n";
    std::cout << " File: " << node->name <<"\n";
    if (!node->isDirectory) {
        std::cout <<" Size: "<<node->content.length()<<" bytes\n";
        std::cout <<" Content: " <<node->content <<"\n";
    }
}
void searchRecursive(VFS::Node* node, const std::string& target, const std::string& currentPath) {
    if (node->name == target) {
        std::cout<<currentPath << (currentPath == "/" ? "": "/") << node->name <<"\n";
    }
    for (auto const& [name, child] : node->children) {
        searchRecursive(child, target, currentPath == "/" ? "/" : currentPath + "/" + node->name);
    }
}
void handleFind(const std::string& arg, VFS::Node*& currentDir) {
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
    VFS::Node* root = new VFS::Node("/",true,nullptr);
    VFS::Node* currentDir = root;

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