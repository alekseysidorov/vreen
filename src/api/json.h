#ifndef VK_JSON_H
#define VK_JSON_H
#include "vk_global.h"
#include <QVariantMap>

namespace vk {

namespace JSON {
    VK_SHARED_EXPORT QVariant parse(const QByteArray &data);
    VK_SHARED_EXPORT QByteArray generate(const QVariant &data, int indent = 0);
} //namespace JSON

} // namespace vk

#endif // VK_JSON_H
