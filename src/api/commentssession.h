#ifndef VK_COMMENTSSESSION_H
#define VK_COMMENTSSESSION_H

#include <QObject>
#include <QVariantMap>
#include "vk_global.h"

namespace vk {

class Reply;
class Contact;
class CommentSessionPrivate;

class VK_SHARED_EXPORT CommentSession : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(CommentSession)
public:
    CommentSession(vk::Contact *contact);
    virtual ~CommentSession();
    void setPostId(int id);
    int postId() const;
public slots:
    Reply *getComments(int offset = 0, int count = 100);
signals:
    void commentAdded(const QVariantMap &item);
    void commentDeleted(int commentId);
private:
    QScopedPointer<CommentSessionPrivate> d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_comments_received(QVariant))
};

typedef QList<QVariantMap> CommentList;

} // namespace vk

#endif // VK_COMMENTSSESSION_H
