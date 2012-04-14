#include "newsfeed.h"
#include "utils.h"
#include "client.h"
#include "roster.h"
#include <QVariantList>

namespace vk {

static const char *filters_str[] = {
    "post",
    "photo",
    "photo_tag",
    "friend",
    "note"
};

class NewsFeed;
class NewsFeedPrivate
{
    Q_DECLARE_PUBLIC(NewsFeed)
public:
    NewsFeedPrivate(NewsFeed *q, Client *client) : q_ptr(q), client(client),
        offset(0), fromId(0) {}
    NewsFeed *q_ptr;
    Client *client;
    int offset;
    int fromId;

    void updateProfiles(const QVariantList &profiles)
    {
        foreach (auto item, profiles) {
            auto map = item.toMap();
            auto roster = client->roster();
            int uid = map.value("uid").toInt();
            auto contact = roster->contact(uid);
            Roster::fillContact(contact, map);
        }
    }

    void updateGroups(const QVariantList &groups)
    {
        foreach (auto item, groups) {
            auto map = item.toMap();
            auto roster = client->roster();
            int gid = -map.value("gid").toInt();
            auto contact = roster->contact(gid, Contact::GroupType);
            Roster::fillContact(contact, map);
        }
    }

    void _q_news_received(const QVariant &response)
    {
        Q_Q(NewsFeed);
        auto map = response.toMap();
        updateProfiles(map.value("profiles").toList());
        updateGroups(map.value("groups").toList());

        auto items = map.value("items").toList();
        foreach (auto item, items) {
            auto newsItem = NewsItem::fromData(item);
            auto map = item.toMap();
            for (int i = 0; i != sizeof(filters_str); i++) {
                auto list = map.value(filters_str[i]).toList();
                if (list.count()) {
                    auto count = list.takeFirst().toInt();
                    newsItem.setAttachments(Attachment::fromVariantList(list));
                    break;
                }
            }
            emit q->newsAdded(newsItem);
        }
        q->setOffset(map.value("new_offset").toInt());
        q->setFrom(map.value("new_from").toInt());
    }
};

/*!
 * \brief The NewsFeed class
 * Api reference: \link http://vk.com/developers.php?oid=-1&p=Расширенные_методы_API
 */

/*!
 * \brief NewsFeed::NewsFeed
 * \param client
 */
NewsFeed::NewsFeed(Client *client) :
    QObject(client),
    d_ptr(new NewsFeedPrivate(this, client))
{

}

NewsFeed::~NewsFeed()
{

}

int NewsFeed::offset() const
{
    return d_func()->offset;
}

int NewsFeed::from() const
{
    return d_func()->fromId;
}

void NewsFeed::setOffset(int offset)
{
    Q_D(NewsFeed);
    d->offset = offset;
    emit offsetChanged(offset);
}

void NewsFeed::setFrom(int fromId)
{
    Q_D(NewsFeed);
    d->fromId = fromId;
    emit fromChanged(fromId);
}

Reply *NewsFeed::getLatestNews(Filters filters, quint8 count)
{
    QVariantMap args;
    args.insert("count", count);
    args.insert("filters", flagsToStrList(filters, filters_str).join(","));
    //    if (offset())
    //        args.insert("offset", offset());
    //    if (from())
    //        args.insert("from", offset());
    auto reply = d_func()->client->request("newsfeed.get", args);
    connect(reply, SIGNAL(resultReady(QVariant)), SLOT(_q_news_received(QVariant)));
    return reply;
}

} //namespace vk

#include "moc_newsfeed.cpp"
