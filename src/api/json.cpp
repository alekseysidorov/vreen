/****************************************************************************
**
** Vreen - vk.com API Qt bindings
**
** Copyright © 2012-2015 Aleksey Sidorov <gorthauer87@ya.ru>
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
#include "json.h"
#include <QJsonDocument>

namespace Vreen {

namespace JSON {

/*!
 * \brief Parse JSON data to QVariant
 * \param String with JSON data
 * \return Result of parsing, QVariant::Null if there was an error
 */
QVariant parse(const QByteArray &data)
{
    QJsonParseError error;

    auto document = QJsonDocument::fromJson(data, &error);
    return document.toVariant();
}

/*!
 * \brief Generate JSON string from QVariant
 * \param data QVariant with data
 * \param indent Identation of new lines
 * \return JSON string with data
*/
QByteArray generate(const QVariant &data, int indent)
{
    Q_UNUSED(indent);

    auto document = QJsonDocument::fromVariant(data);
    return document.toJson(QJsonDocument::Indented);
}

} //namespace JSON

} // namespace Vreen

