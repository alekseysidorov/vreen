#include "messagesession_p.h"
#include "client.h"

namespace vk {

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

} // namespace vk

#include "moc_messagesession.cpp"
