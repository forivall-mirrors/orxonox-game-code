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

#ifndef _DebugDrawer_H__
#define _DebugDrawer_H__

#include "tools/ToolsPrereqs.h"

#include <map>

#include "IcoSphere.h"

namespace orxonox
{
    class _ToolsExport DebugDrawer
    {
        public:
            DebugDrawer(Ogre::SceneManager *_sceneManager, float _fillAlpha);
            ~DebugDrawer();

            void build();

            void drawLine(const Ogre::Vector3& start, const Ogre::Vector3& end, const Ogre::ColourValue& colour);
            void drawCircle(const Ogre::Vector3& centre, float radius, int segmentsCount, const Ogre::ColourValue& colour, bool isFilled = false);
            void drawCylinder(const Ogre::Vector3& centre, float radius, int segmentsCount, float height, const Ogre::ColourValue& colour, bool isFilled = false);
            void drawQuad(const Ogre::Vector3* vertices, const Ogre::ColourValue& colour, bool isFilled = false);
            void drawCuboid(const Ogre::Vector3* vertices, const Ogre::ColourValue& colour, bool isFilled = false);
            void drawSphere(const Ogre::Vector3& centre, float radius, const Ogre::ColourValue& colour, bool isFilled = false);
            void drawTetrahedron(const Ogre::Vector3& centre, float scale, const Ogre::ColourValue& colour, bool isFilled = false);

            bool getEnabled()
            {
                return isEnabled;
            }
            void setEnabled(bool _isEnabled)
            {
                isEnabled = _isEnabled;
            }
            void switchEnabled()
            {
                isEnabled = !isEnabled;
            }

            void clear();

        private:
            const IcoSphere& getIcoSphere(float radius) const;

            Ogre::SceneManager* sceneManager;
            float fillAlpha;
            Ogre::ManualObject* manualObject;
            IcoSphere icoSphere0;
            IcoSphere icoSphere1;
            IcoSphere icoSphere2;
            IcoSphere icoSphere3;
            IcoSphere icoSphere4;

            bool isEnabled;

            std::list<VertexPair> lineVertices, triangleVertices;
            std::list<int> lineIndices, triangleIndices;

            int linesIndex, trianglesIndex;

            void initialise();
            void shutdown();

            void buildLine(const Ogre::Vector3& start, const Ogre::Vector3& end, const Ogre::ColourValue& colour, float alpha = 1.0f);
            void buildQuad(const Ogre::Vector3* vertices, const Ogre::ColourValue& colour, float alpha = 1.0f);
            void buildFilledQuad(const Ogre::Vector3* vertices, const Ogre::ColourValue& colour, float alpha = 1.0f);
            void buildFilledTriangle(const Ogre::Vector3* vertices, const Ogre::ColourValue& colour, float alpha = 1.0f);
            void buildCuboid(const Ogre::Vector3* vertices, const Ogre::ColourValue& colour, float alpha = 1.0f);
            void buildFilledCuboid(const Ogre::Vector3* vertices, const Ogre::ColourValue& colour, float alpha = 1.0f);

            void buildCircle(const Ogre::Vector3& centre, float radius, int segmentsCount, const Ogre::ColourValue& colour, float alpha = 1.0f);
            void buildFilledCircle(const Ogre::Vector3& centre, float radius, int segmentsCount, const Ogre::ColourValue& colour, float alpha = 1.0f);

            void buildCylinder(const Ogre::Vector3& centre, float radius, int segmentsCount, float height, const Ogre::ColourValue& colour, float alpha = 1.0f);
            void buildFilledCylinder(const Ogre::Vector3& centre, float radius, int segmentsCount, float height, const Ogre::ColourValue& colour, float alpha = 1.0f);

            void buildTetrahedron(const Ogre::Vector3& centre, float scale, const Ogre::ColourValue& colour, float alpha = 1.0f);
            void buildFilledTetrahedron(const Ogre::Vector3& centre, float scale, const Ogre::ColourValue& colour, float alpha = 1.0f);

            int addLineVertex(const Ogre::Vector3& vertex, const Ogre::ColourValue& colour);
            void addLineIndices(int index1, int index2);

            int addTriangleVertex(const Ogre::Vector3& vertex, const Ogre::ColourValue& colour);
            void addTriangleIndices(int index1, int index2, int index3);

            void addQuadIndices(int index1, int index2, int index3, int index4);
    };
}

#endif /* _DebugDrawer_H__ */
