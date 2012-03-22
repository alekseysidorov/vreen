#ifndef VK_LONGPOLL_H
#define VK_LONGPOLL_H

#include <QObject>

namespace vk {

class Client;
class LongPollPrivate;
class LongPoll : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(LongPoll)
public:

    enum ServerAnswer {
        AnswerMessageDeleted      = 0,
        AnswerMessageFlagsReplaced= 1,
        AnswerMessageFlagsSet     = 2,
        AnswerMessageFlagsReseted = 3,
        AnswerMessageAdded		= 4,
        AnswerUserOnline          = 8,
        AnswerUserOffline         = 9
    };

    enum MessageFlag {
        FlagMessageUnread   = 1,
        FlagMessageOutbox   = 2,
        FlagMessageReplied  = 4,
        FlagMessageImportant= 8,
        FlagMessageChat     = 16,
        FlagMessageFriends  = 32,
        FlagMessageSpam     = 64,
        FlagMessageDeleted  = 128,
        FlagMessageFixed    = 256,
        FlagMessageMedia    = 512
    };
    Q_DECLARE_FLAGS(MessageFlags, MessageFlag)

    enum OfflineFlag {
        OfflineTimeout = 1
    };
    Q_DECLARE_FLAGS(OfflineFlags, OfflineFlag)

    enum Mode {
        NoRecevieAttachments = 0,
        ReceiveAttachments = 2
    };

    LongPoll(Client *client);
    virtual ~LongPoll();
    void setMode(Mode mode);
    Mode mode() const;
public slots:
    void setRunning(bool set);
protected slots:
    void requestServer();
    void requestData(const QByteArray &timeStamp);
protected:
    QScopedPointer<LongPollPrivate> d_ptr;
};

} // namespace vk

#endif // VK_LONGPOLL_H
