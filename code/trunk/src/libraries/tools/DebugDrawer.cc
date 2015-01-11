/**
 * Copy-pasted from
 *  - https://bitbucket.org/hasyimi/ogre-debug-drawing-utility/src
 *  - http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Debug+Drawing+Utility+Class
 *
 * This source code is released into the Public Domain.
 *
 * Modified by Fabian 'x3n' Landau
 */

#include "DebugDrawer.h"

#include <OgreSceneManager.h>
#include <OgreRenderQueue.h>
#include <OgreManualObject.h>
#include <OgreAxisAlignedBox.h>

#include "util/Output.h"

#define DEFAULT_ICOSPHERE_RECURSION_LEVEL 1

namespace orxonox
{
    DebugDrawer::DebugDrawer(Ogre::SceneManager *_sceneManager, float _fillAlpha) :
            sceneManager(_sceneManager), fillAlpha(_fillAlpha), manualObject(0), linesIndex(0), trianglesIndex(0)
    {
        initialise();
    }

    DebugDrawer::~DebugDrawer()
    {
        shutdown();
    }

    void DebugDrawer::initialise()
    {
        manualObject = sceneManager->createManualObject("debug_object");
        sceneManager->getRootSceneNode()->createChildSceneNode("debug_object")->attachObject(manualObject);
        manualObject->setDynamic(true);

        icoSphere0.create(0);
        icoSphere1.create(1);
        icoSphere2.create(2);
        icoSphere3.create(3);
        icoSphere4.create(4);

        manualObject->begin("debug_draw", Ogre::RenderOperation::OT_LINE_LIST);
        manualObject->position(Ogre::Vector3::ZERO);
        manualObject->colour(Ogre::ColourValue::ZERO);
        manualObject->index(0);
        manualObject->end();
        manualObject->begin("debug_draw", Ogre::RenderOperation::OT_TRIANGLE_LIST);
        manualObject->position(Ogre::Vector3::ZERO);
        manualObject->colour(Ogre::ColourValue::ZERO);
        manualObject->index(0);
        manualObject->end();

        linesIndex = trianglesIndex = 0;
    }

    void DebugDrawer::shutdown()
    {
        sceneManager->destroySceneNode("debug_object");
        sceneManager->destroyManualObject(manualObject);
    }

    void DebugDrawer::buildLine(const Ogre::Vector3& start, const Ogre::Vector3& end, const Ogre::ColourValue& colour, float alpha)
    {
        int i = addLineVertex(start, Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
        addLineVertex(end, Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));

        addLineIndices(i, i + 1);
    }

