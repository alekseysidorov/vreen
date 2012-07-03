#ifndef COMMENTSMODEL_H
#define COMMENTSMODEL_H

#include <QAbstractListModel>
#include <QWeakPointer>
#include <commentssession.h>

class CommentsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(vk::Contact* contact READ contact WRITE setContact NOTIFY contactChanged)
    Q_PROPERTY(int postId READ postId WRITE setPostId NOTIFY postChangedId)
public:
    explicit CommentsModel(QObject *parent = 0);

    enum Roles {
        IdRole = Qt::UserRole,
        FromRole,
        DateRole,
        BodyRole,
        LikesRole
    };

    vk::Contact* contact() const;
    void setContact(vk::Contact* contact);
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex &) const;
    int count() const;
    int findComment(int id) const;
    int postId() const;
    void setPostId(int arg);
signals:
    void contactChanged(vk::Contact*);
    void requestFinished();
    void postChangedId(int);

public slots:
    void clear();
    void getComments(int offset = 0, int count = 100);
private slots:
    void addComment(const QVariantMap &data);
    void deleteComment(int id);
private:
    QWeakPointer<vk::Contact> m_contact;
    QWeakPointer<vk::CommentSession> m_session;
    vk::CommentList m_comments;
    int m_postId;
};

#endif // COMMENTSMODEL_H
