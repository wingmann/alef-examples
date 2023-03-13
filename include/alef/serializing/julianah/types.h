#ifndef ALEF_SERIALIZING_JULIANAH_TYPES_H
#define ALEF_SERIALIZING_JULIANAH_TYPES_H

#include "./const_wrapper.h"
#include "./wrapper.h"

#include "alef/string.h"

#include <deque>
#include <map>
#include <memory>
#include <variant>

ALEF_GLOBAL_NAMESPACE_BEGIN

namespace ser {

class julianah;

enum class json_class : uint8 {
    null,
    object,
    array,
    string,
    floating,
    integer,
    boolean
};

using json_floating = double;
using json_integer  = int64;
using json_boolean  = bool;
using json_null     = std::nullptr_t;
using json_string   = std::string;
using json_list     = std::deque<julianah>;
using json_map      = std::map<json_string, julianah>;
using json_list_ptr = std::shared_ptr<json_list>;
using json_map_ptr  = std::shared_ptr<json_map>;

using json_value = std::variant<
    json_list_ptr,
    json_map_ptr,
    json_string,
    json_floating,
    json_integer,
    json_boolean,
    json_null>;

using json_wrapped_list       = wrapper<json_list>;
using json_wrapped_const_list = const_wrapper<json_list>;
using json_wrapped_map        = wrapper<json_map>;
using json_wrapped_const_map  = const_wrapper<json_map>;

} // namespace ser

ALEF_GLOBAL_NAMESPACE_END

#endif // ALEF_SERIALIZING_JULIANAH_TYPES_H
