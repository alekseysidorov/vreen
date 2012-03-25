#include "newsmodel_p.h"

namespace vk {

static const char *types[] = {
    "post",
    "photo",
    "photo_tag",
    "friend",
    "note"
};

NewsModel::NewsModel(Client *client) :
    QAbstractListModel(client),
    d_ptr(new NewsModelPrivate(this))
{
    Q_D(NewsModel);
    d->client = client;

    auto names = roleNames();
    names[TypeRole] = "type";
    names[ItemRole] = "item";
    names[SourceRole] = "source";
    names[IsGroupRole] = "isGroup";
    setRoleNames(names);
}

NewsModel::~NewsModel()
{

}

int NewsModel::count() const
{
    return rowCount(QModelIndex());
}

void NewsModel::update()
{
    Q_D(NewsModel);
    QVariantMap filter; //TODO implement filters!
    auto reply = d->client->request("newsfeed.get", filter);
    //TODO i want Qt5 for lambdas(((
    connect(reply, SIGNAL(resultReady(QVariant)), SLOT(_q_update_ready(QVariant)));
}

QVariant NewsModel::data(const QModelIndex &index, int role) const
{
    Q_D(const NewsModel);
    NewsItem news = d->newsList.at(index.row());
    switch (role) {
    case TypeRole:
        return news.type;
    case ItemRole:
        return news.data;
    case SourceRole:
        return news.source;
    case IsGroupRole:
        return news.isGroup;
    default:
        break;
    }
    return QVariant::Invalid;
}

int NewsModel::rowCount(const QModelIndex &parent) const
{
    Q_ASSERT(!parent.isValid());
    return d_func()->newsList.count();
}

void NewsModelPrivate::_q_update_ready(const QVariant &response)
{
    Q_Q(NewsModel);
    auto reply = sender_cast<Reply*>(q->sender());
    auto map = response.toMap();
    auto items = map.value("items").toList();
    q->beginInsertRows(QModelIndex(), 0, items.count());
    for (auto item : items) {
        auto data = item.toMap();
        NewsItem newsItem;
        newsItem.type = strToEnum<NewsModel::ItemType>(data.value("type").toString(), types);
        newsItem.data = data;
        newsList.prepend(newsItem);
    }
    q->endInsertRows();

    reply->deleteLater();
}


} //namespace vk

#include "moc_newsmodel.cpp"
