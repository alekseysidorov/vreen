#include <QString>
#include <QtTest>
#include <longpoll.h>
#include <client.h>

static QString getVariable(const char *name)
{
    return qgetenv(name);
}


class LongPollTest : public QObject
{
    Q_OBJECT
private slots:
    void testUpdates_data()
    {
        QTest::addColumn<QString>("login");
        QTest::addColumn<QString>("password");

        QTest::newRow("From enviroment variables VK_LOGIN and VK_PASSWORD")
                << getVariable("VK_LOGIN")
                << getVariable("VK_PASSWORD");
    }

    void testUpdates()
    {
        QFETCH(QString, login);
        QFETCH(QString, password);

        vk::Client client(login, password);

        QEventLoop loop;
        connect(&client, SIGNAL(onlineStateChanged(bool)), &loop, SLOT(quit()));
        connect(&client, SIGNAL(error(vk::Client::Error)), &loop, SLOT(quit()));
        client.connectToHost();
        loop.exec();

        if (!client.isOnline())
            QFAIL("Client is offline!");

        vk::LongPoll poll(&client);
        loop.exec();
    }
};

QTEST_MAIN(LongPollTest)

#include "tst_longpolltest.moc"
