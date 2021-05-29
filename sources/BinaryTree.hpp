#pragma once
#include <iostream>
#include <stack>

namespace ariel
{

    template <typename T>
    class BinaryTree
    {
        class Node
        {
        public:
            Node(const T &val) : val(val), left(nullptr), right(nullptr), father(nullptr) {}
            T val;
            Node *left;
            Node *right;
            Node *father;
        };

        Node *root;
        Node *find(const T &value)
        {

            if (root == nullptr)
            {
                return nullptr;
            }
            std::stack<Node *> nodeStack;
            nodeStack.push(root);
            while (!static_cast<bool>(nodeStack.empty()))
            {
                Node *n = nodeStack.top();
                if (n->val == value)
                {
                    return n;
                }
                nodeStack.pop();
                if (n->right)
                {
                    nodeStack.push(n->right);
                }
                if (n->left)
                {
                    nodeStack.push(n->left);
                }
            }
            return nullptr;
        }
        void delete_tree()
        {
            if (root == nullptr)
            {
                return;
            }
            std::stack<Node *> nodeStack;
            nodeStack.push(root);
            while (!static_cast<bool>(nodeStack.empty()))
            {
                Node *n = nodeStack.top();
                nodeStack.pop();
                if (n->right)
                {
                    nodeStack.push(n->right);
                }
                if (n->left)
                {
                    nodeStack.push(n->left);
                }
                delete n;
            }
        }
        Node *copy(Node *root, Node *parent)
        {
            Node *new_root = nullptr;
            if (root != nullptr)
            {
                new_root = new Node(root->val);
                new_root->father = parent;
                new_root->left = copy(root->left, new_root);
                new_root->right = copy(root->right, new_root);
            }
            else
            {
                return nullptr;
            }
            return new_root;
        }

    public:
        BinaryTree() : root(nullptr) {}
        BinaryTree(const BinaryTree<T> &b)
        {
            if (b.root == nullptr)
            {
                root = nullptr;
            }
            else
            {
                root = copy(b.root, nullptr);
            }
        }
        BinaryTree(BinaryTree<T> &&b) noexcept
        {
            if (b.root == nullptr)
            {
                root = nullptr;
            }
            else
            {
                root = copy(b.root, nullptr);
                b.root = nullptr;
            }
        };
        BinaryTree<T> &add_root(const T &val)
        {
            if (root == nullptr)
            {
                root = new Node(val);
            }
            else
            {
                root->val = val;
            }
            return *this;
        }
        BinaryTree<T> &add_left(const T &place, const T &add)
        {
            if (root == nullptr)
            {
                throw std::invalid_argument("the tree is not initialize");
            }
            Node *n = find(place);
            if (n == nullptr)
            {
                throw std::invalid_argument("no such value in the tree");
            }
            if ((n->left) != nullptr)
            {
                (n->left)->val = add;
            }
            else
            {
                Node *tmp = new Node(add);
                tmp->father = n;
                n->left = tmp;
            }
            return *this;
        }
        BinaryTree<T> &add_right(const T &place, const T &add)
        {
            if (root == nullptr)
            {
                throw std::invalid_argument("the tree is not initialize");
            }
            Node *n = find(place);
            if (n == nullptr)
            {
                throw std::invalid_argument("no such value in the tree");
            }
            if ((n->right) != nullptr)
            {
                (n->right)->val = add;
            }
            else
            {
                Node *tmp = new Node(add);
                tmp->father = n;
                n->right = tmp;
            }
            return *this;
        }

        BinaryTree<T> &operator=(const BinaryTree &b)
        {
            if (this != &b)
            {
                if (root)
                {
                    delete_tree();
                }
                if (b.root == nullptr)
                {
                    root = nullptr;
                }
                else
                {
                    root = copy(b.root, nullptr);
                }
            }
            return *this;
        };
        BinaryTree<T> &operator=(BinaryTree<T> &&b) noexcept
        {
            if (this != &b)
            {
                if (root)
                {
                    delete_tree();
                }
                if (b.root == nullptr)
                {
                    root = nullptr;
                }
                else
                {
                    root = copy(b.root, nullptr);
                    b.root = nullptr;
                }
            }
            return *this;
        }

