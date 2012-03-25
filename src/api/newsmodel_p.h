#ifndef NEWSMODEL_P_H
#define NEWSMODEL_P_H
#include "newsmodel.h"

namespace vk {

struct News
{

};
typedef QList<News> NewsList;

class NewsModel;
class NewsModelPrivate
{
    Q_DECLARE_PUBLIC(NewsModel)
public:
    NewsModelPrivate(NewsModel *q) : q_ptr(q) {}
    NewsModel *q_ptr;
};


} //namespace vk

#endif // NEWSMODEL_P_H
