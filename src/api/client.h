#ifndef VK_CLIENT_H
#define VK_CLIENT_H

#include "vk_global.h"
#include <QScopedPointer>
#include <QObject>
#include <QVariantMap>

class QUrl;
namespace vk {

class Connection;
class ClientPrivate;
class Reply;
class VK_SHARED_EXPORT Client : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Client)

    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString login READ login WRITE setLogin NOTIFY loginChanged)
    Q_PROPERTY(bool isOnline READ isOnline NOTIFY onlineStateChanged)
    Q_PROPERTY(State connectionState READ connectionState NOTIFY connectionStateChanged)

    Q_ENUMS(State)
    Q_ENUMS(Error)
public:

    enum State {
        StateOffline,
        StateConnecting,
        StateOnline,
        StateInvalid
    };
    enum Error {
        ServerIsUnavailableError,
        AuthorizationError,
        UnknownMethodPassedError,
        IncorrectSignatureError
    };

    explicit Client(QObject *parent = 0);
    explicit Client(const QString &login, const QString &password, QObject *parent = 0);
    virtual ~Client();
    QString password() const;
    void setPassword(const QString &password);
    QString login() const;
    void setLogin(const QString &login);
    State connectionState() const;
    bool isOnline() const;

    Connection *connection() const;
    Connection *connection();
    void setConnection(Connection *connection);

    Q_INVOKABLE Reply *request(const QUrl &);
    Q_INVOKABLE Reply *request(const QString &method, const QVariantMap &args = QVariantMap());
public slots:
    void connectToHost();
    void connectToHost(const QString &login, const QString &password);
    void disconnectFromHost();
signals:
    void loginChanged(const QString &login);
    void passwordChanged(const QString &password);
    void connectionStateChanged(vk::Client::State state);
    void error(vk::Client::Error error);
    void onlineStateChanged(bool state);
protected:
    QScopedPointer<ClientPrivate> d_ptr;

private:

    Q_PRIVATE_SLOT(d_func(), void _q_connection_state_changed(vk::Client::State))
    Q_PRIVATE_SLOT(d_func(), void _q_error_received(vk::Client::Error))
};


} // namespace vk
#endif // VK_CLIENT_H
