#include "declarativeview.h"
#include <QDeclarativeContext>

DeclarativeView::DeclarativeView(QWidget *parent) :
    QDeclarativeView(parent)
{
    //auto client = new Vreen::Client(this);
    //auto con = new Vreen::OAuthConnection(1865463, client); //qutIM id
    //con->setConnectionOption(Vreen::Connection::ShowAuthDialog, true);
    //con->setDisplayType(Vreen::OAuthConnection::Popup);
    //client->setConnection(con);
    //rootContext()->setContextProperty("client", client);

    setSource(QUrl("qml/main.qml"));
    setResizeMode(SizeRootObjectToView);
}
