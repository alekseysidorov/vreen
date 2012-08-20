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
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see http://www.gnu.org/licenses/.
** $VKIT_END_LICENSE$
**
****************************************************************************/
#ifndef VK_REPLY_H
#define VK_REPLY_H

#include <QObject>
#include "vk_global.h"

class QNetworkReply;
namespace Vreen {

class ReplyPrivate;
class VK_SHARED_EXPORT Reply : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Reply)
public:
    virtual ~Reply();
    QNetworkReply *networkReply() const;
    QVariant response() const;
    QVariant error() const;
signals:
    void resultReady(const QVariant &variables);
    void error(int code);
protected:
    explicit Reply(QNetworkReply *networkReply = 0);
    void setReply(QNetworkReply *networkReply);

    QScopedPointer<ReplyPrivate> d_ptr;

    friend class Client;

    Q_PRIVATE_SLOT(d_func(), void _q_reply_finished())
    Q_PRIVATE_SLOT(d_func(), void _q_network_reply_error(QNetworkReply::NetworkError))
};

} // namespace Vreen

#endif // VK_REPLY_H

