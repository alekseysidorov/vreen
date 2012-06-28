#ifndef VK_CLIENT_H
#define VK_CLIENT_H

#include "vk_global.h"
#include "reply.h"
#include <QScopedPointer>
#include <QObject>
#include <QVariantMap>

class QUrl;
namespace vk {

class Message;
class Connection;
class ClientPrivate;
class Reply;
class Roster;
class LongPoll;
class Contact;
class VK_SHARED_EXPORT Client : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Client)

    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged DESIGNABLE true)
    Q_PROPERTY(QString login READ login WRITE setLogin NOTIFY loginChanged DESIGNABLE true)
    Q_PROPERTY(bool online READ isOnline NOTIFY onlineStateChanged DESIGNABLE true)
    Q_PROPERTY(State connectionState READ connectionState NOTIFY connectionStateChanged DESIGNABLE true)
    Q_PROPERTY(vk::Roster* roster READ roster NOTIFY rosterChanged DESIGNABLE true)
    Q_PROPERTY(vk::LongPoll* longPoll READ longPoll NOTIFY longPollChanged DESIGNABLE true)
    Q_PROPERTY(vk::Contact* me READ me NOTIFY meChanged DESIGNABLE true)
	Q_PROPERTY(QString activity READ activity WRITE setActivity NOTIFY activityChanged DESIGNABLE true)
	Q_PROPERTY(bool invisible READ isInvisible WRITE setInvisible NOTIFY invisibleChanged)

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
	QString activity() const;
	void setActivity(const QString &activity);
	bool isInvisible() const;
	void setInvisible(bool set);

    Connection *connection() const;
    Connection *connection();
    void setConnection(Connection *connection);
    Roster *roster() const;
    Roster *roster();
	LongPoll *longPoll() const;
	LongPoll *longPoll();

    Reply *request(const QUrl &);
    Reply *request(const QString &method, const QVariantMap &args = QVariantMap());

	Reply *sendMessage(const Message &message);
    Reply *getLastDialogs(int count = 16, int previewLength = -1); //TODO move method    
    Reply *addLike(int ownerId, int postId, bool retweet = false, const QString &message = QString());
    Reply *deleteLike(int ownerId, int postId);

    Q_INVOKABLE Contact *me() const;
public slots:
    void connectToHost();
    void connectToHost(const QString &login, const QString &password);
    void disconnectFromHost();
signals:
    void loginChanged(const QString &login);
    void passwordChanged(const QString &password);
    void connectionStateChanged(vk::Client::State state);
    void replyCreated(vk::Reply*);
    void error(vk::Reply*);
    void error(vk::Client::Error);
    void onlineStateChanged(bool state);
    void rosterChanged(vk::Roster*);
    void longPollChanged(vk::LongPoll*);
    void meChanged(vk::Contact *me);
	void activityChanged(const QString &activity);
	void invisibleChanged(bool set);
protected:
	Reply *setStatus(const QString &text, int aid = 0);
    QScopedPointer<ClientPrivate> d_ptr;

private:

    Q_PRIVATE_SLOT(d_func(), void _q_connection_state_changed(vk::Client::State))
    Q_PRIVATE_SLOT(d_func(), void _q_error_received(int))
    Q_PRIVATE_SLOT(d_func(), void _q_reply_finished(const QVariant &))
	Q_PRIVATE_SLOT(d_func(), void _q_activity_update_finished(const QVariant &))
	Q_PRIVATE_SLOT(d_func(), void _q_update_online())
};

} // namespace vk

Q_DECLARE_METATYPE(vk::Client*)
Q_DECLARE_METATYPE(vk::Reply*)

#endif // VK_CLIENT_H
