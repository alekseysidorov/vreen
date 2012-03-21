#ifndef VK_DIRECTCONNECTION_H
#define VK_DIRECTCONNECTION_H

#include <QNetworkAccessManager>
#include "connection.h"

namespace vk {

class DirectConnection : public QNetworkAccessManager, public Connection
{
    Q_OBJECT
public:
    explicit DirectConnection(QObject *parent = 0);

    virtual void connectToHost(const QString &login, const QString &password);
    virtual void disconnectFromHost();
    virtual Reply *request(const QString &method, const QVariantMap &args);
    virtual Client::State connectionState() const;
signals:
    void connectionStateChanged(vk::Client::State connectionState);
    void error(vk::Client::Error error);
protected:
    void setConnectionState(Client::State connectionState);
    void getToken(const QString &login, const QString &password);
protected slots:
    void getTokenFinished();
private:
    Client::State m_connectionState;
};

} // namespace vk

#endif // VK_DIRECTCONNECTION_H
