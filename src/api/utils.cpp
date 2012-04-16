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

QString toCamelCase(QString string)
{
    int from = 0;
    while ((from = string.indexOf("_", from)) != -1) {
        auto index = from + 1;
        string.remove(from, 1);
        auto letter = string.at(index);
        string.replace(index, 1, letter.toUpper());
    }
    return string;
}

} //namespace vk
