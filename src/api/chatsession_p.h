/****************************************************************************
**
** VKit - vk.com API Qt bindings
**
** Copyright © 2012 Aleksey Sidorov <gorthauer87@ya.ru>
**
*****************************************************************************
**
** $VKIT_BEGIN_LICENSE$
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see http://www.gnu.org/licenses/.
** $VKIT_END_LICENSE$
**
****************************************************************************/
#ifndef CHATSESSION_P_H
#define CHATSESSION_P_H
#include "chatsession.h"

namespace vk {

class ChatSessionPrivate
{
    Q_DECLARE_PUBLIC(ChatSession)
public:
    ChatSessionPrivate(ChatSession *q, Contact *contact) : q_ptr(q),
        contact(contact), isActive(false) {}
    ChatSession *q_ptr;

    Contact *contact;
    bool isActive;

    void _q_history_received(const QVariant &);
    void _q_message_read_state_updated(const QVariant &);
    void _q_message_added(const Message &message);
};

} //namespace vk


#endif // CHATSESSION_P_H

