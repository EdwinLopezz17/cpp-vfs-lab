//
// Created by edwin on 4/12/26.
//

#include "Node.h"

namespace VFS {
    Node::Node(std::string name, bool isDirectory, std::shared_ptr<Node> parent) {
        this->name = name;
        this->isDirectory = isDirectory;
        this->parent = parent;
    }

    void Node::addChild(std::shared_ptr<Node> child) {
        if (isDirectory) {
            child->parent = shared_from_this();
            children[child->name] = child;
        }
    }

    std::string Node::getFullPath() const {
        if (parent.expired()) {
            return name;
        }
        std::shared_ptr<Node> p = parent.lock();
        std::string parentPath = p-> getFullPath();

        if (parentPath == "/") return "/"+name;
        return parentPath + "/" + name;
    }


} // VFS