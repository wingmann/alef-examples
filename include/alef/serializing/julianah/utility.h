#ifndef ALEF_SERIALIZING_JULIANAH_UTILITY_H
#define ALEF_SERIALIZING_JULIANAH_UTILITY_H

#include "./julianah.h"

ALEF_GLOBAL_NAMESPACE_BEGIN

namespace ser {

static void consume_ws(const julianah::string_type& str, julianah::size_type& offset) {
    while (std::isspace(str[offset])) {
        ++offset;
    }
}

static auto json_make(julianah::class_type type) {
    return julianah{type};
}

static auto json_load(const julianah::string_type& value) {
    julianah::size_type offset{};
    return parse_next(value, offset);
}

} // namespace ser

ALEF_GLOBAL_NAMESPACE_END

#endif // ALEF_SERIALIZING_JULIANAH_UTILITY_H
