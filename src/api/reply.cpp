#include "reply_p.h"
#include <QNetworkReply>

namespace vk {

Reply::Reply(QNetworkReply *reply) :
    QObject(reply),
    d_ptr(new ReplyPrivate(this))
{
    setReply(reply);

    //qDebug() << "--Send reply:" << reply->url();
}

Reply::~Reply()
{
    //FIXME maybee it's never been used
    if (auto networkReply = d_func()->networkReply.data())
        networkReply->deleteLater();
}

QNetworkReply *Reply::networkReply() const
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
    if (!d->networkReply.isNull())
        d->networkReply.data()->deleteLater();
    d->networkReply = reply;
    setParent(reply);

    connect(reply, SIGNAL(finished()), SLOT(_q_reply_finished()));
}

} // namespace vk

#include "moc_reply.cpp"
