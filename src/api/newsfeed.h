#ifndef NEWSFEED_H
#define NEWSFEED_H
#include <QObject>
#include "newsitem.h"
#include <QVariantMap>

namespace vk {

class NewsFeedPrivate;
class Client;
class Reply;

class VK_SHARED_EXPORT NewsFeed : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(NewsFeed)
    Q_ENUMS(Filter)
public:

    enum Filter {
        FilterNone      =   0,
        FilterPost      =   0x01,
        FilterPhoto     =   0x02,
        FilterPhotoTag  =   0x04,
        FilterFriend    =   0x08,
        FilterNote      =   0x10
    };
    Q_DECLARE_FLAGS(Filters, Filter)

    NewsFeed(Client *client);
    virtual ~NewsFeed();
public slots:
    Reply *getNews(Filters filters = FilterNone, quint8 count = 25, int offset = 0);
signals:
    void newsAdded(const vk::NewsItem &item);
private:
    QScopedPointer<NewsFeedPrivate> d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_news_received(QVariant))
};

} //namespace vk

Q_DECLARE_OPERATORS_FOR_FLAGS(vk::NewsFeed::Filters)

#endif // NEWSFEED_H
