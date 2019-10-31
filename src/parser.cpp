#include "myhtmlpp/parser.hpp"

#include "myhtmlpp/tree.hpp"

#include <cstddef>
#include <mycore/myosi.h>
#include <myhtml/api.h>
#include <stdexcept>
#include <string>

template <typename ParseFunc, typename... ParseArgs>
myhtmlpp::Tree parse_helper(ParseFunc f, const std::string& html,
                            myhtml_options opt, size_t thread_count,
                            size_t queue_size, ParseArgs... args) {
    myhtml_t* raw_myhtml = myhtml_create();
    mystatus_t init_st = myhtml_init(raw_myhtml, opt, thread_count, queue_size);
    if (init_st != MyHTML_STATUS_OK) {
        throw std::runtime_error("myhtml_init failed with status " +
                                 std::to_string(init_st));
    }

    myhtml_tree_t* raw_tree = myhtml_tree_create();
    mystatus_t tree_st = myhtml_tree_init(raw_tree, raw_myhtml);
    if (tree_st != MyHTML_STATUS_OK) {
        throw std::runtime_error("myhtml_tree_init failed with status " +
                                 std::to_string(tree_st));
    }

    mystatus_t parse_st = f(raw_tree, MyENCODING_UTF_8, html.c_str(),
                            strlen(html.c_str()), args...);
    if (parse_st != MyHTML_STATUS_OK) {
        throw std::runtime_error("parsing failed with status " +
                                 std::to_string(parse_st));
    }

    return myhtmlpp::Tree(raw_myhtml, raw_tree);
}

myhtmlpp::Tree myhtmlpp::parse(const std::string& html) {
    return parse_helper(myhtml_parse, html, MyHTML_OPTIONS_DEFAULT, 1, 0);
}

myhtmlpp::Tree myhtmlpp::parse_with_options(const std::string& html,
                                            myhtml_options opt,
                                            size_t thread_count,
                                            size_t queue_size) {
    return parse_helper(myhtml_parse, html, opt, thread_count, queue_size);
}

myhtmlpp::Tree myhtmlpp::parse_fragment(const std::string& html,
                                        myhtml_tag_id_t tag_id,
                                        myhtml_namespace ns) {
    return parse_helper(myhtml_parse_fragment, html, MyHTML_OPTIONS_DEFAULT, 1,
                        0, tag_id, ns);
}

myhtmlpp::Tree myhtmlpp::parse_fragment_with_options(
    const std::string& html, myhtml_tag_id_t tag_id, myhtml_namespace ns,
    myhtml_options opt, size_t thread_count, size_t queue_size) {
    return parse_helper(myhtml_parse_fragment, html, opt, thread_count,
                        queue_size, tag_id, ns);
}
