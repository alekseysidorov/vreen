#ifndef VK_CLIENT_H
#define VK_CLIENT_H

#include "vk_global.h"
#include <QScopedPointer>
#include <QObject>

namespace vk {

class ClientPrivate;
class VK_SHARED_EXPORT Client : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Client)

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
protected:
    QScopedPointer<ClientPrivate> d_ptr;
};


} // namespace vk
#endif // VK_CLIENT_H
