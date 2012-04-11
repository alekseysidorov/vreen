#ifndef ABSTRACTLISTMODEL_H
#define ABSTRACTLISTMODEL_H

#include <QAbstractListModel>
#include "vk_global.h"

namespace vk {

class VK_SHARED_EXPORT AbstractListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit AbstractListModel(QObject *parent = 0);
    Q_INVOKABLE QVariantMap get(int row);
    Q_INVOKABLE QVariant get(int row, const QByteArray &field);
};

} //namespace vk

#endif // ABSTRACTLISTMODEL_H
