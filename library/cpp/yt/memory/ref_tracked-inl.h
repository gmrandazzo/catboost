#ifndef REF_TRACKED_INL_H_
#error "Direct inclusion of this file is not allowed, include ref_tracked.h"
// For the sake of sane code completion.
#include "ref_tracked.h"
#endif

namespace NYT {

////////////////////////////////////////////////////////////////////////////////

template <class T>
TRefCountedTypeKey GetRefCountedTypeKey()
{
    return &typeid(T);
}

template <class T>
Y_FORCE_INLINE TRefCountedTypeCookie GetRefCountedTypeCookie()
{
    static std::atomic<TRefCountedTypeCookie> cookie{NullRefCountedTypeCookie};
    auto cookieValue = cookie.load(std::memory_order::relaxed);
    if (Y_UNLIKELY(cookieValue == NullRefCountedTypeCookie)) {
        cookieValue = TRefCountedTrackerFacade::GetCookie(
            GetRefCountedTypeKey<T>(),
            sizeof(T),
            NYT::TSourceLocation());
        cookie.store(cookieValue, std::memory_order::relaxed);
    }
    return cookieValue;
}

template <class T, class TTag, int Counter>
Y_FORCE_INLINE TRefCountedTypeCookie GetRefCountedTypeCookieWithLocation(const TSourceLocation& location)
{
    static std::atomic<TRefCountedTypeCookie> cookie{NullRefCountedTypeCookie};
    auto cookieValue = cookie.load(std::memory_order::relaxed);
    if (Y_UNLIKELY(cookieValue == NullRefCountedTypeCookie)) {
        cookieValue = TRefCountedTrackerFacade::GetCookie(
            GetRefCountedTypeKey<T>(),
            sizeof(T),
            location);
        cookie.store(cookieValue, std::memory_order::relaxed);
    }
    return cookieValue;
}

////////////////////////////////////////////////////////////////////////////////

} // namespace NYT
