#include "myhtmlpp/node.hpp"

#include "myhtmlpp/attribute.hpp"
#include "myhtmlpp/tree.hpp"

#include <myencoding/myosi.h>
#include <myhtml/api.h>
#include <optional>
#include <string>
#include <vector>

myhtmlpp::Node::Node(myhtml_tree_node_t* raw_node) : m_raw_node(raw_node) {}

myhtmlpp::Node::~Node() { myhtml_node_free(m_raw_node); }

bool myhtmlpp::Node::operator==(const Node& other) const {
    return m_raw_node == other.m_raw_node;
}

bool myhtmlpp::Node::operator!=(const Node& other) const {
    return !operator==(other);
}

bool myhtmlpp::Node::good() const { return m_raw_node != nullptr; }

std::string myhtmlpp::Node::text() const {
    const char* raw_text = myhtml_node_text(m_raw_node, nullptr);

    return raw_text != nullptr ? raw_text : "";
}

myhtml_tag_id_t myhtmlpp::Node::tag_id() const {
    return myhtml_node_tag_id(m_raw_node);
}

myhtml_namespace_t myhtmlpp::Node::ns() const {
    return myhtml_node_namespace(m_raw_node);
}

void myhtmlpp::Node::set_ns(myhtml_namespace_t new_ns) {
    myhtml_node_namespace_set(m_raw_node, new_ns);
}

std::optional<myhtmlpp::Node> myhtmlpp::Node::first_child() const {
    return optional_node_helper(myhtml_node_child);
}

std::optional<myhtmlpp::Node> myhtmlpp::Node::last_child() const {
    return optional_node_helper(myhtml_node_last_child);
}

std::optional<myhtmlpp::Node> myhtmlpp::Node::previous() const {
    return optional_node_helper(myhtml_node_prev);
}

std::optional<myhtmlpp::Node> myhtmlpp::Node::next() const {
    return optional_node_helper(myhtml_node_next);
}

std::optional<myhtmlpp::Node> myhtmlpp::Node::parent() const {
    return optional_node_helper(myhtml_node_parent);
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

std::optional<myhtmlpp::Attribute> myhtmlpp::Node::first_attribute() const {
    return optional_attribute_helper(myhtml_node_attribute_first);
}

std::optional<myhtmlpp::Attribute> myhtmlpp::Node::last_attribute() const {
    return optional_attribute_helper(myhtml_node_attribute_last);
}

std::vector<myhtmlpp::Attribute> myhtmlpp::Node::attributes() const {
    std::vector<Attribute> res;

    myhtml_tree_attr_t* raw_attr = myhtml_node_attribute_first(m_raw_node);
    while (raw_attr != nullptr) {
        res.emplace_back(myhtml_node_tree(m_raw_node), raw_attr);
        raw_attr = myhtml_attribute_next(raw_attr);
    }

    return res;
}

myhtmlpp::Attribute myhtmlpp::Node::add_attribute(const std::string& key,
                                                  const std::string& value) {
    myhtml_tree_attr_t* raw_attr = myhtml_attribute_add(
        m_raw_node, key.c_str(), strlen(key.c_str()), value.c_str(),
        strlen(value.c_str()), MyENCODING_UTF_8);

    return Attribute(myhtml_node_tree(m_raw_node), raw_attr);
}
