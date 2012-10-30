#include "messagesession_p.h"
#include "client.h"
#include "utils.h"

namespace Vreen {

MessageSession::MessageSession(MessageSessionPrivate *data) :
    QObject(data->client),
    d_ptr(data)
{
}
MessageSession::~MessageSession()
{
}

Client *MessageSession::client() const
{
    return d_func()->client;
}

int MessageSession::uid() const
{
    return d_func()->uid;
}

Reply *MessageSession::getHistory(int count, int offset)
{
    return doGetHistory(count, offset);
}

SendMessageReply *MessageSession::sendMessage(const QString &body, const QString &subject)
{
    Q_D(MessageSession);
    Message msg(d->client);
    msg.setToId(d->uid);
    msg.setBody(body);
    msg.setSubject(subject);
    return sendMessage(msg);
}

SendMessageReply *MessageSession::sendMessage(const Message &message)
{
    return doSendMessage(message);
}

Reply *MessageSession::markMessagesAsRead(IdList ids, bool set)
{
    Q_D(MessageSession);
    QString request = set ? "messages.markAsRead"
                          : "messages.markAsNew";
    QVariantMap args;
    args.insert("mids", join(ids));
    auto reply = d->client->request(request, args);
    reply->setProperty("mids", qVariantFromValue(ids));
    reply->setProperty("set", set);
    return reply;
}

} // namespace Vreen

#include "moc_messagesession.cpp"
