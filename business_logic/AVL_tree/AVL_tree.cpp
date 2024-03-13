/*
Purpose:    AVL_tree implementation.
*/

/*** Headers ***/
#include "AVL_tree.h"

/*** Constants ***/
const char FILLER_CHAR = ' ';
const char BRANCH_CHAR = '_';

/*** Functions ***/
void AVL_tree::AVL_node::left(AVL_tree::AVL_node* left)
{
    /*Shallow copy because every node is responsible for itself. Had the node done allocation, it
    would have needed to free as well on destruction, making it responsible for its child nodes,
    which should be the responsability of the tree, not the node.*/
    this->m_left = left;
}

void AVL_tree::AVL_node::right(AVL_tree::AVL_node* right) { this->m_right = right; }

void AVL_tree::AVL_node::parent(AVL_tree::AVL_node* parent) { this->m_parent = parent; }

void AVL_tree::AVL_node::value(std::pair<uint16_t, uint16_t> const& value)
{
    delete this->m_value;

    this->m_value = new std::pair<uint16_t, uint16_t>(value);
}

AVL_tree::AVL_node* AVL_tree::AVL_node::left()
{
    return static_cast<AVL_tree::AVL_node*>(this->m_left);
}

AVL_tree::AVL_node* AVL_tree::AVL_node::right()
{
    return static_cast<AVL_tree::AVL_node*>(this->m_right);
}

AVL_tree::AVL_node* AVL_tree::AVL_node::parent()
{
    return static_cast<AVL_tree::AVL_node*>(this->m_parent);
}

std::pair<uint16_t, uint16_t> const& AVL_tree::AVL_node::value() const
{
    return *(static_cast<std::pair<uint16_t, uint16_t> const*>(this->m_value));
}

void AVL_tree::AVL_node::find(std::pair<uint16_t, uint16_t> const& value, bool find_parent,
    AVL_node*& node, AVL_node*& parent)
{
    AVL_node* curr_node = this;
    AVL_node* curr_parent = this->parent();

    while (curr_node != nullptr) {
        if (!find_parent && value == curr_node->value()) {
            break;
        }

        curr_parent = curr_node;
        if (value < curr_node->value()) {
            curr_node = curr_node->left();
        } else {
            curr_node = curr_node->right();
        }
    }
    node = curr_node;
    parent = curr_parent;
}

void AVL_tree::AVL_node::find_min(AVL_node*& node)
{
    auto curr_node = this;
    while (nullptr != curr_node->left()) {
        curr_node = curr_node->left();
    }

    node = curr_node;
}

bool AVL_tree::AVL_node::operator<(const AVL_node& other) const
{
    return this->value() < other.value();
}

void AVL_tree::add(std::pair<uint16_t, uint16_t> const& value)
{
    AVL_node* new_node = new AVL_node(value);
    AVL_node* node = nullptr;
    AVL_node* parent = nullptr;

    if (nullptr == this->m_head) {
        this->m_head = new_node;
        return;
    }

    this->m_head->find(value, true, node, parent);
    new_node->parent(parent);
    if (*new_node < *parent) {
        parent->left(new_node);
        return;
    }
    parent->right(new_node);
}

