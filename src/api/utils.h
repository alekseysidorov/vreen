#ifndef UTILS_H
#define UTILS_H
#include "vk_global.h"
#include <QStringList>

#include <QDebug>

namespace vk {

typedef QList<int> IdList;

QString VK_SHARED_EXPORT join(IdList ids);
QString VK_SHARED_EXPORT toCamelCase(QString string);

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

template<int N>
Q_INLINE_TEMPLATE QStringList flagsToStrList(int i, const char *(&strings)[N])
{
    QStringList list;
    for (int pos = 0; pos < N; pos++) {
        int flag = 1 << pos;
        if ((flag) & i)
            list.append(strings[pos]);
    }
    return list;
}

template<typename Container, typename T, typename LessThan>
int lowerBound(Container container, const T &value, LessThan lessThan)
{
    auto it =  qLowerBound(container.begin(), container.end(), value, lessThan);
    int index = it - container.begin();
    return index;
}

} //namespace vk

#endif // UTILS_H
