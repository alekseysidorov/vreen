#ifndef VK_CONNECTION_H
#define VK_CONNECTION_H

#include <QNetworkAccessManager>
#include <QVariantMap>
#include "client.h"

namespace vk {

class Reply;
class VK_SHARED_EXPORT Connection : public QNetworkAccessManager
{
    Q_OBJECT
public:
    Connection(QObject *parent = 0);

    virtual void connectToHost(const QString &login, const QString &password) = 0;
    virtual void disconnectFromHost() = 0;
    virtual QNetworkReply *request(const QString &method, const QVariantMap &args = QVariantMap()) = 0;
    virtual Client::State connectionState() const = 0;
    virtual int uid() const = 0;
signals:
    void connectionStateChanged(vk::Client::State connectionState);
    void error(vk::Client::Error error);
};

} //namespace vk

#endif // VK_CONNECTION_H
