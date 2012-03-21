#ifndef REPLY_P_H
#define REPLY_P_H
#include "reply.h"

namespace vk {

class ReplyPrivate
{
    Q_DECLARE_PUBLIC(Reply)
public:
    ReplyPrivate(Reply *q) : q_ptr(q) {}
    Reply *q_ptr;

    QNetworkReply *networkReply;
};


} //namespace vk

#endif // REPLY_P_H
