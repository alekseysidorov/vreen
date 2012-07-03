#ifndef WALLMODEL_H
#define WALLMODEL_H

#include <QAbstractListModel>
#include <contact.h>
#include <wallsession.h>
#include <QWeakPointer>

class WallModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(vk::Contact* contact READ contact WRITE setContact NOTIFY contactChanged)
public:
    enum Roles {
        IdRole = Qt::UserRole,
        FromRole,
        ToRole,
        DateRole,
        BodyRole,
        CommentsRole,
        LikesRole,
        RepostsRole,
        AttachmentsRole
    };

    explicit WallModel(QObject *parent = 0);
    vk::Contact* contact() const;
    void setContact(vk::Contact* arg);
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex &) const;
    int count() const;
public slots:
    void getLastPosts(int count = 25, vk::WallSession::Filter filter = vk::WallSession::All);
    void addLike(int postId, bool retweet = false, const QString &message = QString());
    void deleteLike(int postId);
    void clear();
    int findPost(int id);
signals:
    void contactChanged(vk::Contact* arg);
    void requestFinished();
private slots:
    void addPost(const vk::WallPost &post);
    void replacePost(int index, const vk::WallPost &post);
    void onPostLikeAdded(int id, int likes, int reposts, bool isRetweeted);
    void onPostLikeDeleted(int postId, int count);
private:
    vk::Roster *roster() const;
    QWeakPointer<vk::Contact> m_contact;
    QWeakPointer<vk::WallSession> m_session;
    vk::WallPostList m_posts;
};

#endif // WALLMODEL_H
