#include "newsitem.h"
#include <QSharedData>

namespace vk {

class NewsItemData : public QSharedData {
public:
};

NewsItem::NewsItem() : data(new NewsItemData)
{
}

NewsItem::NewsItem(const NewsItem &rhs) : data(rhs.data)
{
}

NewsItem &NewsItem::operator=(const NewsItem &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

NewsItem::~NewsItem()
{
}

} // namespace vk
