#ifndef VIRTUALFILES_NODE_H
#define VIRTUALFILES_NODE_H

#include <string>
#include <map>
#include <memory>

namespace VFS {
    class Node : public std::enable_shared_from_this<Node> {
    public:
        std::string name;
        bool isDirectory;
        std::string content;
        Node* parent;

        std::map<std::string, Node*> children;

        Node(std::string name, bool isDirectory, Node* parent = nullptr);
        ~Node();

        void addChild(Node* child);
        std::string getFullPath() const;
    };
}

#endif
