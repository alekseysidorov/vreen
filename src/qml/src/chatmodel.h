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
#ifndef CHATMODEL_H
#define CHATMODEL_H
#include "chatmodel.h"
#include <messagemodel.h>
#include <QPointer>

namespace Vreen {
class ChatSession;
} //namespace Vreen

class ChatModel : public Vreen::MessageListModel
{
    Q_OBJECT
    Q_PROPERTY(Vreen::Contact* contact READ contact WRITE setContact NOTIFY contactChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
public:
    explicit ChatModel(QObject *parent = 0);
    void setContact(Vreen::Contact *contact);
    Vreen::Contact *contact() const;
    QString title() const;
public slots:
    void getHistory(int count = 16, int offset = 0);
    void markAsRead(int mid, bool set = true);
signals:
    void contactChanged(Vreen::Contact*);
    void titleChanged(const QString &title);
    void requestFinished();
private slots:
    void messageReadStateChanged(int id, bool set);
private:
    QPointer<Vreen::ChatSession> m_session;
};

#endif // CHATMODEL_H

