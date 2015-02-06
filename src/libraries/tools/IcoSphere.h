/**
 * Copy-pasted from
 *  - https://bitbucket.org/hasyimi/ogre-debug-drawing-utility/src
 *  - http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Debug+Drawing+Utility+Class
 *
 * This source code is released into the Public Domain.
 *
 * Modified by Fabian 'x3n' Landau
 */

/**
 * @file
 * @brief DebugDrawer is a utility to draw debug shapes (lines, triangles, spheres) with Ogre.
 * This utility is e.g. used by @ref BulletDebugDrawer to visualize collision shapes and other physical entities.
 */

#ifndef _IcoSphere_H__
#define _IcoSphere_H__

#include "tools/ToolsPrereqs.h"

#include <OgreSingleton.h>
#include <map>

namespace orxonox
{
    typedef std::pair<Ogre::Vector3, Ogre::ColourValue> VertexPair;

    class _ToolsExport IcoSphere
    {
        public:
            struct TriangleIndices
            {
                    int v1, v2, v3;

                    TriangleIndices(int _v1, int _v2, int _v3) :
                            v1(_v1), v2(_v2), v3(_v3)
                    {
                    }

                    bool operator <(const TriangleIndices &o) const
                    {
                        return v1 < o.v1 && v2 < o.v2 && v3 < o.v3;
                    }
            };

            struct LineIndices
            {
                    int v1, v2;

                    LineIndices(int _v1, int _v2) :
                            v1(_v1), v2(_v2)
                    {
                    }

                    bool operator ==(const LineIndices &o) const
                    {
                        return (v1 == o.v1 && v2 == o.v2) || (v1 == o.v2 && v2 == o.v1);
                    }
            };

            IcoSphere();
            ~IcoSphere();

            void create(int recursionLevel);
            void addToLineIndices(int baseIndex, std::list<int>* target) const;
            int addToVertices(std::list<VertexPair>* target, const Ogre::Vector3& position, const Ogre::ColourValue& colour, float scale) const;
            void addToTriangleIndices(int baseIndex, std::list<int>* target) const;

        private:
            int addVertex(const Ogre::Vector3& vertex);
            void addLineIndices(int index0, int index1);
            void addTriangleLines(int index0, int index1, int index2);
            int getMiddlePoint(int index0, int index1);
            void addFace(int index0, int index1, int index2);

            void removeLineIndices(int index0, int index1);

            std::vector<Ogre::Vector3> vertices;
            std::list<LineIndices> lineIndices;
            std::list<int> triangleIndices;
            std::list<TriangleIndices> faces;
            std::map<int64_t, int> middlePointIndexCache;
            int index;
    };
}

#endif /* _IcoSphere_H__ */
