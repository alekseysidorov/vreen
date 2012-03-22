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
    return d_func()->networkReply.data();
}

QVariant Reply::response() const
{
    return d_func()->response;
}

QVariant Reply::error() const
{
    return d_func()->error;
}

void Reply::setReply(QNetworkReply *reply)
{
    Q_D(Reply);    
    d->networkReply.reset(reply);
    setParent(reply);

    connect(reply, SIGNAL(finished()), SLOT(_q_reply_finished()));
    connect(reply, SIGNAL(destroyed(QObject*)), SLOT(_q_network_reply_destroyed(QObject*)));
}

} // namespace vk

#include "moc_reply.cpp"
