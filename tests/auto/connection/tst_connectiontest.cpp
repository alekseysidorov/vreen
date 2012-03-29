#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <client.h>
#include <connection.h>
#include <reply.h>

#include "common/utils.h"

class ConnectionTest: public QObject
{
    Q_OBJECT
private slots:
    void testDirectConnection_data()
    {
        VK_ADD_LOGIN_VARS();
    }

    void testDirectConnection()
    {
        VK_CREATE_CLIENT();

        QCOMPARE(client.isOnline(), true);
    }

    void testDirectConnectionWrongData_data()
    {
        QTest::addColumn<QString>("login");
        QTest::addColumn<QString>("password");

        QTest::newRow("Wrong data")
                << QString("Foo")
                << QString("bar");
    }

    void testDirectConnectionWrongData()
    {
        VK_CREATE_CLIENT();

        QCOMPARE(client.isOnline(), false);
    }

    void testRequest_data()
    {
        VK_ADD_LOGIN_VARS();
    }

    void testRequest()
    {
        VK_CREATE_CLIENT();

        if (!client.isOnline())
            QFAIL("Client is offline!");

        vk::Reply *reply = client.request("getUserSettings");
        connect(reply, SIGNAL(resultReady(QVariant)), &loop, SLOT(quit()));
        loop.exec();
        QCOMPARE(reply->response().toInt() > 6, true);
    }
};

QTEST_MAIN(ConnectionTest)

#include "tst_connectiontest.moc"
