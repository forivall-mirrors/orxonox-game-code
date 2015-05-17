/**
 * Copy-pasted from http://www.ogre3d.org/tikiwiki/BulletDebugDrawer&structure=Cookbook
 * This source code is released into the Public Domain.
 *
 * Modified by Fabian 'x3n' Landau
 */

#ifndef _OgreBulletUtils_H__
#define _OgreBulletUtils_H__

#include "tools/ToolsPrereqs.h"
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreColourValue.h>
#include <OgreMatrix3.h>
#include <OgreMatrix4.h>

namespace orxonox
{
    inline btVector3 vector3(const Ogre::Vector3& V)
    {
        return btVector3(V.x, V.y, V.z);
    }

    inline Ogre::Vector3 vector3(const btVector3& V)
    {
        return Ogre::Vector3(V.x(), V.y(), V.z());
    }

    inline btQuaternion quaternion(const Ogre::Quaternion& Q)
    {
        return btQuaternion(Q.x, Q.y, Q.z, Q.w);
    }

    inline Ogre::Quaternion quaternion(const btQuaternion& Q)
    {
        return Ogre::Quaternion(Q.w(), Q.x(), Q.y(), Q.z());
    }

    inline Ogre::ColourValue colour(const btVector3& color, btScalar alpha)
    {
        Ogre::ColourValue c(color.getX(), color.getY(), color.getZ(), alpha);
        c.saturate();
        return c;
    }

    inline Ogre::Matrix3 matrix3(const btMatrix3x3& matrix)
    {
        return Ogre::Matrix3(
                matrix[0][0], matrix[0][1], matrix[0][2],
                matrix[1][0], matrix[1][1], matrix[1][2],
                matrix[2][0], matrix[2][1], matrix[2][2]
            );
    }

    inline Ogre::Matrix4 matrix4(const btTransform& transform)
    {
        const btMatrix3x3& rotation = transform.getBasis();
        const btVector3& translation = transform.getOrigin();

        Ogre::Matrix4 matrix4 = Ogre::Matrix4(matrix3(rotation));
        matrix4.setTrans(vector3(translation));
        return matrix4;
    }
}

#endif /* _OgreBulletUtils_H__ */
