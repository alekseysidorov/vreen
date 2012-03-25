#ifndef NEWSMODEL_H
#define NEWSMODEL_H

#include <QAbstractListModel>

namespace vk {

class Client;
class NewsModelPrivate;
class NewsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(NewsModel)

    Q_ENUMS(ItemType)
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
public slots:
    void update();

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex &parent) const;
protected:
    QScopedPointer<NewsModelPrivate> d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_update_ready(const QVariant &response))
};

} //namespace vk


#endif // NEWSMODEL_H
