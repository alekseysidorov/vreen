#ifndef LONGPOLL_P_H
#define LONGPOLL_P_H

#include "longpoll.h"
#include "client.h"

namespace vk {

class LongPoll;
class LongPollPrivate
{
    Q_DECLARE_PUBLIC(LongPoll)
public:
    LongPollPrivate(LongPoll *q) : q_ptr(q) {}
    LongPoll *q_ptr;
};


} //namespace vk

#endif // LONGPOLL_P_H
