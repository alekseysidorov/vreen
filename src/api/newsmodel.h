#ifndef NEWSMODEL_H
#define NEWSMODEL_H

#include <QAbstractListModel>
#include "vk_global.h"

namespace vk {

class Client;
class NewsModelPrivate;
class VK_SHARED_EXPORT NewsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(NewsModel)

    Q_ENUMS(ItemType)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    enum ItemType
    {
        Post,
        Photo,
        PhotoTag,
        Friend,
        Note //TODO maybe deprecated
    };

    enum Role
    {
        TypeRole = Qt::UserRole + 1,
        ItemRole,
        SourceRole,
        IsGroupRole
    };

    NewsModel(Client *client);
    virtual ~NewsModel();
    int count() const;    
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex &parent) const;
public slots:
    void update();
    void clear();
signals:
    void countChanged(int);
protected:
    QScopedPointer<NewsModelPrivate> d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_update_ready(const QVariant &response))
};

} //namespace vk


#endif // NEWSMODEL_H
