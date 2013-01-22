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
#ifndef DIALOGSMODEL_H
#define DIALOGSMODEL_H
#include <messagemodel.h>
#include <roster.h>
#include <QPointer>

class DialogsModel : public Vreen::MessageListModel
{
    Q_OBJECT

    Q_PROPERTY(int unreadCount READ unreadCount NOTIFY unreadCountChanged)
public:
    explicit DialogsModel(QObject *parent = 0);

    void setUnreadCount(int count);
    int unreadCount() const;
public slots:
    Vreen::Reply *getDialogs(int offset = 0, int count = 25, int previewLength = -1);
signals:
    void clientChanged(QObject*);
    void unreadCountChanged(int count);
protected:
    virtual void doReplaceMessage(int index, const::Vreen::Message &message);
    virtual void doInsertMessage(int index, const::Vreen::Message &message);
    virtual void doRemoveMessage(int index);
private slots:
    void onDialogsReceived(const QVariant &dialogs);
private:
    int m_unreadCount;
};

#endif // DIALOGSMODEL_H

