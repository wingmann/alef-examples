#ifndef ALEF_SERIALIZING_JULIANAH_JULIANAH_H
#define ALEF_SERIALIZING_JULIANAH_JULIANAH_H

#include "./types.h"
#include "./values.h"

#include "alef/concepts.h"
#include "alef/io.h"
#include "alef/string.h"
#include "alef/utility.h"

ALEF_GLOBAL_NAMESPACE_BEGIN

namespace ser {

class julianah;

julianah parse_next(const json_string& str, usize& offset);

class julianah final {
public:
    using string_type             = json_string;
    using floating_type           = json_floating;
    using integer_type            = json_integer;
    using boolean_type            = json_boolean;
    using null_type               = json_null;
    using class_type              = json_class;
    using value_type              = json_value;
    using list_type               = json_list_ptr;
    using map_type                = json_map_ptr;
    using wrapped_list_type       = json_wrapped_list;
    using wrapped_const_list_type = json_wrapped_const_list;
    using wrapped_map_type        = json_wrapped_map;
    using wrapped_const_map_type  = json_wrapped_const_map;
    using size_type               = usize;

private:
    class_type type_{class_type::null};
    value_type internal_{nullptr};

public:
    julianah() = default;
    virtual ~julianah() = default;

    julianah(std::initializer_list<julianah> list) : julianah() {
        set_type(class_type::object);

        for (auto it = list.begin(), end = list.end(); it != end; ++it) {
            operator[](it->to_string()) = *std::next(it);
        }
    }

    julianah(const julianah& other) { *this = other; }

    julianah(julianah&& other) noexcept
        : type_{other.type_}, internal_{std::move(other.internal_)}
    {
        other.type_ = class_type::null;
        other.internal_ = nullptr;
    }

    /// @brief Constructs as null.
    explicit julianah(null_type) : julianah() { }

    /// @brief Constructs from boolean.
    explicit julianah(boolean auto value) : internal_{value}, type_{class_type::boolean} { }

    /// @brief Constructs from integer.
    explicit julianah(integer auto value)
        : internal_{static_cast<integer_type>(value)}, type_{class_type::integer} { }

    /// @brief Constructs from floating.
    explicit julianah(floating_point auto value)
        : internal_{static_cast<floating_type>(value)}, type_{class_type::floating} { }

    /// @brief Constructs from json class.
    explicit julianah(class_type value) : type_{value} {
        switch (value) {
        case class_type::object:
            this->internal_ = std::make_shared<json_map>();
            break;
        case class_type::array:
            this->internal_ = std::make_shared<json_list>();
            break;
        case class_type::string:
            this->internal_ = string_type{};
            break;
        case json_class::null:
            this->internal_ = nullptr;
            break;
        case json_class::floating:
            this->internal_ = floating_type{};
            break;
        case json_class::integer:
            this->internal_ = integer_type{};
            break;
        case json_class::boolean:
            this->internal_ = boolean_type{};
            break;
        }
    }

    /// @brief Constructs from string.
    explicit julianah(convertible_to<string_type> auto value)
        : internal_{string_type{value}}, type_{class_type::string} { }

public:
    // Operators -----------------------------------------------------------------------------------
    julianah& operator=(const julianah& other) & {
        switch (other.type_) {
        case class_type::object:
            this->internal_ = std::make_shared<json_map>(
                std::get<map_type>(other.internal_)->begin(),
                std::get<map_type>(other.internal_)->end());
            break;
        case class_type::array:
            this->internal_ = std::make_shared<json_list>(
                std::get<list_type>(other.internal_)->begin(),
                std::get<list_type>(other.internal_)->end());
            break;
        case class_type::string:
            this->internal_ = std::get<string_type>(other.internal_);
            break;
        case json_class::null:
            this->internal_ = std::get<null_type>(other.internal_);
            break;
        case json_class::floating:
            this->internal_ = std::get<floating_type>(other.internal_);
            break;
        case json_class::integer:
            this->internal_ = std::get<integer_type>(other.internal_);
            break;
        case json_class::boolean:
            this->internal_ = std::get<boolean_type>(other.internal_);
            break;
        }
        this->type_ = other.type_;
        return *this;
    }

