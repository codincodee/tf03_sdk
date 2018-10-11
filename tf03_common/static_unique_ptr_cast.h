#ifndef STATIC_UNIQUE_PTR_CAST_H
#define STATIC_UNIQUE_PTR_CAST_H

#include <memory>

template<typename TO, typename FROM>
std::unique_ptr<TO> dynamic_unique_ptr_cast (std::unique_ptr<FROM>&& old){
    return std::unique_ptr<TO>{dynamic_cast<TO*>(old.release())};
}

#endif // STATIC_UNIQUE_PTR_CAST_H
