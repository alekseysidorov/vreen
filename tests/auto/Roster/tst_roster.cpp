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
#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include "common/utils.h"
#include "roster.h"
#include "contact.h"

#include <QMetaProperty>
#include <QDebug>

class RosterTest : public QObject
{
    Q_OBJECT
    
public:
    RosterTest()
    {
        VREEN_TEST_PREPARE();
    }
    
private Q_SLOTS:
    void testSync_data()
    {
        VREEN_ADD_LOGIN_VARS();
    }

    void testSync()
    {
        VREEN_CREATE_CLIENT();
        if (!client.isOnline())
            QFAIL("Client is offline!");

        Vreen::Roster *roster = client.roster();
        connect(roster, SIGNAL(syncFinished(bool)), &loop, SLOT(quit()));
        roster->sync();
        loop.exec();

        QCOMPARE(roster->buddies().count() > 0, true);

        foreach (Vreen::Contact *contact, roster->buddies()) {
            qDebug() << contact->name() << ":" << "\n";
            const QMetaObject *meta = contact->metaObject();
            for (int index = 0; index != meta->propertyCount(); index++) {
                QMetaProperty property = meta->property(index);
                qDebug() << "Property name: " << property.name() << ". Value: " << property.read(contact);
            }
        }
    }
};

QTEST_MAIN(RosterTest)

#include "tst_roster.moc"

