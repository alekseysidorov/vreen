#include "reply_p.h"
#include <QNetworkReply>

namespace vk {

Reply::Reply(QNetworkReply *reply) :
    QObject(reply),
    d_ptr(new ReplyPrivate(this))
{
}

Reply::~Reply()
{
}

QNetworkReply *Reply::reply() const
{
    return d_func()->networkReply;
}

void Reply::setReply(QNetworkReply *reply)
{
    Q_D(Reply);
    if (d->networkReply)
        d->networkReply->disconnect(this); //TODO may be need a deleteLater?
    d->networkReply = reply;
    setParent(reply);
}

} // namespace vk
