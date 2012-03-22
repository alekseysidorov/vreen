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
    LongPoll(Client *client);
    virtual ~LongPoll();
protected:
    QScopedPointer<LongPollPrivate> d_ptr;
};

} // namespace vk

#endif // VK_LONGPOLL_H
