#ifndef NEWSFEEDMODEL_H
#define NEWSFEEDMODEL_H

#include <QAbstractListModel>
#include <newsfeed.h>
#include <QWeakPointer>

namespace vk {
class Contact;
} //namespace vk

class NewsFeedModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QObject* client READ client WRITE setClient NOTIFY clientChanged)
public:

    enum Roles {
        TypeRole = Qt::UserRole,
        PostIdRole,
        SourceRole,
        DateRole,
        BodyRole,
        AttachmentsRole,
        LikesRole,
        RepostsRole,
        CommentsRole,
        OwnerNameRole,
        SourcePhotoRole,
        SourceNameRole,
        LikesCount,
        CommentsCount
    };

    explicit NewsFeedModel(QObject *parent = 0);
    QObject* client() const;
    void setClient(QObject* arg);
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex &) const;
    int count() const;
    int findNews(int id);
public slots:
    void getNews(int filters = vk::NewsFeed::FilterPost | vk::NewsFeed::FilterPhoto,
                       quint8 count = 10, int offset = 0);
    void addLike(int postId, bool retweet = false, const QString &message = QString());
    void deleteLike(int postId);
    void clear();
    void truncate(int count);
signals:
    void clientChanged(QObject* client);
    void requestFinished();
protected:
    void insertNews(int index, const vk::NewsItem &data);
    void replaceNews(int index, const vk::NewsItem &data);
    vk::Contact *findContact(int id) const;
private slots:
    void onNewsAdded(const vk::NewsItem &data);
    void onAddLike(const QVariant &response);
    void onDeleteLike(const QVariant &response);
private:
    QWeakPointer<vk::Client> m_client;
    QWeakPointer<vk::NewsFeed> m_newsFeed;
    vk::NewsItemList m_newsList;
    Qt::SortOrder m_sortOrder;
};

#endif // NEWSFEEDMODEL_H