AVL_tree::avl_status AVL_tree::remove(std::pair<uint16_t, uint16_t> const& value)
{
    AVL_node* to_remove = nullptr;
    AVL_node* parent = nullptr;
    AVL_node* replacement = nullptr;
    AVL_node* node = nullptr;

    this->m_head->find(value, false, to_remove, parent);
    if (nullptr == to_remove) {
        return AVL_tree::avl_status::VALUE_NOT_FOUND;
    }

    /* to_remove has 2 children - swap its value with its minimal bigger child's value and remove
    the child. */
    if ((nullptr != to_remove->right()) && (nullptr != to_remove->left())) {
        to_remove->right()->find_min(replacement);

        auto tmp_value = to_remove->value();
        to_remove->value(replacement->value());
        replacement->value(tmp_value);

        // auto tmp_node = to_remove->parent();
        // to_remove->parent(replacement->parent());
        // replacement->parent(tmp_node);

        /* replacement, being the minimum of its tree, has no left child, so just remove it with
        a case of 1 or 0 children. */
        to_remove = replacement;
        replacement = nullptr;
    }

    // to_remove has 1 child.
    if ((nullptr == to_remove->right()) ^ (nullptr == to_remove->left())) {
        replacement = (nullptr == to_remove->left() ? to_remove->right() : to_remove->left());
        replacement->parent(to_remove->parent());
        // to_remove has 0 children.
    } else {
        replacement = nullptr;
    }

    // Only the tree's root has no parent.
    if (nullptr == to_remove->parent()) {
        this->m_head = replacement;
    } else {
        if (to_remove->parent()->left() == to_remove) {
            to_remove->parent()->left(replacement);
        } else {
            to_remove->parent()->right(replacement);
        }
    }

    delete to_remove;
    return AVL_tree::avl_status::SUCCESS;
}

bool AVL_tree::is_inside(std::pair<uint16_t, uint16_t> const& value) const
{
    AVL_node* parent;
    AVL_node* node;

    this->m_head->find(value, false, node, parent);

    return nullptr != node;
}

size_t AVL_tree::AVL_node::get_height(AVL_tree::AVL_node const* node)
{
    if (nullptr == node) {
        return 0;
    }

    return 1 + std::max(get_height(node->m_left), get_height(node->m_right));
}

AVL_tree::AVL_node::node_status AVL_tree::AVL_node::print_nth_depth(
    size_t depth, size_t height, bool is_empty)
{
    node_status status = node_status::UNINITIALIZED;
    char value_char = '\0';
    size_t power = 0;

    if (0 == height) {
        return AVL_tree::AVL_node::node_status::INVALID_HEIGHT;
    }

    if (depth == 0) {
        if (is_empty) {
            for (size_t i = 1; i < std::pow(2, height); ++i) {
                std::cout << FILLER_CHAR;
            }
            return node_status::SUCCESS;
        }

        char value_char = static_cast<char>(this->value().first);
        if (height == 1) {
            std::cout << value_char;
            return AVL_tree::AVL_node::node_status::SUCCESS;
        }

        power = height - 2;
        for (size_t i = 0; i < std::pow(2, power); ++i) {
            std::cout << FILLER_CHAR;
        }
        for (size_t i = 0; i < std::pow(2, power) - 1; ++i) {
            if (nullptr == this->left()) {
                std::cout << FILLER_CHAR;
            } else {
                std::cout << BRANCH_CHAR;
            }
        }

        std::cout << value_char;

        for (size_t i = 0; i < std::pow(2, power) - 1; ++i) {
            if (nullptr == this->right()) {
                std::cout << FILLER_CHAR;
            } else {
                std::cout << BRANCH_CHAR;
            }
        }
        for (size_t i = 0; i < std::pow(2, power); ++i) {
            std::cout << FILLER_CHAR;
        }
    } else {
        if (is_empty || nullptr == this->left()) {
            status = this->print_nth_depth(depth - 1, height - 1,
                true);
            if (node_status::SUCCESS != status) {
                return status;
            }
        } else {
            status = this->left()->print_nth_depth(depth - 1, height - 1);
            if (node_status::SUCCESS != status) {
                return status;
            }
        }
        std::cout << FILLER_CHAR;
        if (is_empty || nullptr == this->right()) {
            status = this->print_nth_depth(depth - 1, height - 1, true);
            if (node_status::SUCCESS != status) {
                return status;
            }
        } else {
            status = this->right()->print_nth_depth(depth - 1, height - 1);
            if (node_status::SUCCESS != status) {
                return status;
            }
        }
    }

    return AVL_tree::AVL_node::node_status::SUCCESS;
}

void AVL_tree::print_tree()
{
    size_t height = AVL_node::get_height(this->m_head);

    for (size_t i = 0; i < height; ++i) {
        this->m_head->print_nth_depth(i, height);
        std::cout << "\n";
    }
}