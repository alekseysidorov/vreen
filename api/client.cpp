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
}

} // namespace vk
