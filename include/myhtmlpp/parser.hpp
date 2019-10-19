#pragma once

#include "collection.hpp"
#include "node.hpp"

#include <myhtml/api.h>
#include <string>

namespace myhtmlpp {

class Parser {
public:
    explicit Parser(const std::string& html);
    ~Parser();

    bool good() const;

    Node root() const;

    Collection nodes_by_tag_id(myhtml_tag_id_t tag_id) const;
    Collection nodes_by_name(const std::string& name) const;
    Collection nodes_by_attribute_key(const std::string& key) const;
    Collection nodes_by_attribute_value(const std::string& value,
                                        bool case_insensitive = false) const;
    Collection
    nodes_by_attribute_value_with_key(const std::string& key,
                                      const std::string& value,
                                      bool case_insensitive = false) const;

private:
    myhtml_t* m_raw_myhtml;
    myhtml_tree_t* m_raw_tree;
};

}  // namespace myhtmlpp
