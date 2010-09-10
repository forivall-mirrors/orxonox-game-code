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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

/**
    @file
    @brief Implementation of several math-functions.
*/

#include "Math.h"

#include <OgrePlane.h>

#include "MathConvert.h"
#include "SubString.h"
// Do not remove this include, it avoids linker errors.
#include "mbool.h"

namespace orxonox
{
#if OGRE_VERSION < 0x010603
    /**
        @brief Function for writing a Radian to a stream.
    */
    std::ostream& operator<<(std::ostream& out, const orxonox::Radian& radian)
    {
        out << radian.valueRadians();
        return out;
    }

    /**
        @brief Function for writing a Degree to a stream.
    */
    std::ostream& operator<<(std::ostream& out, const orxonox::Degree& degree)
    {
        out << degree.valueDegrees();
        return out;
    }
#endif

    /**
        @brief Function for reading a Radian from a stream.
    */
    std::istream& operator>>(std::istream& in, orxonox::Radian& radian)
    {
        float temp;
        in >> temp;
        radian = temp;
        return in;
    }

    /**
        @brief Function for reading a Degree from a stream.
    */
    std::istream& operator>>(std::istream& in, orxonox::Degree& degree)
    {
        float temp;
        in >> temp;
        degree = temp;
        return in;
    }


    /**
        @brief Gets the angle between my viewing direction and the direction to the position of the other object.
        @param myposition My position
        @param mydirection My viewing direction
        @param otherposition The position of the other object
        @return The angle in radian

        Examples:
         - If the other object is exactly in front of me, the function returns 0.
         - If the other object is exactly behind me, the function returns pi.
         - If the other object is exactly right/left to me (or above/below), the function returns pi/2.
    */
    float getAngle(const orxonox::Vector3& myposition, const orxonox::Vector3& mydirection, const orxonox::Vector3& otherposition)
    {
        orxonox::Vector3 distance = otherposition - myposition;
        float distancelength = distance.length();
        if (distancelength == 0)
            return 0;
        else
            return acos(clamp<float>(mydirection.dotProduct(distance) / distancelength, -1, 1));
    }

    /**
        @brief Gets the 2D viewing direction (up/down, left/right) to the position of the other object.
        @param myposition My position
        @param mydirection My viewing direction
        @param myorthonormal My orthonormalvector (pointing upwards through my head)
        @param otherposition The position of the other object
        @return The viewing direction

        Examples:
         - If the other object is exactly in front of me, the function returns <tt>Vector2(0, 0)</tt>.
         - If the other object is exactly at my left, the function returns <tt>Vector2(-1, 0)</tt>.
         - If the other object is exactly at my right, the function returns <tt>Vector2(1, 0)</tt>.
         - If the other object is only a bit at my right, the function still returns <tt>Vector2(1, 0)</tt>.
         - If the other object is exactly above me, the function returns <tt>Vector2(0, 1)</tt>.
    */
    orxonox::Vector2 get2DViewdirection(const orxonox::Vector3& myposition, const orxonox::Vector3& mydirection, const orxonox::Vector3& myorthonormal, const orxonox::Vector3& otherposition)
    {
        orxonox::Vector3 distance = otherposition - myposition;

        // project difference vector on our plane
        orxonox::Vector3 projection = Ogre::Plane(mydirection, myposition).projectVector(distance);

        float projectionlength = projection.length();
        if (projectionlength == 0)
        {
            if (myposition.dotProduct(otherposition) >= 0)
                return orxonox::Vector2(0, 0);
            else
                return orxonox::Vector2(0, 1);
        }

        float cos_value = clamp<float>(myorthonormal.dotProduct(projection) / projectionlength, -1, 1);
        float sin_value = sqrt( 1 - cos_value*cos_value );

        if ((mydirection.crossProduct(myorthonormal)).dotProduct(distance) > 0)
            return orxonox::Vector2( sin_value, cos_value );
        else
            return orxonox::Vector2( -sin_value, cos_value );
    }

