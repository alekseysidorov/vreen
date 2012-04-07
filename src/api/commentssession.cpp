#include "commentssession.h"
#include "contact.h"
#include "client.h"
#include "reply.h"

namespace vk {

class CommentSession;
class CommentSessionPrivate
{
    Q_DECLARE_PUBLIC(CommentSession)
public:
    CommentSessionPrivate(CommentSession *q, Contact *contact) :
        q_ptr(q), contact(contact), postId(0),
        sort(Qt::AscendingOrder),
        needLikes(true),
        previewLenght(0)

    {}
    CommentSession *q_ptr;
    Contact *contact;
    int postId;
    Qt::SortOrder sort;
    bool needLikes;
    int previewLenght;

    void _q_comments_received(const QVariant &response)
    {
        auto list = response.toList();
        list.takeFirst();
        foreach (auto item, list)
            emit q_func()->commentAdded(item.toMap());
    }
};


/*!
 * \brief CommentsSession::CommentsSession
 * \param client
 */
CommentSession::CommentSession(Contact *contact) :
    QObject(contact),
    d_ptr(new CommentSessionPrivate(this, contact))
{
}

void CommentSession::setPostId(int postId)
{
    Q_D(CommentSession);
    d->postId = postId;
}

int CommentSession::postId() const
{
    return d_func()->postId;
}

CommentSession::~CommentSession()
{
}

Reply *CommentSession::getComments(int offset, int count)
{
    Q_D(CommentSession);
    QVariantMap args;
    args.insert("owner_id", d->contact->id());
    args.insert("post_id", d->postId);
    args.insert("offset", offset);
    args.insert("count", count);
    args.insert("need_likes", d->needLikes);
    args.insert("preview_lenght", d->previewLenght);
    args.insert("sort", d->sort == Qt::AscendingOrder ? "asc" : "desc");
    auto reply = d->contact->client()->request("wall.getComments", args);
    connect(reply, SIGNAL(resultReady(QVariant)), SLOT(_q_comments_received(QVariant)));
    return reply;
}

} // namespace vk

#include "moc_commentssession.cpp"
