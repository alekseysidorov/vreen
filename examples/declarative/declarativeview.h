#ifndef DECLARATIVEVIEW_H
#define DECLARATIVEVIEW_H

#include <QDeclarativeView>

class DeclarativeView : public QDeclarativeView
{
    Q_OBJECT
public:
    explicit DeclarativeView(QWidget *parent = 0);
    
signals:
    
public slots:
    
};

#endif // DECLARATIVEVIEW_H