    /**
        @brief Gets the 2D viewing direction (up/down, left/right) to the position of the other object, multiplied with the viewing distance to the object (0° = 0, 180° = 1).
        @param myposition My position
        @param mydirection My viewing direction
        @param myorthonormal My orthonormalvector (pointing upwards through my head)
        @param otherposition The position of the other object
        @return The viewing direction

        Examples:
         - If the other object is exactly in front of me, the function returns <tt>Vector2(0, 0)</tt>.
         - If the other object is exactly at my left, the function returns <tt>Vector2(-0.5, 0)</tt>.
         - If the other object is exactly at my right, the function returns <tt>Vector2(0.5, 0)</tt>.
         - If the other object is only a bit at my right, the function still returns <tt>Vector2(0.01, 0)</tt>.
         - If the other object is exactly above me, the function returns <tt>Vector2(0, 0.5)</tt>.
    */
    orxonox::Vector2 get2DViewcoordinates(const orxonox::Vector3& myposition, const orxonox::Vector3& mydirection, const orxonox::Vector3& myorthonormal, const orxonox::Vector3& otherposition)
    {
        orxonox::Vector3 distance = otherposition - myposition;

        // project difference vector on our plane
        orxonox::Vector3 projection = Ogre::Plane(mydirection, myposition).projectVector(distance);

        float projectionlength = projection.length();
        if (projectionlength == 0)
        {
            if (myposition.dotProduct(otherposition) >= 0)
                return orxonox::Vector2(0, 0);
            else
                return orxonox::Vector2(0, 1);
        }
        //float angle = acos(clamp<float>(myorthonormal.dotProduct(projection) / projectionlength, -1, 1));

        float cos_value = clamp<float>(myorthonormal.dotProduct(projection) / projectionlength, -1, 1);
        float sin_value = sqrt( 1 - cos_value*cos_value );

        float distancelength = distance.length();
        if (distancelength == 0) return orxonox::Vector2(0, 0);
        float radius = acos(clamp<float>(mydirection.dotProduct(distance) / distancelength, -1, 1)) / math::pi;

        if ((mydirection.crossProduct(myorthonormal)).dotProduct(distance) > 0)
            return orxonox::Vector2( sin_value * radius, cos_value * radius);
        else
            return orxonox::Vector2( -sin_value * radius, cos_value * radius);
    }

    /**
        @brief Returns the predicted position I have to aim at, if I want to hit a moving target with a moving projectile.
        @param myposition My position
        @param projectilespeed The speed of my projectile
        @param targetposition The position of my target
        @param targetvelocity The velocity of my target
        @return The predicted position

        The function predicts the position based on a linear velocity of the target. If the target changes speed or direction, the projectile will miss.
    */
    orxonox::Vector3 getPredictedPosition(const orxonox::Vector3& myposition, float projectilespeed, const orxonox::Vector3& targetposition, const orxonox::Vector3& targetvelocity)
    {
        float squaredProjectilespeed = projectilespeed * projectilespeed;
        orxonox::Vector3 distance = targetposition - myposition;
        float a = distance.squaredLength();
        float b = 2 * (distance.x + distance.y + distance.z) * (targetvelocity.x + targetvelocity.y + targetvelocity.z);
        float c = targetvelocity.squaredLength();

        float temp = 4*squaredProjectilespeed*c + a*a - 4*b*c;
        if (temp < 0)
            return orxonox::Vector3::ZERO;

        temp = sqrt(temp);
        float time = (temp + a) / (2 * (squaredProjectilespeed - b));
        return (targetposition + targetvelocity * time);
    }

    /**
        @brief Returns a unique number. This function will never return the same value twice.
    */
    unsigned long getUniqueNumber()
    {
        static unsigned long aNumber = 135;
        return aNumber++;
    }


    //////////////////////////
    // Conversion functions //
    //////////////////////////

