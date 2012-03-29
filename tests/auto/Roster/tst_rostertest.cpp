#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include "common/utils.h"
#include "roster.h"
#include "buddy.h"

#include <QMetaProperty>
#include <QDebug>

class RosterTest : public QObject
{
    Q_OBJECT
    
public:
    RosterTest()
    {

    }
    
private Q_SLOTS:
    void testSync_data()
    {
        VK_ADD_LOGIN_VARS();
    }

    void testSync()
    {
        VK_CREATE_CLIENT();
        if (!client.isOnline())
            QFAIL("Client is offline!");

        vk::Roster *roster = client.roster();
        connect(roster, SIGNAL(syncFinished(bool)), &loop, SLOT(quit()));
        roster->sync();
        loop.exec();

        QCOMPARE(roster->contacts().count() > 0, true);

        foreach (vk::Contact *contact, roster->contacts()) {
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

#include "tst_rostertest.moc"
