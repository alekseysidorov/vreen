#include "newsfeedmodel.h"
#include <QApplication>
#include <roster.h>
#include <client.h>
#include <utils.h>
#include <QDateTime>
#include <QNetworkReply>

NewsFeedModel::NewsFeedModel(QObject *parent) :
    QAbstractListModel(parent),
    m_sortOrder(Qt::DescendingOrder)
{
    auto roles = roleNames();
    roles[TypeRole] = "type";
    roles[PostIdRole] = "postId";
    roles[SourceRole] = "source";
    roles[DateRole] = "date";
    roles[BodyRole] = "body";
    roles[AttachmentsRole] = "attachments";
    roles[LikesRole] = "likes";
    roles[RepostsRole] = "reposts";
    roles[CommentsRole] = "comments";
    roles[OwnerNameRole] = "ownerName";
    roles[SourcePhotoRole] = "sourcePhoto";
    roles[SourceNameRole] = "sourceName";
    roles[LikesCount] = "likesCount";
    roles[CommentsCount] = "commentsCount";
    setRoleNames(roles);
}

QObject *NewsFeedModel::client() const
{
    return m_client.data();
}

void NewsFeedModel::setClient(QObject *obj)
{
    auto client = static_cast<vk::Client*>(obj);
    m_client = client;
    if (m_newsFeed.data())
        m_newsFeed.data()->deleteLater();
    if (!client)
        return;

    auto newsFeed = new vk::NewsFeed(client);
    connect(newsFeed, SIGNAL(newsAdded(vk::NewsItem)), SLOT(onNewsAdded(vk::NewsItem)));

    m_newsFeed = newsFeed;
}

QVariant NewsFeedModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    auto news = m_newsList.at(row);
    switch (role) {
    case TypeRole:
        return news.type();
    case PostIdRole:
        return news.postId();
        break;
    case SourceRole: {
        int source = news.sourceId();
        return qVariantFromValue(findContact(source));
    }
    case DateRole:
        return news.date();
    case BodyRole:
        return news.body();
    case AttachmentsRole:
        return vk::Attachment::toVariantMap(news.attachments());
    case LikesRole:
        return news.likes();
    case RepostsRole:
        return news.reposts();
    case CommentsRole:
        return news.property("comments");
    case OwnerNameRole: {
        int ownerId = news.property("copy_owner_id").toInt();
        if (ownerId) {
            auto contact = m_client.data()->roster()->contact(ownerId);
            return contact->name();
        }
        return QVariant();
    }
    case SourcePhotoRole: {
        if (auto contact = findContact(news.sourceId()))
            return contact->photoSource();
        break;
    }
    case SourceNameRole: {
        if (auto contact = findContact(news.sourceId()))
            return contact->name();
        break;
    }
    case LikesCount: {
        return news.likes().value("count").toInt();
    }
    case CommentsCount:
        return news.property("comments").toMap().value("count").toInt();
    default:
        break;
    }
    return QVariant::Invalid;
}

int NewsFeedModel::rowCount(const QModelIndex &) const
{
    return count();
}

int NewsFeedModel::count() const
{
    return m_newsList.count();
}

void NewsFeedModel::getNews(int filters, quint8 count, int offset)
{
    if (m_newsFeed.isNull())
        return;

    qDebug() << Q_FUNC_INFO << filters << count << offset;
    auto reply = m_newsFeed.data()->getNews(static_cast<vk::NewsFeed::Filters>(filters), count, offset);
    connect(reply, SIGNAL(resultReady(QVariant)), SIGNAL(requestFinished()));
}

void NewsFeedModel::addLike(int postId, bool retweet, const QString &message)
{
    int index = findNews(postId);
    if (index != -1) {
        auto news = m_newsList.at(index);
        auto reply = m_client.data()->addLike(news.sourceId(),
                                              postId,
                                              retweet,
                                              message);
        connect(reply, SIGNAL(resultReady(QVariant)), SLOT(onAddLike(QVariant)));
    }
}

void NewsFeedModel::deleteLike(int postId)
{
    int index = findNews(postId);
    if (index != -1) {
        auto news = m_newsList.at(index);
        auto reply = m_client.data()->deleteLike(news.sourceId(),
                                                 postId);
        connect(reply, SIGNAL(resultReady(QVariant)), SLOT(onDeleteLike(QVariant)));
    }
}

int NewsFeedModel::findNews(int id)
{
    for (int i = 0 ; i != count(); i++) {
        if (id == m_newsList.at(i).postId())
            return i;
    }
    return -1;
}

void NewsFeedModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_newsList.count());
    m_newsList.clear();
    endRemoveRows();
}

void NewsFeedModel::truncate(int count)
{
    if (count >= m_newsList.count())
        count = m_newsList.count() - 1;

    beginRemoveRows(QModelIndex(), count, m_newsList.count() - 1);
    m_newsList.erase(m_newsList.begin() + count - 1, m_newsList.end());
    endRemoveRows();
}

static bool newsItemMoreThan(const vk::NewsItem &a, const vk::NewsItem &b)
{
    return a.date() > b.date();
}

void NewsFeedModel::onNewsAdded(const vk::NewsItem &item)
{
    if (findNews(item.postId()) != -1)
        return;

    auto index = vk::lowerBound(m_newsList, item, newsItemMoreThan);
    insertNews(index, item);
}

void NewsFeedModel::onAddLike(const QVariant &response)
{
    auto reply = vk::sender_cast<vk::Reply*>(sender());
    auto url = reply->networkReply()->url();

    int postId = url.queryItemValue("post_id").toInt();
    int retweet = url.queryItemValue("repost").toInt();
    auto map = response.toMap();
    int likes = map.value("likes").toInt();
    int reposts = map.value("reposts").toInt();

    int index = findNews(postId);
    if (index != -1) {
        auto news = m_newsList.at(index);

        map = news.likes();
        map.insert("count", likes);
        map.insert("user_likes", true);
        news.setLikes(map);

        map = news.reposts();
        map.insert("count", reposts);
        map.insert("user_reposted", retweet);
        news.setReposts(map);

        replaceNews(index, news);
    }
}

void NewsFeedModel::onDeleteLike(const QVariant &response)
{
    auto reply = vk::sender_cast<vk::Reply*>(sender());
    auto url = reply->networkReply()->url();

    int postId = url.queryItemValue("post_id").toInt();
    int likes = response.toMap().value("likes").toInt();
    int index = findNews(postId);
    if (index != -1) {
        auto news = m_newsList.at(index);

        auto map = news.likes();
        map.insert("count", likes);
        map.insert("user_likes", false);
        news.setLikes(map);

        map = news.reposts();
        map.insert("user_reposted", false);
        news.setReposts(map);

        replaceNews(index, news);
    }
}


void NewsFeedModel::insertNews(int index, const vk::NewsItem &item)
{
    beginInsertRows(QModelIndex(), index, index);
    m_newsList.insert(index, item);
    endInsertRows();
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
}

void NewsFeedModel::replaceNews(int i, const vk::NewsItem &news)
{
    auto index = createIndex(i, 0);
    m_newsList[i] = news;
    emit dataChanged(index, index);
}

vk::Contact *NewsFeedModel::findContact(int postId) const
{
    return m_client.data()->roster()->contact(postId);
}
