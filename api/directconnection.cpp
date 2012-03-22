#include "directconnection_p.h"
#include "json.h"
#include <QUrl>
#include <QNetworkReply>

#include <QDebug>

namespace vk {

//TODO use normal framework
struct LoginVars {
    const char *grantType;
    const char *clientId;
    const char *clientSecret;
    const char *appName;
    int scope;
};

static const LoginVars loginVars = {
    "password",
    "1950109",
    "bJKfYSu0LS6N52M0HnBo",
    "TitanIM",
    2 + 4 + 8 + 16 + 1024 + 4096 + 8192 //FIXME в нормальный вид привести

};

struct ServerUrls {
    QUrl apiServer;
    QUrl authServer;
};

static const ServerUrls serverUrls = {
    QUrl(QLatin1String("https://api.vk.com/method/")),
    QUrl(QLatin1String("https://api.vk.com/oauth/token"))
};

DirectConnection::DirectConnection(QObject *parent) :
    Connection(parent)
{
}

void DirectConnection::connectToHost(const QString &login, const QString &password)
{
    setConnectionState(Client::StateConnecting);

    //TODO add token check
    getToken(login, password);
}

void DirectConnection::disconnectFromHost()
{
    setConnectionState(Client::StateOffline);
}

Reply *DirectConnection::request(const QString &method, const QVariantMap &args)
{
}

Client::State DirectConnection::connectionState() const
{
    return m_connectionState;
}

void DirectConnection::setConnectionState(Client::State state)
{
    if (m_connectionState != state) {
        m_connectionState = state;
        emit connectionStateChanged(state);
    }
}

void DirectConnection::getToken(const QString &login, const QString &password)
{
    QUrl url = serverUrls.authServer;
    url.addEncodedQueryItem("grant_type", loginVars.grantType);
    url.addEncodedQueryItem("client_id", loginVars.clientId);
    url.addEncodedQueryItem("client_secret", loginVars.clientSecret);
    url.addEncodedQueryItem("scope", QByteArray::number(loginVars.scope));

    url.addQueryItem(QLatin1String("username"), login); //TODO use C++11 literals in suffix
    url.addQueryItem(QLatin1String("password"), password);

    QNetworkReply *reply = get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()), SLOT(getTokenFinished()));
}

void DirectConnection::getTokenFinished()
{
    auto reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();

    switch (reply->error()) {
    case QNetworkReply::NoError: {
        QVariantMap response = JSON::parse(reply->readAll()).toMap();
        if (!response.value("access_token").isNull()) {
            m_token.token = response.value("access_token").toByteArray();
            m_token.expireTime = response.value("expires_in").toInt();
            m_token.uid = response.value("user_id").toInt();
            setConnectionState(Client::StateOnline);
            return;
        }
        break;
    }
    case QNetworkReply::AuthenticationRequiredError: //TODO
    default:
        disconnectFromHost();
        emit error(Client::ServerIsUnavailableError);
        break;
    }
}

} // namespace vk
