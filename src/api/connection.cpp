#include "connection.h"

namespace vk {

Connection::Connection(QObject *parent) :
    QNetworkAccessManager(parent)
{
}

} // namespace vk
