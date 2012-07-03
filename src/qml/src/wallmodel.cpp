#include "wallmodel.h"
#include <QApplication>
#include <roster.h>
#include <wallpost.h>
#include <QDateTime>
#include <QDebug>
#include <QNetworkReply>

WallModel::WallModel(QObject *parent) :
    QAbstractListModel(parent)
{
    auto roles = roleNames();
    roles[IdRole] = "postId";
    roles[BodyRole] = "body";
    roles[FromRole] = "from";
    roles[ToRole] = "to";
    roles[DateRole] = "date";
    roles[AttachmentsRole] = "attachments";
    roles[LikesRole] = "likes";
    roles[RepostsRole] = "reposts";
    roles[CommentsRole] = "comments";
    setRoleNames(roles);
}

vk::Contact *WallModel::contact() const
{
    return m_contact.data();
}

void WallModel::setContact(vk::Contact *contact)
{
    if (!m_session.isNull()) {
        clear();
        m_session.data()->deleteLater();
    }
    if (!contact)
        return;
    auto session = new vk::WallSession(contact);
    connect(session, SIGNAL(postAdded(vk::WallPost)), SLOT(addPost(vk::WallPost)));
    connect(session, SIGNAL(postLikeAdded(int,int,int,bool)), SLOT(onPostLikeAdded(int,int,int,bool)));
    connect(session, SIGNAL(postLikeDeleted(int,int)), SLOT(onPostLikeDeleted(int,int)));

    m_contact = contact;
    m_session = session;
    emit contactChanged(contact);
}

QVariant WallModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    auto post = m_posts.at(row);
    switch (role) {
    case IdRole:
        return post.id();
    case FromRole:
        return qVariantFromValue(post.from());
    case ToRole:
        return qVariantFromValue(post.to());
    case BodyRole:
        return post.body();
    case DateRole:
        return post.date();
    case AttachmentsRole:
        return vk::Attachment::toVariantMap(post.attachments());
    case LikesRole:
        return post.likes();
    case RepostsRole:
        return post.reposts();
    case CommentsRole:
        return post.property("comments");
    default:
        break;
    }
    return QVariant::Invalid;
}

int WallModel::rowCount(const QModelIndex &) const
{
    return count();
}

int WallModel::count() const
{
    return m_posts.count();
}

void WallModel::getLastPosts(int count, vk::WallSession::Filter filter)
{
    if (m_session.isNull())
        qWarning("WallModel: contact is null! Please set a contact!");
    else {
    auto reply = m_session.data()->getPosts(filter, count, 0, false);
    connect(reply, SIGNAL(resultReady(QVariant)), SIGNAL(requestFinished()));
    }
}

void WallModel::addLike(int postId, bool retweet, const QString &message)
{
    if (m_session.isNull())
        qWarning("WallModel: contact is null! Please set a contact!");
    else {
        m_session.data()->addLike(postId, retweet, message);
    }
}

void WallModel::deleteLike(int postId)
{
    if (m_session.isNull())
        qWarning("WallModel: contact is null! Please set a contact!");
    else {
        m_session.data()->deleteLike(postId);
    }
}

void WallModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_posts.count());
    m_posts.clear();
    endRemoveRows();
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
}

int WallModel::findPost(int id)
{
    for (int i = 0; i != m_posts.count(); i++) {
        if (data(createIndex(i, 0), IdRole).toInt() == id)
            return i;
    }
    return -1;
}


static bool postLessThan(const vk::WallPost &a, const vk::WallPost &b)
{
    return a.id() < b.id();
}


void WallModel::addPost(const vk::WallPost &post)
{
    if (findPost(post.id()) != -1)
        return;

    auto it = qLowerBound(m_posts.end(), m_posts.begin(), post , postLessThan);
    auto last = it - m_posts.begin();
    beginInsertRows(QModelIndex(), last, last);
    m_posts.insert(it, post);
    endInsertRows();
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
}

void WallModel::replacePost(int i, const vk::WallPost &post)
{
    auto index = createIndex(i, 0);
    m_posts[i] = post;
    emit dataChanged(index, index);
}

void WallModel::onPostLikeAdded(int postId, int likes, int reposts, bool isRetweeted)
{
    int id = findPost(postId);
    if (id != -1) {
        vk::WallPost post = m_posts.at(id);

        auto map = post.likes();
        map.insert("count", likes);
        map.insert("user_likes", true);
        post.setLikes(map);

        map = post.reposts();
        map.insert("count", reposts);
        map.insert("user_reposted", isRetweeted);
        post.setReposts(map);

        replacePost(id, post);
    }
}

void WallModel::onPostLikeDeleted(int postId, int count)
{
    int id = findPost(postId);
    if (id != -1) {
        vk::WallPost post = m_posts.at(id);

        auto map = post.likes();
        map.insert("count", count);
        map.insert("user_likes", false);
        post.setLikes(map);

        map = post.reposts();
        map.insert("user_reposted", false);
        post.setReposts(map);

        replacePost(id, post);

    }
}

vk::Roster *WallModel::roster() const
{
    return m_contact.data()->client()->roster();
}