    // std::string to Vector2
    bool ConverterFallback<std::string, orxonox::Vector2>::convert(orxonox::Vector2* output, const std::string& input)
    {
        size_t opening_parenthesis, closing_parenthesis = input.find('}');
        if ((opening_parenthesis = input.find('{')) == std::string::npos)
            opening_parenthesis = 0;
        else
            opening_parenthesis++;

        SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis),
                         ",", SubString::WhiteSpaces, false, '\\', true, '"', true, '\0', '\0', true, '\0');
        if (tokens.size() >= 2)
        {
            if (!convertValue(&(output->x), tokens[0]))
                return false;
            if (!convertValue(&(output->y), tokens[1]))
                return false;

            return true;
        }
        return false;
    }

    // std::string to Vector3
    bool ConverterFallback<std::string, orxonox::Vector3>::convert(orxonox::Vector3* output, const std::string& input)
    {
        size_t opening_parenthesis, closing_parenthesis = input.find('}');
        if ((opening_parenthesis = input.find('{')) == std::string::npos)
            opening_parenthesis = 0;
        else
            opening_parenthesis++;

        SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis),
                         ",", SubString::WhiteSpaces, false, '\\', true, '"', true, '\0', '\0', true, '\0');
        if (tokens.size() >= 3)
        {
            if (!convertValue(&(output->x), tokens[0]))
                return false;
            if (!convertValue(&(output->y), tokens[1]))
                return false;
            if (!convertValue(&(output->z), tokens[2]))
                return false;

            return true;
        }
        return false;
    }

    // std::string to Vector4
    bool ConverterFallback<std::string, orxonox::Vector4>::convert(orxonox::Vector4* output, const std::string& input)
    {
        size_t opening_parenthesis, closing_parenthesis = input.find('}');
        if ((opening_parenthesis = input.find('{')) == std::string::npos)
            opening_parenthesis = 0;
        else
            opening_parenthesis++;

        SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis),
                         ",", SubString::WhiteSpaces, false, '\\', true, '"', true, '\0', '\0', true, '\0');
        if (tokens.size() >= 4)
        {
            if (!convertValue(&(output->x), tokens[0]))
                return false;
            if (!convertValue(&(output->y), tokens[1]))
                return false;
            if (!convertValue(&(output->z), tokens[2]))
                return false;
            if (!convertValue(&(output->w), tokens[3]))
                return false;

            return true;
        }
        return false;
    }

    // std::string to Quaternion
    bool ConverterFallback<std::string, orxonox::Quaternion>::convert(orxonox::Quaternion* output, const std::string& input)
    {
        size_t opening_parenthesis, closing_parenthesis = input.find('}');
        if ((opening_parenthesis = input.find('{')) == std::string::npos)
            opening_parenthesis = 0;
        else
            opening_parenthesis++;

        SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis), ",", SubString::WhiteSpaces, false, '\\', true, '"', true, '\0', '\0', true, '\0');
        if (tokens.size() >= 4)
        {
            if (!convertValue(&(output->w), tokens[0]))
                return false;
            if (!convertValue(&(output->x), tokens[1]))
                return false;
            if (!convertValue(&(output->y), tokens[2]))
                return false;
            if (!convertValue(&(output->z), tokens[3]))
                return false;

            return true;
        }
        return false;
    }

    // std::string to ColourValue
    bool ConverterFallback<std::string, orxonox::ColourValue>::convert(orxonox::ColourValue* output, const std::string& input)
    {
        size_t opening_parenthesis, closing_parenthesis = input.find('}');
        if ((opening_parenthesis = input.find('{')) == std::string::npos)
            opening_parenthesis = 0;
        else
            opening_parenthesis++;

        SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis), ",", SubString::WhiteSpaces, false, '\\', true, '"', true, '\0', '\0', true, '\0');
        if (tokens.size() >= 3)
        {
            if (!convertValue(&(output->r), tokens[0]))
                return false;
            if (!convertValue(&(output->g), tokens[1]))
                return false;
            if (!convertValue(&(output->b), tokens[2]))
                return false;
            if (tokens.size() >= 4)
            {
                if (!convertValue(&(output->a), tokens[3]))
                    return false;
            }
            else
                output->a = 1.0;

            return true;
        }
        return false;
    }
}
