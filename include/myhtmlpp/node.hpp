#pragma once

#include "attribute.hpp"
#include "constants.hpp"

#include <iterator>
#include <myhtml/myhtml.h>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

namespace myhtmlpp {

/// A HTML Node class.
class Node {
public:
    /**
     * @brief Node constructor.
     *
     * Initialises m_raw_node with the given raw_node.
     *
     * @param raw_node A pointer to a myhtml_tree_node.
     */
    explicit Node(myhtml_tree_node_t* raw_node);

    ~Node() = default;

    Node(const Node&) = default;
    Node& operator=(const Node&) = default;

    Node(Node&& other) noexcept;
    Node& operator=(Node&& other) noexcept;

    /**
     * @brief Check if two nodes are equal.
     *
     * @return Whether both nodes point to the same myhtml struct;
     *         i.e. m_raw_node == other.m_raw_node.
     */
    [[nodiscard]] bool operator==(const Node& other) const;

    /**
     * @brief Check if two nodes are not equal.
     *
     * @return Whether both nodes point to different myhtml structs;
     *         i.e. m_raw_node != other.m_raw_node.
     */
    [[nodiscard]] bool operator!=(const Node& other) const;

    /**
     * Returns the value of the attribute with key `key`.
     *
     * @param key The key of the attribute to access.
     * @return Value of the Attribute with the key `key` if it exists,
     *         Attribute that gets initialised with nullptr otherwise.
     */
    [[nodiscard]] std::string operator[](const std::string& key) const noexcept;

    /**
     * @brief Check if myhtml pointer is not nullptr.
     *
     * @return Whether m_raw_node is != nullptr.
     */
    [[nodiscard]] bool good() const;

    /**
     * @brief Returns a HTML representation of the node.
     *
     * @return A string with the HTML code.
     */
    [[nodiscard]] std::string html() const;

    /**
     * @brief Returns a HTML representation of the tree
     *        starting at the node.
     *
     * @return A string with the HTML code.
     */
    [[nodiscard]] std::string html_deep() const;

    /**
     * @brief Returns a string of the text in the node.
     *
     * Only works for myhtmlpp::TAG::TEXT_, myhtmlpp::TAG::COMMENT_
     * and myhtmlpp::TAG::STYLE.
     *
     * @return A string with the text, an empty string if it doesn't have text.
     */
    [[nodiscard]] std::string text() const;

    /**
     * @brief Returns the joined text of children nodes.
     *
     * @return A string with the joined text of all children nodes with text.
     */
    [[nodiscard]] std::string inner_text() const;

    /**
     * @brief Returns the tag id of the node.
     *
     * @return An enum member of myhtmlpp::TAG with the tag id of the node.
     */
    [[nodiscard]] TAG tag_id() const;

    /**
     * @brief Returns a string of the tag of the node.
     *
     * @return A string corresponding to the tag of the node.
     *
     * @see Node::tag_id
     */
    [[nodiscard]] std::string tag_name() const;

    /**
     * @brief Returns the namespace of the node.
     *
     * @return An enum member of myhtmlpp::NAMESPACE
     *         with the namespace of the node.
     */
    [[nodiscard]] NAMESPACE get_namespace() const;

    /**
     * @brief Returns if the node is a void element or not.
     *
     * @return true if the node is a void element, false otherwise.
     *
     * @see http://w3c.github.io/html-reference/syntax.html#void-elements
     */
    [[nodiscard]] bool is_void_element() const;

    /**
     * @brief Returns if the node can have text.
     *
     * @return true if `tag_id` returns `myhtmlpp::TAG::TEXT_` or
     *         `myhtmlpp::TAG::COMMENT_` or `myhtmlpp::TAG::STYLE`,
     *         false otherwise.
     */
    [[nodiscard, deprecated]] bool is_text_node() const;

    /**
     * @brief Returns the first child in the node.
     *
     * @return An optional with the first child node if it exists,
     *         std::nullopt otherwise.
     */
    [[nodiscard]] std::optional<Node> first_child() const;

    /**
     * @brief Returns the last child in the node.
     *
     * @return An optional with the last child node if it exists,
     *         std::nullopt otherwise.
     */
    [[nodiscard]] std::optional<Node> last_child() const;

    /**
     * @brief Returns the previous sibling of the node.
     *
     * @return An optional with the previous sibling node if it exists,
     *         std::nullopt otherwise.
     */
    [[nodiscard]] std::optional<Node> previous() const;

