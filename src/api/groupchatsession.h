#ifndef VK_GROUPCHATSESSION_H
#define VK_GROUPCHATSESSION_H
#include "messagesession.h"
#include "contact.h"

namespace vk {

class GroupChatSessionPrivate;

class GroupChatSession : public vk::MessageSession
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(GroupChatSession)

    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
public:
    explicit GroupChatSession(int chatId, Client *parent);

    ContactList participants() const;
    Buddy *admin() const;
    QString title() const;

    virtual Reply *getHistory(int count = 16, int offset = 0) = 0;
    virtual Reply *sendMessage(const QString &body, const QString &subject = QString(),
                               const Attachment &attachment = Attachment()) = 0;
public slots:
    Reply *getInfo();
    Reply *addParticipant(Contact *buddy);
    Reply *removeParticipant(Contact *buddy);
    Reply *updateTitle(const QString &title);
signals:
    void participantAdded(vk::Contact*);
    void participantRemoved(vk::Contact*);
    void titleChanged(QString arg);
protected:
    void setTitle(const QString &title);
private:
    Q_PRIVATE_SLOT(d_func(), void _q_history_received(const QVariant &response))
    Q_PRIVATE_SLOT(d_func(), void _q_info_received(const QVariant &response))
    Q_PRIVATE_SLOT(d_func(), void _q_participant_added(const QVariant &response))
    Q_PRIVATE_SLOT(d_func(), void _q_participant_removed(const QVariant &response))
    Q_PRIVATE_SLOT(d_func(), void _q_title_updated(const QVariant &response))
};

} // namespace vk

#endif // VK_GROUPCHATSESSION_H
