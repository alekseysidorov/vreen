#include "longpoll_p.h"
#include "connection.h"

namespace vk {

LongPoll::LongPoll(Client *client) :
    QObject(client),
    d_ptr(new LongPollPrivate(this))
{
    Q_D(LongPoll);
    d->client = client;
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

} // namespace vk
