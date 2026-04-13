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

        std::weak_ptr<Node> parent;
        std::map<std::string, std::shared_ptr<Node>> children;

        Node(std::string name, bool isDirectory, std::shared_ptr<Node> p = nullptr);

        void addChild(std::shared_ptr<Node> child);
        std::string getFullPath() const;
    };

}

#endif //VIRTUALFILES_NODE_H
