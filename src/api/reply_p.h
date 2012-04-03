#ifndef REPLY_P_H
#define REPLY_P_H
#include "reply.h"
#include "json.h"
#include "reply.h"
#include "QNetworkReply"
#include <QWeakPointer>

namespace vk {

class ReplyPrivate
{
    Q_DECLARE_PUBLIC(Reply)
public:
    ReplyPrivate(Reply *q) : q_ptr(q) {}
    Reply *q_ptr;

    QWeakPointer<QNetworkReply> networkReply;
    QVariant response;
    QVariant error;

    void _q_reply_finished()
    {
        Q_Q(Reply);
        auto reply = static_cast<QNetworkReply*>(q->sender());
        QVariantMap map = JSON::parse(reply->readAll()).toMap();
        //TODO error and captcha handler

        response = map.value("response");
        if (!response.isNull()) {
            emit q->resultReady(response);
            return;
        } else {
            error = map.value("error");
            int errorCode = error.toMap().value("error_code").toInt();
            if (errorCode) {
                emit q->error(errorCode);
                return;
            }
        }
        if (!map.isEmpty()) {
            response = map;
            emit q->resultReady(response);
        }

    }
};


} //namespace vk

#endif // REPLY_P_H
