#include "longpoll_p.h"
#include "connection.h"
#include "message.h"
#include "roster.h"
#include "buddy.h"
#include <QDateTime>

namespace vk {

/*!
 * \brief The LongPoll class
 * Api reference: \link http://vk.com/developers.php?oid=-1&p=messages.getLongPollServer
 */

/*!
 * \brief LongPoll::LongPoll
 * \param client
 */
LongPoll::LongPoll(Client *client) :
    QObject(client),
    d_ptr(new LongPollPrivate(this))
{
    Q_D(LongPoll);
    d->client = client;
    setRunning(client->isOnline());

    connect(client, SIGNAL(onlineStateChanged(bool)), SLOT(setRunning(bool)));
}

LongPoll::~LongPoll()
{

}

void LongPoll::setMode(LongPoll::Mode mode)
{
    d_func()->mode = mode;
}

LongPoll::Mode LongPoll::mode() const
{
    return d_func()->mode;
}

void LongPoll::setRunning(bool set)
{
    if (set)
        requestServer();
}

void LongPoll::requestServer()
{
    Q_D(LongPoll);
    auto reply = d->client->request("messages.getLongPollServer");
    connect(reply, SIGNAL(resultReady(QVariant)), SLOT(_q_request_server_finished(QVariant)));
}

void LongPoll::requestData(const QByteArray &timeStamp)
{
    Q_D(LongPoll);
    QUrl url = d->dataUrl;
    url.addQueryItem("ts", timeStamp);
    auto reply = d->client->request(url);
    connect(reply, SIGNAL(resultReady(QVariant)), this, SLOT(_q_on_data_received(QVariant)));
}

void LongPollPrivate::_q_request_server_finished(const QVariant &response)
{
    Q_Q(LongPoll);

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
}

void LongPollPrivate::_q_on_data_received(const QVariant &response)
{
    Q_Q(LongPoll);
    auto data = response.toMap();

    if (data.contains("failed")) {
        q->requestServer();
        return;
    }

    QVariantList updates = data.value("updates").toList();
    for (int i = 0; i < updates.size(); i++) {
        QVariantList update = updates.at(i).toList();
        int updateType = update.value(0, -1).toInt();
        switch (updateType) {
        case LongPoll::MessageDeleted: {
            emit q->messageDeleted(update.value(1).toInt());
            break;
        }
        case LongPoll::MessageAdded: {
            LongPoll::MessageFlags flags(update.value(2).toInt());
            Message message(client);
            message.setDirection(flags & LongPoll::FlagMessageOutbox ? Message::Out
                                                                     : Message::In);
            message.setReadState(flags & LongPoll::FlagMessageUnread ? Message::Unread
                                                                     : Message::Read);
            int id = update.value(3).toInt();
            message.setId(update.value(1).toInt());
            message.setTitle(update.value(5).toString());
            message.setBody(update.value(6).toString());
            message.setFrom(client->roster()->contact(id));
            message.setDate(QDateTime::currentDateTime());
            emit q->messageAdded(message);
            break;
        }
        case LongPoll::UserOnline:
        case LongPoll::UserOffline: {
            // WTF? Why VKontakte sends minus as first char of id?
            auto id = qAbs(update.value(1).toInt());
            Buddy::Status status;
            if (updateType == LongPoll::UserOnline)
                status = Buddy::Online;
            else
                status = update.value(2).toInt() == 1 ? Buddy::Away
                                                      : Buddy::Offline;
            emit q->contactStatusChanged(id, status);
            break;
        }
        }
    }

    if (client->isOnline())
        q->requestData(data.value("ts").toByteArray());
}

} // namespace vk

#include "moc_longpoll.cpp"
