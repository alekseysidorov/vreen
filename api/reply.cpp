#include "reply_p.h"
#include <QNetworkReply>

namespace vk {

Reply::Reply(QNetworkReply *reply) :
    QObject(reply),
    d_ptr(new ReplyPrivate(this))
{
    setReply(reply);
}

Reply::~Reply()
{
}

QNetworkReply *Reply::reply() const
{
    return d_func()->networkReply;
}

QVariant Reply::response() const
{
    return d_func()->response;
}

void Reply::setReply(QNetworkReply *reply)
{
    Q_D(Reply);
    if (d->networkReply)
        d->networkReply->disconnect(this); //TODO may be need a deleteLater?
    d->networkReply = reply;
    setParent(reply);

    connect(reply, SIGNAL(finished()), SLOT(_q_reply_finished()));
}

} // namespace vk

#include "moc_reply.cpp"
