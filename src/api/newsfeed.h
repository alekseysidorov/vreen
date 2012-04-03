#ifndef NEWSFEED_H
#define NEWSFEED_H
#include <QSharedData>

namespace vk {

class NewsFeedItemData;
class NewsFeedItem
{
public:

protected:
    QSharedDataPointer<NewsFeedItemData> d;
};

} //namespace vk

#endif // NEWSFEED_H
