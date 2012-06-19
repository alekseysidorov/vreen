#ifndef VK_LONGPOLL_H
#define VK_LONGPOLL_H

#include <QObject>
#include "contact.h"

namespace vk {

class Message;
class Client;
class LongPollPrivate;
class VK_SHARED_EXPORT LongPoll : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(LongPoll)
public:

    enum ServerAnswer {
        MessageDeleted      = 0,
        MessageFlagsReplaced= 1,
        MessageFlagsSet     = 2,
        MessageFlagsReseted = 3,
        MessageAdded		= 4,
        UserOnline          = 8,
		UserOffline         = 9,
		ChatTyping			= 61
		//GroupChatTyping	= 62,
		//UserCall			= 70

    };

    enum OfflineFlag {
        OfflineTimeout = 1
    };
    Q_DECLARE_FLAGS(OfflineFlags, OfflineFlag)

    enum Mode {
		NoRecieveAttachments = 0,
		RecieveAttachments = 2
    };

    LongPoll(Client *client);
    virtual ~LongPoll();
    void setMode(Mode mode);
    Mode mode() const;
signals:
	void messageAdded(const vk::Message &msg);
    void messageDeleted(int mid);
    void messageFlagsReplaced(int mid, int mask, int userId = 0);
    void messageFlagsReseted(int mid, int mask, int userId = 0);
	void contactStatusChanged(int userId, vk::Contact::Status status);
	void contactTyping(int userId);
public slots:
    void setRunning(bool set);
protected slots:
    void requestServer();
	void requestData(const QByteArray &timeStamp = QByteArray());
protected:
    QScopedPointer<LongPollPrivate> d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_request_server_finished(const QVariant &))
	Q_PRIVATE_SLOT(d_func(), void _q_on_data_recieved(const QVariant &))
};

} // namespace vk

Q_DECLARE_METATYPE(vk::LongPoll*)

#endif // VK_LONGPOLL_H
