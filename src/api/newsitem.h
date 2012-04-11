#ifndef VK_NEWSITEM_H
#define VK_NEWSITEM_H

#include <QSharedDataPointer>

namespace vk {

class NewsItemData;

class NewsItem
{
public:

    enum Type {
        Post,
        Photo,
        PhotoTag,
        Note
    };

    NewsItem();
    NewsItem(const NewsItem &);
    NewsItem &operator=(const NewsItem &);
    ~NewsItem();
    
private:
    QSharedDataPointer<NewsItemData> data;
};

} // namespace vk

#endif // VK_NEWSITEM_H
