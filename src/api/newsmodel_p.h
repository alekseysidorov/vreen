#ifndef NEWSMODEL_P_H
#define NEWSMODEL_P_H
#include "newsmodel.h"
#include "client.h"

namespace vk {

//struct NewsItem
//{
//    NewsModel::ItemType type;
//    int sourceId;
//    time_t date;
//};

struct NewsItem
{
    NewsModel::ItemType type;
    QVariant data;
    QVariant source;
    bool isGroup;
    //time_t time;
};
typedef QList<NewsItem> NewsList;

class NewsModel;
class NewsModelPrivate
{
    Q_DECLARE_PUBLIC(NewsModel)
public:
    NewsModelPrivate(NewsModel *q) : q_ptr(q), client(0) {}
    NewsModel *q_ptr;

    Client *client;
    NewsList newsList;

    void _q_update_ready(const QVariant &response);
};


} //namespace vk

#endif // NEWSMODEL_P_H
