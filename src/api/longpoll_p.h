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
        mode(LongPoll::ReceiveAttachments), pollInterval(1000), waitInterval(25) {}
    LongPoll *q_ptr;
    Client *client;

    LongPoll::Mode mode;
    int pollInterval;
    int waitInterval;
    QUrl dataUrl;

    void _q_request_server_finished(const QVariant &response)
    {
        Q_Q(LongPoll);
        auto reply = static_cast<Reply*>(q->sender());

        QVariantMap data = response.toMap();
        if (data.isEmpty()) {
            QTimer::singleShot(pollInterval, q, SLOT(requestServer()));
            return;
        }

        QString url("http://%1?act=a_check&key=%2&wait=%3&mode=%4");
        dataUrl = url.arg(data.value("server").toString(),
                          data.value("key").toString(),
                          QString::number(waitInterval),
                          QString::number(mode));

        if (client->isOnline())
            q->requestData(data.value("ts").toByteArray());
        reply->deleteLater();
    }

    void _q_on_data_received(const QVariant &response)
    {
        Q_Q(LongPoll);
        auto reply = static_cast<QNetworkReply*>(q->sender());
        auto data = response.toMap();

        if (client->isOnline())
            q->requestData(data.value("ts").toByteArray());

        reply->deleteLater();
    }
};


} //namespace vk

#endif // LONGPOLL_P_H
