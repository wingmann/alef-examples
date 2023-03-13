#ifndef ALEF_SERIALIZING_JSON_WRAPPER_H
#define ALEF_SERIALIZING_JSON_WRAPPER_H

#include "alef/alef.h"

ALEF_GLOBAL_NAMESPACE_BEGIN

namespace ser {

template<typename Container>
class wrapper {
    Container* object_;

public:
    explicit wrapper(Container* value = nullptr) : object_{value} { }

public:
    typename Container::iterator begin() {
        return object_ ? object_->begin() : typename Container::iterator{};
    }

    [[nodiscard]]
    typename Container::const_iterator begin() const {
        return object_ ? object_->begin() : typename Container::iterator{};
    }

    typename Container::iterator end() {
        return object_ ? object_->end() : typename Container::iterator{};
    }

    [[nodiscard]]
    typename Container::const_iterator end() const {
        return object_ ? object_->end() : typename Container::iterator{};
    }
};

} // namespace ser

ALEF_GLOBAL_NAMESPACE_END

#endif // ALEF_SERIALIZING_JSON_WRAPPER_H
