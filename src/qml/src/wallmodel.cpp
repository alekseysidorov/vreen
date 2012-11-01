/****************************************************************************
**
** Vreen - vk.com API Qt bindings
**
** Copyright © 2012 Aleksey Sidorov <gorthauer87@ya.ru>
**
*****************************************************************************
**
** $VREEN_BEGIN_LICENSE$
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see http://www.gnu.org/licenses/.
** $VREEN_END_LICENSE$
**
****************************************************************************/
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

Vreen::Contact *WallModel::contact() const
{
    return m_contact.data();
}

void WallModel::setContact(Vreen::Contact *contact)
{
    if (!m_session.isNull()) {
        clear();
        m_session.data()->deleteLater();
    }
    if (!contact)
        return;
    auto session = new Vreen::WallSession(contact);
    connect(session, SIGNAL(postAdded(Vreen::WallPost)), SLOT(addPost(Vreen::WallPost)));
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
        return Vreen::Attachment::toVariantMap(post.attachments());
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

void WallModel::getLastPosts(int count, Vreen::WallSession::Filter filter)
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


static bool postLessThan(const Vreen::WallPost &a, const Vreen::WallPost &b)
{
    return a.id() < b.id();
}


void WallModel::addPost(const Vreen::WallPost &post)
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

void WallModel::replacePost(int i, const Vreen::WallPost &post)
{
    auto index = createIndex(i, 0);
    m_posts[i] = post;
    emit dataChanged(index, index);
}

void WallModel::onPostLikeAdded(int postId, int likes, int reposts, bool isRetweeted)
{
    int id = findPost(postId);
    if (id != -1) {
        Vreen::WallPost post = m_posts.at(id);

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
        Vreen::WallPost post = m_posts.at(id);

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

Vreen::Roster *WallModel::roster() const
{
    return m_contact.data()->client()->roster();
}

