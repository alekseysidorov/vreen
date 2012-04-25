#include "photomanager.h"
#include "client.h"

namespace vk {

PhotoManager::PhotoManager(Client *client) :
    QObject(client)
{
}

} // namespace vk
