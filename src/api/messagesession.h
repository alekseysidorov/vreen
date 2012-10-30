#ifndef VK_MESSAGESESSION_H
#define VK_MESSAGESESSION_H

#include <QObject>
#include "message.h"
#include "client.h"

namespace Vreen {

class Message;
class Client;
class MessageSessionPrivate;

class VK_SHARED_EXPORT MessageSession : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MessageSession)

    Q_PROPERTY(int uid READ uid CONSTANT)
    Q_PROPERTY(Client* client READ client CONSTANT)
public:
    explicit MessageSession(MessageSessionPrivate *data);
    virtual ~MessageSession();
    Client *client() const;
    int uid() const;
public slots:
    Reply *getHistory(int count = 16, int offset = 0);
    SendMessageReply *sendMessage(const QString &body, const QString &subject = QString());
    SendMessageReply *sendMessage(const Message &message);
    Reply *markMessagesAsRead(IdList ids, bool set = true);
signals:
    void messageAdded(const Vreen::Message &message);
    void messageDeleted(int id);
    void messageReadStateChanged(int mid, bool isRead);
protected:
    virtual SendMessageReply *doSendMessage(const Vreen::Message &message) = 0;
    virtual Reply *doGetHistory(int count, int offset) = 0;
    QScopedPointer<MessageSessionPrivate> d_ptr;
};

} // namespace Vreen

#endif // VK_MESSAGESESSION_H
