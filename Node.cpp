#include "Node.h"
#include <iostream>
namespace VFS {
    Node::Node(std::string name, bool isDirectory, Node* parent) {
        this->name = name;
        this->isDirectory = isDirectory;
        this->parent = parent;
    }
    Node::~Node() {
        for (auto& pair : children) {
            delete pair.second;
        }
        children.clear();
    }

    void Node::addChild(Node* child) {
        if (isDirectory &&  child != nullptr) {
            child->parent = this;
            children[child->name] = child;
        }
    }

    std::string Node::getFullPath() const{
        if (parent == nullptr) {
            return name;
        }

        std::string parentPath = parent->getFullPath();
        if (parentPath == "/") return "/" + name;
        return parentPath + "/" + name;
    }
    void Node::setContent(const std::string& newContent) {
        if (!isDirectory) {
            content = newContent;
        }else {
            std::cout << "Error: Cannot write content to a directory.\n";
        }
    }
}
