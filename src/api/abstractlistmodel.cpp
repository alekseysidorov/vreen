#include "abstractlistmodel.h"
#include <QDebug>

namespace vk {

AbstractListModel::AbstractListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

QVariantMap AbstractListModel::get(int row)
{
    auto roles = roleNames();
    QVariantMap map;
    auto index = createIndex(row, 0);
    for (auto it = roles.constBegin(); it != roles.constEnd(); it++) {
        auto value = data(index, it.key());
        map.insert(it.value(), value);
    }
    return map;
}

QVariant AbstractListModel::get(int row, const QByteArray &field)
{
    auto index = createIndex(row, 0);
    return data(index, roleNames().key(field));
}

} //namespace vk
