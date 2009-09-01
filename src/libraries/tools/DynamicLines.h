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
 *      Baxissimo, Emmeran, DWORD, EtherDrive (OGRE Wiki)
 *   Co-authors:
 *      ...
 *
 */

#ifndef _DynamicLines_H__
#define _DynamicLines_H__

#include "tools/ToolsPrereqs.h"

#include <vector>
#include "DynamicRenderable.h"

namespace Ogre
{
    class _ToolsExport DynamicLines : public DynamicRenderable
    {
        typedef RenderOperation::OperationType OperationType;

    public:
        /// Constructor - see setOperationType() for description of argument.
        DynamicLines(OperationType opType = RenderOperation::OT_LINE_STRIP);
        virtual ~DynamicLines();

        /// Add a point to the point list
        void addPoint(const Vector3 &p);
        /// Add a point to the point list
        void addPoint(Real x, Real y, Real z);

        /// Change the location of an existing point in the point list
        void setPoint(unsigned short index, const Vector3 &value);

        /// Return the location of an existing point in the point list
        const Vector3& getPoint(unsigned short index) const;

        /// Return the total number of points in the point list
        unsigned short getNumPoints(void) const;

        /// Remove all points from the point list
        void clear();

        /// Call this to update the hardware buffer after making changes.
        void update();

        /**
        @brief
            Set the type of operation to draw with.
        @param opType
            Can be one of
                - RenderOperation::OT_LINE_STRIP
                - RenderOperation::OT_LINE_LIST
                - RenderOperation::OT_POINT_LIST
                - RenderOperation::OT_TRIANGLE_LIST
                - RenderOperation::OT_TRIANGLE_STRIP
                - RenderOperation::OT_TRIANGLE_FAN
            The default is OT_LINE_STRIP.
        */
        void setOperationType(OperationType opType);
        OperationType getOperationType() const;

    protected:
        /// Implementation DynamicRenderable, creates a simple vertex-only decl
        virtual void createVertexDeclaration();
        /// Implementation DynamicRenderable, pushes point list out to hardware memory
        virtual void fillHardwareBuffers();

    private:
        std::vector<Vector3> mPoints;
        bool mDirty;
    };
}

#endif /* _DynamicLines_H__ */
