#ifndef ALEF_SERIALIZING_JULIANAH_VALUES_H
#define ALEF_SERIALIZING_JULIANAH_VALUES_H

#include "alef/alef.h"

ALEF_GLOBAL_NAMESPACE_BEGIN

namespace ser {

static constexpr auto json_boolean_value = [](bool value) { return value ? "true" : "false"; };
static constexpr auto json_null_value  = []() { return "null"; };

} // namespace ser

ALEF_GLOBAL_NAMESPACE_END

#endif // ALEF_SERIALIZING_JULIANAH_VALUES_H
