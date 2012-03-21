#ifndef VK_CONNECTION_H
#define VK_CONNECTION_H

#include <QNetworkAccessManager>
#include <QVariant>
#include "client.h"

namespace vk {

//TODO таки найти способ красиво рассказать клиенту о том, что connection state поменялся
class Reply;

class Connection
{
public:
    Connection();
    virtual ~Connection() {}

    virtual void connectToHost(const QString &login, const QString &password) = 0;
    virtual void disconnectFromHost() = 0;
    virtual Reply *request(const QString &method, const QVariantMap &args) = 0;
    virtual Client::State connectionState() const = 0;
protected: //signals
    virtual void connectionStateChanged(vk::Client::State connectionState) = 0;
    virtual void error(vk::Client::Error error) = 0;
};

} // namespace vk

#endif // VK_CONNECTION_H