        ~BinaryTree()
        {
            delete_tree();
        }

        //===============================================================
        class pre_iterator
        {

        private:
            Node *node;
            void preorder_next()
            {
                if ((node->left) != nullptr)
                {
                    node = node->left;
                    return;
                }
                if ((node->right) != nullptr)
                {
                    node = node->right;
                    return;
                }
                Node *tmp = nullptr;
                do
                {
                    tmp = node;
                    node = node->father;
                    if (node == nullptr)
                    {
                        return;
                    }
                } while (node->right == nullptr || (tmp == node->right));
                node = node->right;
            }

        public:
            pre_iterator(Node *ptr = nullptr) : node(ptr) {}
            pre_iterator(const pre_iterator &it) : node(it.node) {}
            pre_iterator(const pre_iterator &&it) noexcept : node(it.node) {}
            pre_iterator &operator=(pre_iterator &&o) noexcept
            {
                if (this != &o)
                {
                    node = o.node;
                    o.node = nullptr;
                }
                return *this;
            }
            ~pre_iterator() {}

            T &operator*() const
            {
                if (node == nullptr)
                {
                    throw std::invalid_argument("try to reach null pointer");
                }
                return node->val;
            }

            T *operator->() const
            {
                if (node == nullptr)
                {
                    return nullptr;
                }
                return &(node->val);
            }

            pre_iterator &operator++()
            {
                if (node == nullptr)
                {
                    throw std::invalid_argument("tree is not initiaized");
                }
                preorder_next();
                return *this;
            }

            pre_iterator operator++(int)
            {
                if (node == nullptr)
                {
                    throw std::invalid_argument("tree is not initiaized");
                }
                pre_iterator tmp = *this;
                preorder_next();
                return tmp;
            }

            bool operator==(const pre_iterator &it) const
            {
                return node == it.node;
            }

            bool operator!=(const pre_iterator &it) const
            {
                return node != it.node;
            }
            pre_iterator &operator=(const pre_iterator &it)
            {
                if (this == &it)
                {
                    return *this;
                }
                node = it.node;
                return *this;
            }
        };
        pre_iterator begin_preorder()
        {
            pre_iterator i(root);
            return i;
        }
        pre_iterator end_preorder()
        {
            return pre_iterator();
        }
        //===============================================================
        class in_iterator
        {

        private:
            Node *node;
            void inorder_next()
            {
                if ((node->right) != nullptr)
                {
                    node = node->right;
                    while ((node->left) != nullptr)
                    {
                        node = node->left;
                    }
                }
                else
                {
                    Node *tmp = nullptr;
                    do
                    {
                        tmp = node;
                        node = node->father;
                        if (node == nullptr)
                        {
                            break;
                        }
                    } while (tmp != node->left);
                }
            }

        public:
            in_iterator(Node *ptr = nullptr) : node(ptr) {}
            in_iterator(const in_iterator &it) : node(it.node) {}
            in_iterator(const in_iterator &&it) noexcept : node(it.node) {}
            in_iterator &operator=(in_iterator &&o) noexcept
            {
                if (this != &o)
                {
                    node = o.node;
                    o.node = nullptr;
                }
                return *this;
            }
            ~in_iterator() {}

            T &operator*() const
            {
                if (node == nullptr)
                {
                    throw std::invalid_argument("try to reach null pointer");
                }
                return node->val;
            }

            T *operator->() const
            {
                if (node == nullptr)
                {
                    return nullptr;
                }
                return &(node->val);
            }

            in_iterator &operator++()
            {
                if (node == nullptr)
                {
                    throw std::invalid_argument("tree is not initiaized");
                }
                inorder_next();
                return *this;
            }

            in_iterator operator++(int)
            {
                if (node == nullptr)
                {
                    throw std::invalid_argument("tree is not initiaized");
                }
                in_iterator tmp = *this;
                inorder_next();
                return tmp;
            }

            bool operator==(const in_iterator &it) const
            {
                return node == it.node;
            }

