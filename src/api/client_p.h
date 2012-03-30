#ifndef CLIENT_P_H
#define CLIENT_P_H

#include "client.h"
#include "reply.h"
#include <QWeakPointer>
#include "directconnection_p.h"
#include "roster.h"
#include "reply.h"
#include "message.h"

namespace vk {

class ClientPrivate
{
    Q_DECLARE_PUBLIC(Client)
public:
    ClientPrivate(Client *q) : q_ptr(q) {}
    Client *q_ptr;
    QString login;
    QString password;
    QWeakPointer<Connection> connection;
	QWeakPointer<Roster> roster;

    void _q_connection_state_changed(vk::Client::State state)
    {
        Q_Q(Client);
        if (state != Client::StateConnecting)
            emit q->onlineStateChanged(state == Client::StateOnline);
        emit q->connectionStateChanged(state);
    }

    void _q_error_received(vk::Client::Error)
    {

    }

    void _q_reply_finished(const QVariant &)
    {
        auto reply = sender_cast<Reply*>(q_func()->sender());
        reply->deleteLater();
    }
};

} //namespace vk

#endif // CLIENT_P_H
