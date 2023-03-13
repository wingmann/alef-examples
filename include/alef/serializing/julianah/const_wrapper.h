#ifndef ALEF_SERIALIZING_JSON_CONST_WRAPPER_H
#define ALEF_SERIALIZING_JSON_CONST_WRAPPER_H

#include "alef/alef.h"

#include <memory>

ALEF_GLOBAL_NAMESPACE_BEGIN

namespace ser {

template<typename Container>
class const_wrapper {
    const Container* object_;

public:
    explicit const_wrapper(const Container* value = nullptr) : object_{value} { }

public:
    [[nodiscard]]
    typename Container::const_iterator begin() const {
        return object_ ? object_->begin() : typename Container::const_iterator{};
    }

    [[nodiscard]]
    typename Container::const_iterator end() const {
        return object_ ? object_->end() : typename Container::const_iterator{};
    }
};

} // namespace ser

ALEF_GLOBAL_NAMESPACE_END

#endif // ALEF_SERIALIZING_JSON_CONST_WRAPPER_H
