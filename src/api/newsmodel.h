#ifndef NEWSMODEL_H
#define NEWSMODEL_H

#include <QAbstractListModel>

namespace vk {

class NewsModelPrivate;
class NewsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(NewsModel)
public:
    NewsModel(QObject *parent = 0);
    virtual ~NewsModel();
protected:
    QScopedPointer<NewsModelPrivate> d_ptr;
};

} //namespace vk


#endif // NEWSMODEL_H