    void DebugDrawer::buildQuad(const Ogre::Vector3* vertices, const Ogre::ColourValue& colour, float alpha)
    {
        int index = addLineVertex(vertices[0], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
        addLineVertex(vertices[1], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
        addLineVertex(vertices[2], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
        addLineVertex(vertices[3], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));

        for (int i = 0; i < 4; ++i)
            addLineIndices(index + i, index + ((i + 1) % 4));
    }

    void DebugDrawer::buildCircle(const Ogre::Matrix4& transform, float radius, int segmentsCount, const Ogre::ColourValue& colour, float alpha)
    {
        int index = linesIndex;
        float increment = 2 * Ogre::Math::PI / segmentsCount;
        float angle = 0.0f;

        for (int i = 0; i < segmentsCount; i++)
        {
            addLineVertex(transform * Ogre::Vector3(radius * Ogre::Math::Cos(angle), 0, radius * Ogre::Math::Sin(angle)),
                    Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
            angle += increment;
        }

        for (int i = 0; i < segmentsCount; i++)
            addLineIndices(index + i, i + 1 < segmentsCount ? index + i + 1 : index);
    }

    void DebugDrawer::buildFilledCircle(const Ogre::Matrix4& transform, float radius, int segmentsCount, const Ogre::ColourValue& colour, bool up, float alpha)
    {
        int index = trianglesIndex;
        float increment = 2 * Ogre::Math::PI / segmentsCount;
        float angle = 0.0f;

        for (int i = 0; i < segmentsCount; i++)
        {
            addTriangleVertex(transform * Ogre::Vector3(radius * Ogre::Math::Cos(angle), 0, radius * Ogre::Math::Sin(angle)),
                    Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
            angle += increment;
        }

        addTriangleVertex(transform.getTrans(), Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));

        for (int i = 0; i < segmentsCount; i++)
        {
            if (up)
                addTriangleIndices(i + 1 < segmentsCount ? index + i + 1 : index, index + i, index + segmentsCount);
            else
                addTriangleIndices(index + i, i + 1 < segmentsCount ? index + i + 1 : index, index + segmentsCount);
        }
    }

    void DebugDrawer::buildCylinder(const Ogre::Vector3& centre, const Ogre::Quaternion& rotation, float radius, int segmentsCount, float height, const Ogre::ColourValue& colour, float alpha)
    {
        int index = linesIndex;

        Ogre::Matrix4 transform(rotation);
        transform.setTrans(centre + rotation * Ogre::Vector3(0, height / 2, 0));
        this->buildCircle(transform, radius, segmentsCount, colour, alpha);
        transform.setTrans(centre + rotation * Ogre::Vector3(0, -height / 2, 0));
        this->buildCircle(transform, radius, segmentsCount, colour, alpha);

        for (int i = 0; i < segmentsCount; i++)
            addLineIndices(index + i, segmentsCount + index + i);
    }

    void DebugDrawer::buildFilledCylinder(const Ogre::Vector3& centre, const Ogre::Quaternion& rotation, float radius, int segmentsCount, float height, const Ogre::ColourValue& colour,
            float alpha)
    {
        int index = trianglesIndex;

        Ogre::Matrix4 transform(rotation);
        transform.setTrans(centre + rotation * Ogre::Vector3(0, height / 2, 0));
        this->buildCircle(transform, radius, segmentsCount, colour, alpha);
        this->buildFilledCircle(transform, radius, segmentsCount, colour, true, alpha);

        transform.setTrans(centre + rotation * Ogre::Vector3(0, -height / 2, 0));
        this->buildCircle(transform, radius, segmentsCount, colour, alpha);
        this->buildFilledCircle(transform, radius, segmentsCount, colour, false, alpha);

        for (int i = 0; i < segmentsCount; i++)
        {
            addQuadIndices(index + i, i + 1 < segmentsCount ? index + i + 1 : index,
                    i + 1 < segmentsCount ? (segmentsCount + 1) + index + i + 1 : (segmentsCount + 1) + index, (segmentsCount + 1) + index + i);
        }
    }

    void DebugDrawer::buildCone(const Ogre::Vector3& centre, const Ogre::Quaternion& rotation, float radius, int segmentsCount, float height, const Ogre::ColourValue& colour, float alpha)
    {
        int index = linesIndex;

        Ogre::Matrix4 transform(rotation);
        transform.setTrans(centre + rotation * Ogre::Vector3(0, -height / 2, 0));
        this->buildCircle(transform, radius, segmentsCount, colour, alpha);

        addLineVertex(centre + rotation * Ogre::Vector3(0, height / 2, 0), Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));

        for (int i = 0; i < segmentsCount; i++)
            addLineIndices(index + i, index + segmentsCount);
    }

    void DebugDrawer::buildFilledCone(const Ogre::Vector3& centre, const Ogre::Quaternion& rotation, float radius, int segmentsCount, float height, const Ogre::ColourValue& colour,
            float alpha)
    {
        int index = trianglesIndex;

        Ogre::Matrix4 transform(rotation);
        transform.setTrans(centre + rotation * Ogre::Vector3(0, -height / 2, 0));
        this->buildCircle(transform, radius, segmentsCount, colour, alpha);
        this->buildFilledCircle(transform, radius, segmentsCount, colour, false, alpha);

        addTriangleVertex(centre + rotation * Ogre::Vector3(0, height / 2, 0), Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));

        for (int i = 0; i < segmentsCount; i++)
            addTriangleIndices(index + i, index + segmentsCount + 1, i + 1 < segmentsCount ? index + i + 1 : index);
    }

    void DebugDrawer::buildCuboid(const Ogre::Vector3* vertices, const Ogre::ColourValue& colour, float alpha)
    {
        int index = addLineVertex(vertices[0], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
        for (int i = 1; i < 8; ++i)
            addLineVertex(vertices[i], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));

        for (int i = 0; i < 4; ++i)
            addLineIndices(index + i, index + ((i + 1) % 4));
        for (int i = 4; i < 8; ++i)
            addLineIndices(index + i, i == 7 ? index + 4 : index + i + 1);
        addLineIndices(index + 1, index + 5);
        addLineIndices(index + 2, index + 4);
        addLineIndices(index, index + 6);
        addLineIndices(index + 3, index + 7);
    }

    void DebugDrawer::buildFilledCuboid(const Ogre::Vector3* vertices, const Ogre::ColourValue& colour, float alpha)
    {
        int index = addTriangleVertex(vertices[0], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
        for (int i = 1; i < 8; ++i)
            addTriangleVertex(vertices[i], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));

        addQuadIndices(index, index + 1, index + 2, index + 3);
        addQuadIndices(index + 4, index + 5, index + 6, index + 7);

        addQuadIndices(index + 1, index + 5, index + 4, index + 2);
        addQuadIndices(index, index + 3, index + 7, index + 6);

        addQuadIndices(index + 1, index, index + 6, index + 5);
        addQuadIndices(index + 4, index + 7, index + 3, index + 2);
    }

    void DebugDrawer::buildFilledQuad(const Ogre::Vector3* vertices, const Ogre::ColourValue& colour, float alpha)
    {
        int index = addTriangleVertex(vertices[0], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
        addTriangleVertex(vertices[1], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
        addTriangleVertex(vertices[2], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
        addTriangleVertex(vertices[3], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));

        addQuadIndices(index, index + 1, index + 2, index + 3);
    }

    void DebugDrawer::buildFilledTriangle(const Ogre::Vector3* vertices, const Ogre::ColourValue& colour, float alpha)
    {
        int index = addTriangleVertex(vertices[0], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
        addTriangleVertex(vertices[1], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
        addTriangleVertex(vertices[2], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));

        addTriangleIndices(index, index + 1, index + 2);
    }

    void DebugDrawer::buildTetrahedron(const Ogre::Vector3& centre, float scale, const Ogre::ColourValue& colour, float alpha)
    {
        int index = linesIndex;

        // Distance from the centre
        float bottomDistance = scale * 0.2f;
        float topDistance = scale * 0.62f;
        float frontDistance = scale * 0.289f;
        float backDistance = scale * 0.577f;
        float leftRightDistance = scale * 0.5f;

        addLineVertex(Ogre::Vector3(centre.x, centre.y + topDistance, centre.z), Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
        addLineVertex(Ogre::Vector3(centre.x, centre.y - bottomDistance, centre.z + frontDistance), Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
        addLineVertex(Ogre::Vector3(centre.x + leftRightDistance, centre.y - bottomDistance, centre.z - backDistance),
                Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
        addLineVertex(Ogre::Vector3(centre.x - leftRightDistance, centre.y - bottomDistance, centre.z - backDistance),
                Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));

        addLineIndices(index, index + 1);
        addLineIndices(index, index + 2);
        addLineIndices(index, index + 3);

        addLineIndices(index + 1, index + 2);
        addLineIndices(index + 2, index + 3);
        addLineIndices(index + 3, index + 1);
    }

    void DebugDrawer::buildFilledTetrahedron(const Ogre::Vector3& centre, float scale, const Ogre::ColourValue& colour, float alpha)
    {
        int index = trianglesIndex;

        // Distance from the centre
        float bottomDistance = scale * 0.2f;
        float topDistance = scale * 0.62f;
        float frontDistance = scale * 0.289f;
        float backDistance = scale * 0.577f;
        float leftRightDistance = scale * 0.5f;

        addTriangleVertex(Ogre::Vector3(centre.x, centre.y + topDistance, centre.z), Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
        addTriangleVertex(Ogre::Vector3(centre.x, centre.y - bottomDistance, centre.z + frontDistance), Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
        addTriangleVertex(Ogre::Vector3(centre.x + leftRightDistance, centre.y - bottomDistance, centre.z - backDistance),
                Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
        addTriangleVertex(Ogre::Vector3(centre.x - leftRightDistance, centre.y - bottomDistance, centre.z - backDistance),
                Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));

        addTriangleIndices(index, index + 1, index + 2);
        addTriangleIndices(index, index + 2, index + 3);
        addTriangleIndices(index, index + 3, index + 1);

        addTriangleIndices(index + 1, index + 3, index + 2);
    }

    void DebugDrawer::drawLine(const Ogre::Vector3& start, const Ogre::Vector3& end, const Ogre::ColourValue& colour)
    {
        buildLine(start, end, colour);
    }

    void DebugDrawer::drawCircle(const Ogre::Vector3& centre, const Ogre::Quaternion& rotation, float radius, const Ogre::ColourValue& colour, bool isFilled)
    {
        int segmentsCount = std::min(100.0, radius / 2.5);

        Ogre::Matrix4 transform(rotation);
        transform.setTrans(centre);

        buildCircle(transform, radius, segmentsCount, colour);
        if (isFilled)
            buildFilledCircle(transform, radius, segmentsCount, colour, fillAlpha);
    }

    void DebugDrawer::drawCylinder(const Ogre::Vector3& centre, const Ogre::Quaternion& rotation, float radius, float height, const Ogre::ColourValue& colour, bool isFilled)
    {
        int segmentsCount = std::min(100.0, radius / 2.5);

        if (isFilled)
            buildFilledCylinder(centre, rotation, radius, segmentsCount, height, colour, fillAlpha);
        else
            buildCylinder(centre, rotation, radius, segmentsCount, height, colour);
    }

    void DebugDrawer::drawCone(const Ogre::Vector3& centre, const Ogre::Quaternion& rotation, float radius, float height, const Ogre::ColourValue& colour, bool isFilled)
    {
        int segmentsCount = std::min(100.0, radius / 2.5);

        if (isFilled)
            buildFilledCone(centre, rotation, radius, segmentsCount, height, colour, fillAlpha);
        else
            buildCone(centre, rotation, radius, segmentsCount, height, colour);
    }

    void DebugDrawer::drawQuad(const Ogre::Vector3* vertices, const Ogre::ColourValue& colour, bool isFilled)
    {
        buildQuad(vertices, colour);
        if (isFilled)
            buildFilledQuad(vertices, colour, fillAlpha);
    }

    void DebugDrawer::drawCuboid(const Ogre::Vector3* vertices, const Ogre::ColourValue& colour, bool isFilled)
    {
        buildCuboid(vertices, colour);
        if (isFilled)
            buildFilledCuboid(vertices, colour, fillAlpha);
    }

    void DebugDrawer::drawSphere(const Ogre::Vector3& centre, const Ogre::Quaternion& rotation, float radius, const Ogre::ColourValue& colour, bool isFilled)
    {
        const IcoSphere& sphere = this->getIcoSphere(radius);

        if (isFilled)
        {
            this->drawCircle(centre, rotation * Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3(1, 0, 0)), radius, colour, false);
            this->drawCircle(centre, rotation * Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3(0, 1, 0)), radius, colour, false);
            this->drawCircle(centre, rotation * Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3(0, 0, 1)), radius, colour, false);

            int baseIndex = trianglesIndex;
            trianglesIndex += sphere.addToVertices(&triangleVertices, centre, Ogre::ColourValue(colour.r, colour.g, colour.b, fillAlpha), radius);
            sphere.addToTriangleIndices(baseIndex, &triangleIndices);
        }
        else
        {
            int baseIndex = linesIndex;
            linesIndex += sphere.addToVertices(&lineVertices, centre, colour, radius);
            sphere.addToLineIndices(baseIndex, &lineIndices);
        }
    }

    const IcoSphere& DebugDrawer::getIcoSphere(float radius) const
    {
        if (radius < 50)
            return this->icoSphere0;
        else if (radius < 500)
            return this->icoSphere1;
        else if (radius < 5000)
            return this->icoSphere2;
        else if (radius < 50000)
            return this->icoSphere3;
        else
            return this->icoSphere4;
    }

    void DebugDrawer::drawTetrahedron(const Ogre::Vector3& centre, float scale, const Ogre::ColourValue& colour, bool isFilled)
    {
        buildTetrahedron(centre, scale, colour);
        if (isFilled)
            buildFilledTetrahedron(centre, scale, colour, fillAlpha);
    }

    void DebugDrawer::build()
    {
        manualObject->beginUpdate(0);
        if (lineVertices.size() > 0 && isEnabled)
        {
            manualObject->estimateVertexCount(lineVertices.size());
            manualObject->estimateIndexCount(lineIndices.size());
            for (std::list<VertexPair>::iterator i = lineVertices.begin(); i != lineVertices.end(); i++)
            {
                manualObject->position(i->first);
                manualObject->colour(i->second);
            }
            for (std::list<int>::iterator i = lineIndices.begin(); i != lineIndices.end(); i++)
                manualObject->index(*i);
        }
        manualObject->end();

        manualObject->beginUpdate(1);
        if (triangleVertices.size() > 0 && isEnabled)
        {
            manualObject->estimateVertexCount(triangleVertices.size());
            manualObject->estimateIndexCount(triangleIndices.size());
            for (std::list<VertexPair>::iterator i = triangleVertices.begin(); i != triangleVertices.end(); i++)
            {
                manualObject->position(i->first);
                manualObject->colour(i->second.r, i->second.g, i->second.b, fillAlpha);
            }
            for (std::list<int>::iterator i = triangleIndices.begin(); i != triangleIndices.end(); i++)
                manualObject->index(*i);
        }
        manualObject->end();
    }

    void DebugDrawer::clear()
    {
        lineVertices.clear();
        triangleVertices.clear();
        lineIndices.clear();
        triangleIndices.clear();
        linesIndex = trianglesIndex = 0;
    }

    int DebugDrawer::addLineVertex(const Ogre::Vector3& vertex, const Ogre::ColourValue& colour)
    {
        lineVertices.push_back(VertexPair(vertex, colour));
        return linesIndex++;
    }

    void DebugDrawer::addLineIndices(int index1, int index2)
    {
        lineIndices.push_back(index1);
        lineIndices.push_back(index2);
    }

    int DebugDrawer::addTriangleVertex(const Ogre::Vector3& vertex, const Ogre::ColourValue& colour)
    {
        triangleVertices.push_back(VertexPair(vertex, colour));
        return trianglesIndex++;
    }

    void DebugDrawer::addTriangleIndices(int index1, int index2, int index3)
    {
        triangleIndices.push_back(index1);
        triangleIndices.push_back(index2);
        triangleIndices.push_back(index3);
    }

    void DebugDrawer::addQuadIndices(int index1, int index2, int index3, int index4)
    {
        triangleIndices.push_back(index1);
        triangleIndices.push_back(index2);
        triangleIndices.push_back(index3);

        triangleIndices.push_back(index1);
        triangleIndices.push_back(index3);
        triangleIndices.push_back(index4);
    }
}