            bool operator!=(const in_iterator &it) const
            {
                return node != it.node;
            }
            in_iterator &operator=(const in_iterator &it)
            {
                if (this == &it)
                {
                    return *this;
                }
                node = it.node;
                return *this;
            }
            void initial_begin()
            {
                if (node == nullptr)
                {
                    return;
                }
                while (node->left != nullptr)
                {
                    node = node->left;
                }
            }
        };
        in_iterator begin_inorder()
        {
            in_iterator i(root);
            i.initial_begin();
            return i;
        }
        in_iterator end_inorder()
        {
            return in_iterator();
        }
        in_iterator begin()
        {
            in_iterator i(root);
            i.initial_begin();
            return i;
        }
        in_iterator end()
        {
            return in_iterator();
        }
        //===============================================================
        class post_iterator
        {

        private:
            Node *node;
            void postorder_next()
            {
                Node *parent = node->father;
                if (parent == nullptr)
                {
                    node = nullptr;
                }
                else if ((parent->right) == node || (parent->right) == nullptr)
                {
                    node = parent;
                }
                else
                {
                    Node *sibiling = parent->right;
                    while (sibiling != nullptr)
                    {
                        parent = sibiling;
                        if ((sibiling->left) != nullptr)
                        {
                            sibiling = sibiling->left;
                        }
                        else
                        {
                            sibiling = sibiling->right;
                        }
                    }
                    node = parent;
                }
            }

        public:
            post_iterator(Node *ptr = nullptr) : node(ptr) {}
            post_iterator(const post_iterator &it) : node(it.node) {}
            post_iterator(const post_iterator &&it) noexcept : node(it.node) {}
            post_iterator &operator=(post_iterator &&o) noexcept
            {
                if (this != &o)
                {
                    node = o.node;
                    o.node = nullptr;
                }
                return *this;
            }
            ~post_iterator() {}
            T &operator*() const
            {
                if (node == nullptr)
                {
                    throw std::invalid_argument("null pointer");
                }
                return node->val;
            }

            T *operator->() const
            {
                if (node == nullptr)
                {
                    return nullptr;
                }
                return &(node->val);
            }

            post_iterator &operator++()
            {
                if (node == nullptr)
                {
                    throw std::invalid_argument("tree is not initiaized");
                }
                postorder_next();
                return *this;
            }

            post_iterator operator++(int)
            {
                if (node == nullptr)
                {
                    throw std::invalid_argument("tree is not initiaized");
                }
                post_iterator tmp = *this;
                postorder_next();
                return tmp;
            }

            bool operator==(const post_iterator &it) const
            {
                return node == it.node;
            }

            bool operator!=(const post_iterator &it) const
            {
                return node != it.node;
            }
            post_iterator &operator=(const post_iterator &it)
            {
                if (this != &it)
                {
                    node = it.node;
                }
                return *this;
            }
            void initial_begin()
            {
                if (node == nullptr)
                {
                    return;
                }
                while (node->right != nullptr || node->left != nullptr)
                {
                    while (node->left != nullptr)
                    {
                        node = node->left;
                    }
                    if (node->right != nullptr)
                    {
                        node = node->right;
                    }
                }
            }
        };
        post_iterator begin_postorder()
        {
            post_iterator i(root);
            i.initial_begin();
            return i;
        }
        post_iterator end_postorder()
        {
            return post_iterator();
        }

    private:
        void printBT(const std::string &prefix, const Node *node, bool isLeft, std::ostream &out) const
        {
            if (node != nullptr)
            {
                out << prefix;

                if (isLeft)
                {
                    out << "├──";
                }
                else
                {
                    out << "└──";
                }
                out << node->val << std::endl;
                printBT(prefix + (isLeft ? "│   " : "    "), node->right, true, out);
                printBT(prefix + (isLeft ? "│   " : "    "), node->left, false, out);
            }
        }

    public:
        void printBT(std::ostream &out) const
        {
            printBT("", root, false, out);
        }
    };

    template <typename T>
    std::ostream &operator<<(std::ostream &out, const BinaryTree<T> &tree)
    {
        tree.printBT(out);
        return out;
    }
}