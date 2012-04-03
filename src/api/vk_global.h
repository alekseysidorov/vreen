#ifndef API_GLOBAL_H
#define API_GLOBAL_H

#include <QObject>

#if defined(VK_LIBRARY)
#  define VK_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define VK_SHARED_EXPORT Q_DECL_IMPORT
#endif

template <typename T>
Q_INLINE_TEMPLATE T sender_cast(QObject *sender)
{
#ifndef QT_NO_DEBUG
    Q_ASSERT(qobject_cast<T>(sender));
#endif
    return static_cast<T>(sender);
}

template<typename T, int N>
Q_INLINE_TEMPLATE int strToEnum(const T &str, const char *(&strings)[N])
{
    for(int i=0; i < N; i++) {
        if(QLatin1String(strings[i]) == str)
            return i;
    }
    return -1;
}

template<typename X,typename T, int N>
Q_INLINE_TEMPLATE X strToEnum(const T &str, const char *(&strings)[N])
{
    return static_cast<X>(strToEnum(str,strings));
}

template<int N>
Q_INLINE_TEMPLATE QLatin1String enumToStr(int i, const char *(&strings)[N])
{
    return QLatin1String((i < 0 || i >= N) ? 0 : strings[i]);
}

#endif // API_GLOBAL_H
