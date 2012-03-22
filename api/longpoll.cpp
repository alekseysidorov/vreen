#include "longpoll_p.h"

namespace vk {

LongPoll::LongPoll(Client *client) :
    QObject(client),
    d_ptr(new LongPollPrivate(this))
{

}

LongPoll::~LongPoll()
{

}

} // namespace vk
