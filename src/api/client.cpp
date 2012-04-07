#include "client_p.h"
#include "message.h"
#include "contact.h"

namespace vk {

Client::Client(QObject *parent) :
    QObject(parent),
    d_ptr(new ClientPrivate(this))
{
}

Client::Client(const QString &login, const QString &password, QObject *parent) :
    QObject(parent),
    d_ptr(new ClientPrivate(this))
{
    Q_D(Client);
    d->login = login;
    d->password = password;
}

Client::~Client()
{
}

QString Client::password() const
{
    return d_func()->password;
}

void Client::setPassword(const QString &password)
{
    d_func()->password = password;
    emit passwordChanged(password);
}

QString Client::login() const
{
    return d_func()->login;
}

void Client::setLogin(const QString &login)
{
    d_func()->login = login;
    emit loginChanged(login);
}

Client::State Client::connectionState() const
{
    Q_D(const Client);
    if (d->connection.isNull())
        return StateInvalid;
    return d->connection.data()->connectionState();
}

bool Client::isOnline() const
{
    if (auto c = connection())
        return c->connectionState() == Client::StateOnline;
    else
        return Client::StateInvalid;
}

Connection *Client::connection() const
{
    return d_func()->connection.data();
}

Connection *Client::connection()
{
    Q_D(Client);
    if (d->connection.isNull())
        setConnection(new DirectConnection(this));
    return d_func()->connection.data();
}

void Client::setConnection(Connection *connection)
{
    Q_D(Client);
    if (d->connection) {
        //TODO cleanup
    }

    d->connection = connection;
    connect(connection, SIGNAL(connectionStateChanged(vk::Client::State)),
            this, SLOT(_q_connection_state_changed(vk::Client::State)));
    connect(connection, SIGNAL(error(vk::Client::Error)),
            this, SIGNAL(error(vk::Client::Error))); //TODO error handler
}

Roster *Client::roster() const
{
    return d_func()->roster.data();
}

Roster *Client::roster()
{
    Q_D(Client);
    if (d->roster.isNull()) {
        d->roster = new Roster(this);
    }
    return d->roster.data();
}

LongPoll *Client::longPoll() const
{
        return d_func()->longPoll.data();
}

LongPoll *Client::longPoll()
{
    Q_D(Client);
    if (d->longPoll.isNull()) {
        d->longPoll = new LongPoll(this);
        connect(this, SIGNAL(onlineStateChanged(bool)),
                d->longPoll.data(), SLOT(setRunning(bool)));
        emit longPollChanged(d->longPoll.data());
    }
    return d->longPoll.data();
}

Reply *Client::request(const QUrl &url)
{
    QNetworkRequest request(url);
    auto networkReply = connection()->get(request);
    auto reply = new Reply(networkReply);
    connect(reply, SIGNAL(resultReady(const QVariant &)), SLOT(_q_reply_finished(const QVariant &)));
    return reply;
}

Reply *Client::request(const QString &method, const QVariantMap &args)
{
    auto reply = new Reply(connection()->request(method, args));
    connect(reply, SIGNAL(resultReady(const QVariant &)), SLOT(_q_reply_finished(const QVariant &)));
    return reply;
}

Contact *Client::me() const
{
    if (auto r = roster())
        return r->owner();
    return 0;
}

Reply *Client::sendMessage(const Message &message)
{
    //TODO add delayed send
    if (!isOnline())
        return 0;

    //checks
    Q_ASSERT(message.to());

    QVariantMap args;
    //TODO add chat messages support and contact check
    args.insert("uid", message.to()->id());
    args.insert("message", message.body());
    args.insert("title", message.subject());
    return request("messages.send", args);
}

Reply *Client::getLastDialogs(int count, int previewLength)
{
    QVariantMap args;
    args.insert("count", count);
    if (previewLength != -1)
        args.insert("preview_length", previewLength);
    return request("messages.getDialogs", args);
}

void Client::connectToHost()
{
    Q_D(Client);
    //TODO add warnings
    connection()->connectToHost(d->login, d->password);
}

void Client::connectToHost(const QString &login, const QString &password)
{
    setLogin(login);
    setPassword(password);
    connectToHost();
}

void Client::disconnectFromHost()
{
    connection()->disconnectFromHost();
}

void ClientPrivate::_q_connection_state_changed(Client::State state)
{
    Q_Q(Client);
    switch (state) {
    case Client::StateOffline:
        emit q->onlineStateChanged(false);
        break;
    case Client::StateOnline:
        emit q->onlineStateChanged(true);
        if (!roster.isNull()) {
            roster.data()->setUid(connection.data()->uid());
            emit q->meChanged(roster.data()->owner());
        }
        break;
    default:
        break;
    }
    emit q->connectionStateChanged(state);
}

} // namespace vk

#include "moc_client.cpp"
