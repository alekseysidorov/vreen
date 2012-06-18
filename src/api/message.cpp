#include "message.h"
#include "contact.h"
#include "client.h"
#include "roster.h"
#include <QDateTime>
#include "dynamicpropertydata_p.h"
#include <QDebug>

namespace vk {

class MessageData : public DynamicPropertyData
{
public:
	MessageData(Client *client) :
		client(client),
		id(0),
		from(client->me()),
		to(client->me()),
		chatId(0),
		userCount(0),
		latitude(-1),
		longitude(-1)
	{}
	MessageData(const MessageData &o) :
		DynamicPropertyData(o),
		client(o.client),
		id(o.id),
		from(o.from),
		to(o.to),
		date(o.date),
		flags(o.flags),
		subject(o.subject),
		body(o.body),
		forwardMsgIds(o.forwardMsgIds),
		chatId(o.chatId),
		chatActive(o.chatActive),
		userCount(o.userCount),
		admin(o.admin),
		latitude(o.latitude),
		longitude(o.longitude)
	{}
	~MessageData() {}

	Client *client;
	int id;
	QWeakPointer<Contact> from;
	QWeakPointer<Contact> to;
	QDateTime date;
	Message::Flags flags;
	QString subject;
	QString body;
	QList<int> forwardMsgIds;
	int chatId;
	QList<int> chatActive;
	int userCount;
	QWeakPointer<Contact> admin;
	qreal latitude;
	qreal longitude;
	Attachment::Hash attachmentHash;

	void fill(const QVariantMap &data)
	{
		id = data.value("mid").toInt();

		int clientId = data.value("from_id").toInt();
		if (clientId) {
			auto contact = client->roster()->contact(clientId);
			bool isIncoming = contact == client->me();
			setFlag(Message::FlagOutbox, !isIncoming);
			if (isIncoming) {
				from = client->me();
				to.clear();

			} else {
				from = contact;
				to = client->me();
			}
		} else {
			setFlag(Message::FlagOutbox, data.value("out").toBool());
			clientId = data.value("uid").toInt();
			auto contact = client->roster()->contact(clientId);
			if (!flags.testFlag(Message::FlagOutbox)) {
				from = contact;
				to = client->me();
			} else {
				to = contact;
				from = client->me();
			}
		}

		date = QDateTime::fromTime_t(data.value("date").toInt());
		setFlag(Message::FlagUnread, !data.value("read_state").toBool());
		subject = data.value("title").toString();
		body = data.value("body").toString();
		attachmentHash = Attachment::toHash(Attachment::fromVariantList(data.value("attachments").toList()));
		//TODO forward messages
		//TODO groupchats
	}
	void setFlag(Message::Flag flag, bool set = true)
	{
		if (set)
			flags |= flag;
		else
			flags &= ~flag;
	}
};


/*!
 * \brief The Message class
 * Api reference: \link http://vk.com/developers.php?oid=-1&p=Формат_описания_личных_сообщений */

Message::Message(Client *client) :
	d(new MessageData(client))
{
}

Message::Message(const QVariantMap &data, Client *client) :
	d(new MessageData(client))
{
	d->fill(data);
}

Message::Message(const Message &other) : d(other.d)
{
}

Message &Message::operator =(const Message &other)
{
	if (this != &other)
		d.operator=(other.d);
	return *this;
}

bool Message::operator ==(const Message &other)
{
	return id() == other.id();
}

Message::~Message()
{
}

int Message::id() const
{
	return d->id;
}

void Message::setId(int id)
{
	d->id = id;
}

Client *Message::client() const
{
	return d->client;
}

QDateTime Message::date() const
{
	return d->date;
}

void Message::setDate(const QDateTime &date)
{
	d->date = date;
}

int Message::fromId() const
{
	return d->from.isNull() ? -1 : d->from.data()->id();
}

int Message::toId() const
{
	return d->to.isNull() ? -1 : d->to.data()->id();
}

Contact *Message::from() const
{
	return d->from.data();
}

void Message::setFrom(Contact *contact)
{
	d->from = contact;
}

Contact *Message::to() const
{
	return d->to.data();
}

void Message::setTo(Contact *to)
{
	d->to = to;
}

QString Message::subject() const
{
	return d->subject;
}

void Message::setSubject(const QString &title)
{
	d->subject = title;
}

QString Message::body() const
{
	return d->body;
}

void Message::setBody(const QString &body)
{
	d->body = body;
}

bool Message::isUnread() const
{
	return testFlag(FlagUnread);
}

void Message::setUnread(bool set)
{
	setFlag(FlagUnread, set);
}

bool Message::isIncoming() const
{
	return !testFlag(FlagOutbox);
}

void Message::setIncoming(bool set)
{
	setFlag(FlagOutbox, !set);
}

void Message::setFlags(Message::Flags flags)
{
	d->flags = flags;
}

Message::Flags Message::flags() const
{
	return d->flags;
}

void Message::setFlag(Flag flag, bool set)
{
	d->setFlag(flag, set);
}

bool Message::testFlag(Flag flag) const
{
	return d->flags.testFlag(flag);
}

Attachment::Hash Message::attachments() const
{
	return d->attachmentHash;
}

Attachment::List Message::attachments(Attachment::Type type) const
{
	return d->attachmentHash.values(type);
}

void Message::setAttachments(const Attachment::List &attachmentList)
{
	d->attachmentHash = Attachment::toHash(attachmentList);
}

} // namespace vk