    julianah& operator=(julianah&& other) & noexcept {
        this->internal_ = other.internal_;
        this->type_ = other.type_;

        other.internal_ = nullptr;
        other.type_ = class_type::null;

        return *this;
    }

    julianah& operator=(boolean auto value) & {
        set_type(class_type::boolean);
        internal_ = value;
        return *this;
    }

    julianah& operator=(integer auto value) & {
        set_type(class_type::integer);
        internal_ = value;
        return *this;
    }

    julianah& operator=(floating_point auto value) & {
        set_type(class_type::floating);
        internal_ = value;
        return *this;
    }

    julianah& operator=(convertible_to<string_type> auto value) & {
        set_type(class_type::string);
        internal_ = string_type{value};
        return *this;
    }

    julianah& operator[](const string_type& key) & {
        set_type(class_type::object);
        return std::get<map_type>(internal_)->operator[](key);
    }

    julianah& operator[](unsigned index) & {
        set_type(class_type::array);

        if (index >= std::get<list_type>(internal_)->size()) {
            std::get<list_type>(internal_)->resize(index + 1);
        }
        return std::get<list_type>(internal_)->operator[](index);
    }

    // Methods -------------------------------------------------------------------------------------
    void set_type(class_type type) {
        if (type_ == type) {
            return;
        }
        switch (type) {
        case class_type::null:
            internal_ = nullptr;
            break;
        case class_type::object:
            internal_ = std::make_shared<json_map>();
            break;
        case class_type::array:
            internal_ = std::make_shared<json_list>();
            break;
        case class_type::string:
            internal_ = string_type{};
            break;
        case class_type::floating:
            internal_ = floating_type{};
            break;
        case class_type::integer:
            internal_ = integer_type{};
            break;
        case class_type::boolean:
            internal_ = boolean_type{};
            break;
        }
        type_ = type;
    }

    template<typename T>
    void append(T arg) {
        set_type(class_type::array);
        std::get<list_type>(internal_)->emplace_back(arg);
    }

    template<typename T, typename... U>
    void append(T arg, U... args) {
        append(arg);
        append(args...);
    }

    [[nodiscard]]
    const julianah& at(const string_type& key) const {
        return std::get<map_type>(internal_)->at(key);
    }

    julianah& at(const string_type& key) {
        return operator[](key);
    }

    [[nodiscard]]
    const julianah& at(unsigned index) const {
        return std::get<list_type>(internal_)->at(index);
    }

    julianah& at(unsigned index) {
        return operator[](index);
    }

    [[nodiscard]]
    size_type length() const {
        return (type_ == class_type::array)
            ? std::get<list_type>(internal_)->size()
            : max_value<size_type>();
    }

    [[nodiscard]]
    bool has_key(const string_type& key) const {
        return (type_ == class_type::object) &&
               (std::get<map_type>(internal_)->find(key) != std::get<map_type>(internal_)->end());
    }

    [[nodiscard]]
    size_type size() const {
        switch (type_) {
        case class_type::object:
            return std::get<map_type>(internal_)->size();
        case class_type::array:
            return std::get<list_type>(internal_)->size();
        default:
            return max_value<size_type>();
        }
    }

    [[nodiscard]]
    class_type get_type() const {
        return type_;
    }

    // Functions for getting primitives from the json object.
    [[nodiscard]]
    boolean_type is_null() const {
        return (type_ == class_type::null);
    }

    [[nodiscard]]
    string_type to_string() const {
        return to_string(bool{});
    }

    string_type to_string(bool&& ok) const {
        return (ok = (type_ == class_type::string))
            ? json_escape(std::get<string_type>(internal_))
            : string_type{};
    }

    [[nodiscard]]
    floating_type to_floating() const {
        return to_floating({});
    }

