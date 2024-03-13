/*
Purpose:    AVL_tree class declaration.
*/

#ifndef __AVL_TREE_H__
#define __AVL_TREE_H__

/*** Headers ***/
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stddef.h>
#include <stdint.h>
#include <utility>

/*** Classes ***/
class AVL_tree {
public:
    enum class avl_status {
        UNINITIALIZED = -1,
        SUCCESS = 0,
        VALUE_NOT_FOUND,
    };

    AVL_tree() = default;
    AVL_tree(std::pair<uint16_t, uint16_t> const& head_value):
        m_head(new AVL_node(head_value, nullptr, nullptr, nullptr))
    {
    }

    void add(std::pair<uint16_t, uint16_t> const& value);
    avl_status remove(std::pair<uint16_t, uint16_t> const& value);
    bool is_inside(std::pair<uint16_t, uint16_t> const& value) const;

    /**
     * @brief Prints the tree to the console, taking the left values.
     *
     * @note    *Debug function.
     *          *Only looks good for values whose char representation is the size of 1.
     */
    void print_tree();

private:
    class AVL_node {
    public:
        enum class node_status {
            UNINITIALIZED = -1,
            SUCCESS = 0,
            INVALID_HEIGHT,
        };

        AVL_node(std::pair<uint16_t, uint16_t> const& value, AVL_node* left, AVL_node* right,
            AVL_node* parent):
            m_value(new std::pair<uint16_t, uint16_t>(value)),
            m_left(left),
            m_right(right),
            m_parent(parent)
        {
        }
        AVL_node(std::pair<uint16_t, uint16_t> const& value):
            AVL_node(value, nullptr, nullptr, nullptr)
        {
        }
        ~AVL_node() { delete this->m_value; }

        void left(AVL_node* left);
        void right(AVL_node* right);
        void parent(AVL_node* parent);
        void value(std::pair<uint16_t, uint16_t> const& value);
        AVL_node* left();
        AVL_node* right();
        AVL_node* parent();
        std::pair<uint16_t, uint16_t> const& value() const;
        /**
         * @brief Find a node by its value.
         *
         * @param[in] value The node value to search for
         * @param[in] find_parent If true search for the value's potential parent if inserted. @node
         * is always nullptr in that case.
         * @param[out] node The node if found, nullptr otherwise.
         * @param[out] parent Parent of the node or potential parent if @find_parent is true.
         */
        void find(std::pair<uint16_t, uint16_t> const& value, bool find_parent, AVL_node*& node,
            AVL_node*& parent);

        /**
         * @brief Find the minimum node in the tree.
         *
         * @param[out] node The node with the minimum value. Could be the calling node.
         */
        void find_min(AVL_node*& node);

        bool operator<(const AVL_node& other) const;

        static size_t get_height(AVL_node const* node);
        node_status print_nth_depth(size_t depth, size_t height, bool is_empty = false);

    private:
        AVL_node* m_left = nullptr;
        AVL_node* m_right = nullptr;
        AVL_node* m_parent = nullptr;
        std::pair<uint16_t, uint16_t> const* m_value;
    };

    AVL_node* m_head = nullptr;
};

#endif // __AVL_TREE_H__
