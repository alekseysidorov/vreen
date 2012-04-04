#include "utils.h"
#include <QStringBuilder>

namespace vk {

QString join(IdList ids)
{
    QString result;
    if (ids.isEmpty())
        return result;

    result = QString::number(ids.takeFirst());
    foreach (auto id, ids)
        result += QLatin1Literal(",") % QString::number(id);
    return result;
}

} //namespace vk
