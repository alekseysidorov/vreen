/****************************************************************************
**
** Vreen - vk.com API Qt bindings
**
** Copyright © 2012 Aleksey Sidorov <gorthauer87@ya.ru>
**
*****************************************************************************
**
** $VREEN_BEGIN_LICENSE$
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
** $VREEN_END_LICENSE$
**
****************************************************************************/
#ifndef CLIENT_H
#define CLIENT_H
#include <client.h>
#include <QWeakPointer>

class Client : public Vreen::Client
{
    Q_OBJECT
    Q_CLASSINFO("DefaultProperty", "connection")
public:
    Client(QObject *parent = 0);
    Q_INVOKABLE QObject *request(const QString &method, const QVariantMap &args = QVariantMap());
    Q_INVOKABLE Vreen::Contact *contact(int id);
signals:
    void messageReceived(Vreen::Contact *from);
private slots:
    void onOnlineStateChanged(bool state);
    void setOnline(bool set);
    void onMessageAdded(const Vreen::Message &msg);
    void onReplyCreated(Vreen::Reply *reply);
    void onReplyFinished(const QVariant &);
    void onReplyError(Vreen::Client::Error);
};

#endif // CLIENT_H

