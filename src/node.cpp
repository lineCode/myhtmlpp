#include "myhtmlpp/node.hpp"

#include "myhtmlpp/attribute.hpp"
#include "utils.hpp"

#include <cstring>
#include <mycore/myosi.h>
#include <mycore/mystring.h>
#include <myencoding/myosi.h>
#include <myhtml/api.h>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

myhtmlpp::Node::Node(myhtml_tree_node_t* raw_node) : m_raw_node(raw_node) {}

bool myhtmlpp::Node::operator==(const Node& other) const {
    return m_raw_node == other.m_raw_node;
}

bool myhtmlpp::Node::operator!=(const Node& other) const {
    return !operator==(other);
}

myhtmlpp::Attribute myhtmlpp::Node::operator[](const std::string& key) const
    noexcept {
    myhtml_tree_attr_t* attr =
        myhtml_attribute_by_key(m_raw_node, key.c_str(), strlen(key.c_str()));

    return Attribute(attr);
}

bool myhtmlpp::Node::good() const { return m_raw_node != nullptr; }

std::string myhtmlpp::Node::html() const {
    mycore_string_raw_t str = {nullptr, 0, 0};
    myhtml_serialization_node_buffer(m_raw_node, &str);

    return str.data != nullptr ? str.data : "";
}

std::string myhtmlpp::Node::text() const {
    const char* raw_text = myhtml_node_text(m_raw_node, nullptr);

    return raw_text != nullptr ? raw_text : "";
}

myhtml_tag_id_t myhtmlpp::Node::tag_id() const {
    return myhtml_node_tag_id(m_raw_node);
}

std::string myhtmlpp::Node::tag_string() const {
    const char* tag_name =
        myhtml_tag_name_by_id(myhtml_node_tree(m_raw_node), tag_id(), nullptr);

    return tag_name != nullptr ? tag_name : "";
}

myhtml_namespace_t myhtmlpp::Node::ns() const {
    return myhtml_node_namespace(m_raw_node);
}

bool myhtmlpp::Node::is_void_element() const {
    return myhtml_node_is_void_element(m_raw_node);
}

void myhtmlpp::Node::set_ns(myhtml_namespace_t new_ns) {
    myhtml_node_namespace_set(m_raw_node, new_ns);
}

std::optional<myhtmlpp::Node> myhtmlpp::Node::first_child() const {
    return optional_helper<Node>(myhtml_node_child, m_raw_node);
}

std::optional<myhtmlpp::Node> myhtmlpp::Node::last_child() const {
    return optional_helper<Node>(myhtml_node_last_child, m_raw_node);
}

std::optional<myhtmlpp::Node> myhtmlpp::Node::previous() const {
    return optional_helper<Node>(myhtml_node_prev, m_raw_node);
}

std::optional<myhtmlpp::Node> myhtmlpp::Node::next() const {
    return optional_helper<Node>(myhtml_node_next, m_raw_node);
}

std::optional<myhtmlpp::Node> myhtmlpp::Node::parent() const {
    return optional_helper<Node>(myhtml_node_parent, m_raw_node);
}

std::vector<myhtmlpp::Node> myhtmlpp::Node::children() const {
    std::vector<myhtmlpp::Node> res;

    myhtml_tree_node_t* raw_child = myhtml_node_child(m_raw_node);
    while (raw_child != nullptr) {
        res.emplace_back(raw_child);
        raw_child = myhtml_node_next(raw_child);
    }

    return res;
}

void myhtmlpp::Node::add_child(const Node& node) {
    myhtml_tree_node_add_child(m_raw_node, node.m_raw_node);
}

void myhtmlpp::Node::insert_before(const Node& node) {
    myhtml_tree_node_insert_before(m_raw_node, node.m_raw_node);
}

void myhtmlpp::Node::insert_after(const Node& node) {
    myhtml_tree_node_insert_after(m_raw_node, node.m_raw_node);
}

void myhtmlpp::Node::remove() { myhtml_node_remove(m_raw_node); }

myhtmlpp::Attribute myhtmlpp::Node::at(const std::string& key) const {
    myhtml_tree_attr_t* attr =
        myhtml_attribute_by_key(m_raw_node, key.c_str(), strlen(key.c_str()));

    if (attr == nullptr) {
        throw std::out_of_range("attribute with key " + key +
                                " does not exist.");
    }

    return Attribute(attr);
}

std::optional<myhtmlpp::Attribute> myhtmlpp::Node::first_attribute() const {
    return optional_helper<Attribute>(myhtml_node_attribute_first, m_raw_node);
}

std::optional<myhtmlpp::Attribute> myhtmlpp::Node::last_attribute() const {
    return optional_helper<Attribute>(myhtml_node_attribute_last, m_raw_node);
}

std::vector<myhtmlpp::Attribute> myhtmlpp::Node::attributes() const {
    return std::vector(begin(), end());
}

myhtmlpp::Attribute myhtmlpp::Node::add_attribute(const std::string& key,
                                                  const std::string& value) {
    myhtml_tree_attr_t* raw_attr = myhtml_attribute_add(
        m_raw_node, key.c_str(), strlen(key.c_str()), value.c_str(),
        strlen(value.c_str()), MyENCODING_UTF_8);

    return Attribute(raw_attr);
}

bool myhtmlpp::Node::remove_attribute_by_key(const std::string& key) {
    myhtml_tree_attr_t* attr = myhtml_attribute_remove_by_key(
        m_raw_node, key.c_str(), strlen(key.c_str()));

    return attr != nullptr;
}

// Iterator
myhtmlpp::Node::Iterator::Iterator(const Attribute& attr) : m_attr(attr) {}

myhtmlpp::Node::Iterator& myhtmlpp::Node::Iterator::operator++() {
    if (auto next = m_attr.next()) {
        m_attr = next.value();
    } else {
        m_attr = Attribute(nullptr);
    }

    return *this;
}

myhtmlpp::Node::Iterator myhtmlpp::Node::begin() noexcept {
    return Iterator(Attribute(myhtml_node_attribute_first(m_raw_node)));
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
myhtmlpp::Node::Iterator myhtmlpp::Node::end() noexcept {
    return Iterator(Attribute(nullptr));
}

// ConstIterator
myhtmlpp::Node::ConstIterator::ConstIterator(const Attribute& attr)
    : m_attr(attr) {}

myhtmlpp::Node::ConstIterator& myhtmlpp::Node::ConstIterator::operator++() {
    if (auto next = m_attr.next()) {
        m_attr = next.value();
    } else {
        m_attr = Attribute(nullptr);
    }

    return *this;
}

myhtmlpp::Node::ConstIterator myhtmlpp::Node::begin() const noexcept {
    return ConstIterator(Attribute(myhtml_node_attribute_first(m_raw_node)));
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
myhtmlpp::Node::ConstIterator myhtmlpp::Node::end() const noexcept {
    return ConstIterator(Attribute(nullptr));
}

myhtmlpp::Node::ConstIterator myhtmlpp::Node::cbegin() const noexcept {
    return begin();
}

myhtmlpp::Node::ConstIterator myhtmlpp::Node::cend() const noexcept {
    return end();
}

std::ostream& myhtmlpp::operator<<(std::ostream& os, const myhtmlpp::Node& n) {
    os << n.html();

    return os;
}
