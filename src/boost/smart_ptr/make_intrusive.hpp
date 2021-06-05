#pragma once

namespace boost {

template<typename T, typename... Args>
inline intrusive_ptr<T>
make_intrusive(Args&&... args)
{
    return boost::intrusive_ptr<T>(new T(std::forward<Args>(args)...));
}

} /* boost */