    /**
     * @brief Returns the next sibling of the node.
     *
     * @return An optional with the next sibling node if it exists,
     *         std::nullopt otherwise.
     */
    [[nodiscard]] std::optional<Node> next() const;

    /**
     * @brief Returns the parent of the node.
     *
     * @return An optional with the parent node if it exists,
     *         std::nullopt otherwise.
     */
    [[nodiscard]] std::optional<Node> parent() const;

    /**
     * @brief Returns a vector of all children of the node.
     *
     * @return A vector of all children nodes.
     */
    [[nodiscard]] std::vector<Node> children() const;

    /**
     * @brief Returns a vector of all siblings of the node.
     *
     * @return A vector of all sibling nodes.
     */
    [[nodiscard]] std::vector<Node> siblings() const;

    /**
     * Returns an Attribute in the node with the key `key`.
     *
     * @param key the key of the attribute to access.
     * @return Optional with the Attribute with the key `key` if it exists,
     *         std::nullopt otherwise.
     */
    [[nodiscard]] std::optional<std::string> at(const std::string& key) const;

    /**
     * @brief Checks if the node has at least one attribute.
     *
     * @return true if the node has one or more attributes, false otherwise.
     */
    [[nodiscard]] bool has_attributes() const;

    /**
     * @brief Checks if the node has an attribute with the key `key`.
     *
     * @param key The key to check.
     * @return true if the node has an attribute with the key `key`,
     *         false otherwise.
     */
    [[nodiscard]] bool has_attribute(const std::string& key) const;

    /**
     * @brief Returns the first attribute in the node.
     *
     * @return An optional with the first attribute in the node if it exists,
     *         std::nullopt otherwise.
     */
    [[nodiscard]] std::optional<Attribute> first_attribute() const;

    /**
     * @brief Returns the last attribute in the node.
     *
     * @return An optional with the last attribute in the node if it exists,
     *         std::nullopt otherwise.
     */
    [[nodiscard]] std::optional<Attribute> last_attribute() const;

    /**
     * @brief Returns a vector of all attributes of the node.
     *
     * @return A vector of all attributes of the node.
     */
    [[nodiscard]] std::vector<Attribute> attributes() const;

    /// A Node Iterator class
    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Attribute;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        explicit Iterator(Attribute attr);

        reference operator*();

        Iterator& operator++();

        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;

    private:
        Attribute m_attr;
    };

    /// A Node ConstIterator class
    class ConstIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Attribute;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

        explicit ConstIterator(Attribute attr);

        reference operator*() const;

        ConstIterator& operator++();

        bool operator==(const ConstIterator& other) const;
        bool operator!=(const ConstIterator& other) const;

    private:
        Attribute m_attr;
    };

    /**
     * @brief Returns an iterator to the first attribute.
     *
     * @return Iterator to first attribute if it exists,
     *         Iterator to `Attribute(nullptr)` otherwise.
     */
    Iterator begin() noexcept;

    /**
     * @brief Returns an iterator to after the last attribute.
     *
     * @return Iterator to `Attribute(nullptr)`.
     */
    Iterator end() noexcept;

    /**
     * @brief Returns a const iterator to the first attribute.
     *
     * @return ConstIterator to first attribute if it exists,
     *         ConstIterator to `Attribute(nullptr)` otherwise.
     */
    [[nodiscard]] ConstIterator begin() const noexcept;

    /**
     * @brief Returns an const iterator to after the last attribute.
     *
     * @return ConstIterator to `Attribute(nullptr)`.
     */
    [[nodiscard]] ConstIterator end() const noexcept;

    /**
     * @brief Returns a const iterator to the first attribute.
     *
     * @return ConstIterator to first attribute if it exists,
     *         ConstIterator to `Attribute(nullptr)` otherwise.
     */
    [[nodiscard]] ConstIterator cbegin() const noexcept;

    /**
     * @brief Returns an const iterator to after the last attribute.
     *
     * @return ConstIterator to `Attribute(nullptr)`.
     */
    [[nodiscard]] ConstIterator cend() const noexcept;

private:
    /// Pointer to the underlying myhtml node struct.
    myhtml_tree_node_t* m_raw_node;
};

/**
 * @brief Print the node.
 */
std::ostream& operator<<(std::ostream& os, const Node& n);

}  // namespace myhtmlpp