    floating_type to_floating(bool&& ok) const {
        return (ok = (type_ == class_type::floating))
            ? std::get<floating_type>(internal_)
            : floating_type{};
    }

    [[nodiscard]]
    integer_type to_integer() const {
        return to_integer({});
    }

    integer_type to_integer(bool&& ok) const {
        return (ok = (type_ == class_type::integer))
            ? std::get<integer_type>(internal_)
            : integer_type{};
    }

    [[nodiscard]]
    boolean_type to_boolean() const {
        return to_boolean({});
    }

    boolean_type to_boolean(bool&& ok) const {
        return (ok = (type_ == class_type::boolean)) && std::get<boolean_type>(internal_);
    }

    [[nodiscard]]
    auto array_range() const {
        return (type_ == class_type::array)
            ? wrapped_const_list_type{std::get<list_type>(internal_).get()}
            : wrapped_const_list_type{nullptr};
    }

    auto array_range() {
        return (type_ == class_type::array)
            ? wrapped_list_type{std::get<list_type>(internal_).get()}
            : wrapped_list_type{nullptr};
    }

    [[nodiscard]]
    auto object_range() const {
        return (type_ == class_type::object)
            ? wrapped_const_map_type{std::get<map_type>(internal_).get()}
            : wrapped_const_map_type{nullptr};
    }

    auto object_range() {
        return (type_ == class_type::object)
            ? wrapped_map_type{std::get<map_type>(internal_).get()}
            : wrapped_map_type{nullptr};
    }

    [[nodiscard]]
    string_type dump(int depth = 1, const size_type tab = 4) const {
        string_type tab_value = algo::repeat({0x20}, tab);
        string_type padding;
        std::stringstream ss;

        for (size_type i = 0; i < depth; ++i, padding += tab_value) { }

        switch (type_) {
        case class_type::object:
        {
            ss << "{\n";
            auto skip{true};

            for (auto& item : *std::get<map_type>(internal_)) {
                if (!skip) {
                    ss << ",\n";
                }
                ss << padding << std::quoted(item.first) << ": "
                   << item.second.dump(depth + 1, tab);
                skip = false;
            }
            ss << '\n' << padding.erase(0, tab_value.length()) << '}';
            break;
        }
        case class_type::array:
        {
            ss << '[';
            auto skip{true};

            for (auto& item : *std::get<list_type>(internal_)) {
                if (!skip) {
                    ss << ", ";
                }
                ss << item.dump(depth + 1, tab);
                skip = false;
            }
            ss << ']';
            break;
        }
        case class_type::string:
            ss << std::quoted(json_escape(std::get<string_type>(internal_)));
            break;
        case class_type::floating:
            ss << std::to_string(std::get<floating_type>(internal_));
            break;
        case class_type::integer:
            ss << std::to_string(std::get<integer_type>(internal_));
            break;
        case class_type::boolean:
            ss << json_boolean_value(std::get<boolean_type>(internal_));
            break;
        default:
            ss << json_null_value();
            break;
        }
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os) const {
        os << dump();
        return os;
    }

    template<typename... T>
    static auto array(T... args) {
        auto value = julianah{class_type::array};
        value.append(args...);
        return value;
    }

    static auto array() {
        return julianah{class_type::array};
    }

    static auto object() {
        return julianah{class_type::object};
    }

    static string_type json_escape(const string_type& value) {
        std::stringstream ss;

        for (const char& i : value) {
            switch (i) {
            case '\"': ss << "\\\""; break;
            case '\\': ss << "\\\\"; break;
            case '\b': ss << "\\b";  break;
            case '\f': ss << "\\f";  break;
            case '\n': ss << "\\n";  break;
            case '\r': ss << "\\r";  break;
            case '\t': ss << "\\t";  break;
            default:   ss << i;      break;
            }
        }
        return ss.str();
    }
};

} // namespace ser

ALEF_GLOBAL_NAMESPACE_END

#endif // ALEF_SERIALIZING_JULIANAH_JULIANAH_H
