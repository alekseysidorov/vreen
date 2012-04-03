#include "json.h"
#include <k8json.h>

namespace vk {

namespace JSON {

/*!
 * \brief Parse JSON data to QVariant
 * \param String with JSON data
 * \return Result of parsing, QVariant::Null if there was an error
 */
QVariant parse(const QByteArray &data)
{
    QVariant res;
    int len = data.size();
    K8JSON::parseRecord(res, reinterpret_cast<const uchar *>(data.constData()), &len);
    return res;
}

/*!
 * \brief Generate JSON string from QVariant
 * \param data QVariant with data
 * \param indent Identation of new lines
 * \return JSON string with data
*/
QByteArray generate(const QVariant &data, int indent)
{
    QByteArray res;
    K8JSON::generate(res, data, indent);
    return res;
}

} //namespace JSON

} // namespace vk
