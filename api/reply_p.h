#ifndef REPLY_P_H
#define REPLY_P_H
#include "reply.h"
#include "json.h"
#include "QNetworkReply"

namespace vk {

class ReplyPrivate
{
    Q_DECLARE_PUBLIC(Reply)
public:
    ReplyPrivate(Reply *q) : q_ptr(q), networkReply(0) {}
    Reply *q_ptr;

    QNetworkReply *networkReply;
    QVariant response;

    void _q_reply_finished()
    {
        Q_Q(Reply);
        auto reply = static_cast<QNetworkReply*>(q->sender());
        QVariantMap map = JSON::parse(reply->readAll()).toMap();
        //TODO error and captcha handler

        response = map.value("response");
        emit q->resultReady(response);
    }

    void _q_network_reply_destroyed(QObject*)
    {

    }
};


} //namespace vk

#endif // REPLY_P_H
