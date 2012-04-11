#ifndef VK_CHATSESSION_H
#define VK_CHATSESSION_H

#include "message.h"

namespace vk {

class ChatSessionPrivate;
class VK_SHARED_EXPORT ChatSession : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ChatSession)

    Q_PROPERTY(Contact *contact READ contact CONSTANT)
public:
    ChatSession(Contact *contact);
    virtual ~ChatSession();

    Contact *contact() const;
    void markMessagesAsRead(IdList ids, bool set = true);
    QString title() const;
    bool isActive() const;
    void setActive(bool set);
public slots:
    void getHistory(int count = 16, int offset = 0);
signals:
    void messageAdded(const vk::Message &message);
    void messageDeleted(int id);
    void messageReadStateChanged(int mid, bool isRead);
private:
    QScopedPointer<ChatSessionPrivate> d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_history_received(const QVariant &))
    Q_PRIVATE_SLOT(d_func(), void _q_message_added(const vk::Message &))
};

} // namespace vk

#endif // VK_CHATSESSION_H
