#ifndef VK_DIRECTCONNECTION_H
#define VK_DIRECTCONNECTION_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "connection.h"

namespace vk {

struct AccessToken {
    QByteArray accessToken;
    int expireTime;
    int uid;
};

class DirectConnection : public Connection
{
    Q_OBJECT
public:
    explicit DirectConnection(QObject *parent = 0);

    virtual void connectToHost(const QString &login, const QString &password);
    virtual void disconnectFromHost();
    virtual QNetworkReply *request(const QString &method, const QVariantMap &args);
    virtual Client::State connectionState() const;
    virtual int uid() const;

    using Connection::get;
    QNetworkReply *get(const QString &method, const QVariantMap &args = QVariantMap());
protected:
    void setConnectionState(Client::State connectionState);
    void getToken(const QString &login, const QString &password);
protected slots:
    void getTokenFinished();
    void onReplyFinished();
    void onReplyError(QNetworkReply::NetworkError error);
private:
    Client::State m_connectionState;
    AccessToken m_token;
};

} // namespace vk

#endif // VK_DIRECTCONNECTION_H
