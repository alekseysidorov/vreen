#ifndef LONGPOLL_P_H
#define LONGPOLL_P_H

#include "longpoll.h"
#include "client.h"
#include "reply.h"

#include <QVariant>
#include <QTimer>
#include <QUrl>
#include <QNetworkReply>

#include <QDebug>

namespace vk {

class LongPoll;
class LongPollPrivate
{
    Q_DECLARE_PUBLIC(LongPoll)
public:
    LongPollPrivate(LongPoll *q) : q_ptr(q), client(0),
        mode(LongPoll::ReceiveAttachments), pollInterval(1000) {}
    LongPoll *q_ptr;
    Client *client;

    LongPoll::Mode mode;
    int pollInterval;
    QUrl dataUrl;

    void _q_request_server_finished(const QVariant &raw)
    {
        Q_Q(LongPoll);
        auto reply = static_cast<Reply*>(q->sender());

        QVariantMap data = raw.toMap();
        if (data.isEmpty()) {
            QTimer::singleShot(pollInterval, q, SLOT(requestServer()));
            return;
        }

        QString url("http://%1?act=a_check&key=%2&wait=25");
        dataUrl = url.arg(data.value("server").toString(), data.value("key").toString());

        if (client->isOnline())
            q->requestData(data.value("ts").toByteArray());
        reply->deleteLater();
    }

    void _q_on_data_received(const QVariant &response)
    {
        Q_Q(LongPoll);
        auto reply = static_cast<QNetworkReply*>(q->sender());

        reply->deleteLater();
        qDebug() << Q_FUNC_INFO << response;
    }
};


} //namespace vk

#endif // LONGPOLL_P_H
