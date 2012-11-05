#include "declarativeview.h"
#include <QDeclarativeContext>
#include <vreen/auth/oauthconnection.h>
#include <vreen/client.h>

DeclarativeView::DeclarativeView(QWidget *parent) :
    QDeclarativeView(parent)
{
    auto client = new Vreen::Client(this);
    auto con = new Vreen::OAuthConnection(1865463, client); //qutIM id
    client->setConnection(con);

    setSource(QUrl("qml/main.qml"));
    rootContext()->setContextProperty("client", client);
}
