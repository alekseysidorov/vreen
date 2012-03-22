#include "client_p.h"


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

void Client::setlogin(const QString &login)
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
    return connection()->connectionState() == Client::StateOnline;
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

void Client::connectToHost()
{
    Q_D(Client);
    //TODO add warnings
    connection()->connectToHost(d->login, d->password);
}

void Client::disconnectFromHost()
{
    connection()->disconnectFromHost();
}

} // namespace vk

#include "moc_client.cpp"
