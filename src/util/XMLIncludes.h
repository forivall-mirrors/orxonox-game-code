/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      ...
 *   Co-authors:
 *      ...
 *
 */

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

class TiXmlString;
class TiXmlOutStream;
class TiXmlNode;
class TiXmlHandle;
class TiXmlDocument;
class TiXmlElement;
class TiXmlComment;
class TiXmlUnknown;
class TiXmlAttribute;
class TiXmlText;
class TiXmlDeclaration;
class TiXmlParsingData;

namespace ticpp
{
    class Document;
    class Element;
    class Declaration;
    class StylesheetReference;
    class Text;
    class Comment;
    class Attribute;
}

namespace orxonox
{
    typedef ticpp::Document Document;
    typedef ticpp::Element Element;
    typedef ticpp::Declaration Declaration;
    typedef ticpp::StylesheetReference StylesheetReference;
    typedef ticpp::Text Text;
    typedef ticpp::Comment Comment;
    typedef ticpp::Attribute Attribute;
}
