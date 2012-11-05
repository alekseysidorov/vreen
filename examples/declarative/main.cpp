#include <QApplication>
#include "declarativeview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DeclarativeView view;
    view.showNormal();
    return a.exec();
}
