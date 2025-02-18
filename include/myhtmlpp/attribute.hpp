#pragma once

#include "constants.hpp"

#include <myhtml/myhtml.h>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>

namespace myhtmlpp {

/// A HTML Attribute class.
class Attribute {
public:
    /**
     * @brief Attribute constructor.
     *
     * Initialises m_raw_attribute with the given raw_attribute.
     *
     * @param raw_attribute A pointer to a myhtml_tree_attr.
     */
    explicit Attribute(myhtml_tree_attr_t* raw_attribute);

    ~Attribute() = default;

    Attribute(const Attribute&) = default;
    Attribute& operator=(const Attribute&) = default;

    Attribute(Attribute&& other) noexcept;
    Attribute& operator=(Attribute&& other) noexcept;

    /**
     * @brief Check if two attributes are equal.
     *
     * @return Whether both attributes point to the same myhtml struct;
     *         i.e. m_raw_attribute == other.m_raw_attribute.
     */
    [[nodiscard]] bool operator==(const Attribute& other) const;

    /**
     * @brief Check if two attributes are not equal.
     *
     * @return Whether both attributes point to different myhtml structs;
     *         i.e. m_raw_attribute != other.m_raw_attribute.
     */
    [[nodiscard]] bool operator!=(const Attribute& other) const;

    /**
     * @brief Check if myhtml pointer is not nullptr.
     *
     * @return Whether m_raw_attribute is != nullptr.
     */
    [[nodiscard]] bool good() const;

    /**
     * @brief Get the key of the attribute.
     *
     * @return A string of the attribute key.
     */
    [[nodiscard]] std::string key() const;

    /**
     * @brief Get the value of the attribute.
     *
     * @return A string of the attribute value.
     */
    [[nodiscard]] std::string value() const;

    /**
     * @brief Get the namespace of the attribute.
     *
     * @return An enum member of myhtml_namespace_t with the attribute
     *         namespace.
     */
    [[nodiscard]] NAMESPACE get_namespace() const;

    /**
     * @brief Get the previous attribute in the node.
     *
     * @return An optional with the previous attribute if it exists,
     *         std::nullopt otherwise.
     */
    [[nodiscard]] std::optional<Attribute> previous() const;

    /**
     * @brief Get the next attribute in the node.
     *
     * @return An optional with the next attribute if it exists,
     *         std::nullopt otherwise.
     */
    [[nodiscard]] std::optional<Attribute> next() const;

    // structured bindings support
    // auto [key, value] = attr;
    template <std::size_t N>
    decltype(auto) get() const {
        if constexpr (N == 0) {
            return key();
        } else if constexpr (N == 1) {
            return value();
        }
    }

private:
    /// Pointer to the underlying myhtml attribute struct.
    myhtml_tree_attr_t* m_raw_attribute;
};

/**
 * @brief Print the attribute.
 */
std::ostream& operator<<(std::ostream& os, const Attribute& attr);

}  // namespace myhtmlpp

// required for structured bindings
namespace std {
template <>
struct tuple_size<myhtmlpp::Attribute>
    : std::integral_constant<std::size_t, 2> {};

template <std::size_t N>
struct tuple_element<N, myhtmlpp::Attribute> {
    using type = decltype(std::declval<myhtmlpp::Attribute>().get<N>());
};

}  // namespace std
