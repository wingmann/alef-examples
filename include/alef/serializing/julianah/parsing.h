#ifndef ALEF_SERIALIZING_JULIANAH_PARSING_H
#define ALEF_SERIALIZING_JULIANAH_PARSING_H

#include "./utility.h"
#include "./values.h"

ALEF_GLOBAL_NAMESPACE_BEGIN

namespace ser {

static julianah parse_object(const julianah::string_type& str, julianah::size_type& offset) {
    julianah value{julianah::class_type::object};

    ++offset;
    consume_ws(str, offset);

    if (str[offset] == '}') {
        ++offset;
        return value;
    }
    while (true) {
        auto key = parse_next(str, offset);
        consume_ws(str, offset);

        if (str[offset] != ':') {
            throw std::invalid_argument{std::format("expected ':', found: '{}'", str[offset])};
        }
        consume_ws(str, ++offset);

        auto next = parse_next(str, offset);
        value[key.to_string()] = next;

        consume_ws(str, offset);

        if (str[offset] == ',') {
            ++offset;
            continue;
        } else if (str[offset] == '}') {
            ++offset;
            break;
        } else {
            throw std::invalid_argument{std::format("expected ',', found: '{}'", str[offset])};
        }
    }
    return value;
}

static julianah parse_array(const julianah::string_type& str, julianah::size_type& offset) {
    julianah value{julianah::class_type::array};
    julianah::size_type index{};

    ++offset;
    consume_ws(str, offset);

    if (str[offset] == ']') {
        ++offset;
        return value;
    }
    while (true) {
        value[index++] = parse_next(str, offset);
        consume_ws(str, offset);

        if (str[offset] == ',') {
            ++offset;
            continue;
        } else if (str[offset] == ']') {
            ++offset;
            break;
        } else {
            throw std::invalid_argument{
                std::format("expected ',' or ']', found: '{}'", str[offset])};
        }
    }
    return value;
}

static julianah parse_string(const julianah::string_type& str, julianah::size_type& offset) {
    julianah value;
    std::stringstream ss;

    for (auto c = str[++offset]; c != '\"'; c = str[++offset]) {
        if (c != '\\') {
            ss << c;
            continue;
        }
        switch (str[++offset]) {
        case '\"': ss << '\"'; break;
        case '\\': ss << '\\'; break;
        case '/':  ss << '/';  break;
        case 'b':  ss << '\b'; break;
        case 'f':  ss << '\f'; break;
        case 'n':  ss << '\n'; break;
        case 'r':  ss << '\r'; break;
        case 't':  ss << '\t'; break;
        case 'u':
        {
            ss << "\\u";

            for (julianah::size_type i = 1; i <= 4; ++i) {
                c = str[offset + i];

                auto correct =
                    (((c >= '0') && (c <= '9')) ||
                     ((c >= 'a') && (c <= 'f')) ||
                     ((c >= 'A') && (c <= 'F')));

                if (correct) {
                    ss << c;
                } else {
                    throw std::invalid_argument{
                        std::format("expected hex character in unicode escape, found: '{}'", c)};
                }
            }
            offset += 4;
            break;
        }
        default:
            ss << '\\';
            break;
        }
    }
    ++offset;
    value = ss.str();
    return value;
}

static julianah parse_number(const julianah::string_type& str, julianah::size_type& offset) {
    julianah value;
    julianah::boolean_type is_floating{};
    julianah::integer_type exponent{};

    std::stringstream ss;
    std::stringstream expression;

    char symbol;

    while (true) {
        symbol = str[offset++];

        if ((symbol == '-') || ((symbol >= '0') && (symbol <= '9'))) {
            ss << symbol;
        } else if (symbol == '.') {
            ss << symbol;
            is_floating = true;
        } else {
            break;
        }
    }
    if (symbol == 'E' || symbol == 'e') {
        symbol = str[offset++];

        if (symbol == '-') {
            ++offset;
            expression << '-';
        }
        while (true) {
            symbol = str[offset++];

            if (symbol >= '0' && symbol <= '9') {
                expression << symbol;
            } else if ((symbol != ' ') && (symbol != ',') && (symbol != ']') && (symbol != '}')) {
                throw std::invalid_argument{
                    std::format("expected a number for exponent, found: '{}'", symbol)};
            } else {
                break;
            }
        }
        exponent = std::stoll(expression.str());
    } else if ((symbol != ' ') && (symbol != ',') && (symbol != ']') && (symbol != '}')) {
        throw std::invalid_argument{std::format("unexpected character: {}", symbol)};
    }
    --offset;

    if (is_floating) {
        value = std::stod(ss.str()) * std::pow(10, exponent);
    } else if (!expression.str().empty()) {
        value = static_cast<julianah::floating_type>(std::stoll(ss.str())) * std::pow(10, exponent);
    } else {
        value = static_cast<julianah::floating_type>(std::stoll(ss.str()));
    }
    return value;
}

static julianah parse_bool(const julianah::string_type& str, julianah::size_type& offset) {
    julianah value;

    if (str.substr(offset, 4) == json_boolean_value(true)) {
        value = true;
    } else if (str.substr(offset, 5) == json_boolean_value(false)) {
        value = false;
    } else {
        throw std::invalid_argument{
            std::format("expected 'true' or 'false', found: '{}'", str.substr(offset, 5))};
    }
    offset += value.to_boolean() ? 4 : 5;
    return value;
}

static julianah parse_null(const julianah::string_type& str, julianah::size_type& offset) {
    julianah value;

    if (str.substr(offset, 4) != json_null_value()) {
        throw std::invalid_argument{
            std::format("expected 'null', found: '{}'", str.substr(offset, 4))};
    }
    offset += 4;
    return value;
}

static julianah parse_next(const julianah::string_type& str, julianah::size_type& offset) {
    consume_ws(str, offset);
    auto symbol = str[offset];

    switch (symbol) {
    case '[':  return parse_array(str, offset);
    case '{':  return parse_object(str, offset);
    case '\"': return parse_string(str, offset);
    case 't':
    case 'f':  return parse_bool(str, offset);
    case 'n':  return parse_null(str, offset);
    default:
        if (((symbol <= '9') && (symbol >= '0')) || (symbol == '-')) {
            return parse_number(str, offset);
        }
    }
    throw std::invalid_argument{std::format("unknown starting character: '{}'", symbol)};
}

} // namespace ser

ALEF_GLOBAL_NAMESPACE_END

#endif // ALEF_SERIALIZING_JULIANAH_PARSING_H
