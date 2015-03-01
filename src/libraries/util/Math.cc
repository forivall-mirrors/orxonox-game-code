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
 *      Wolfgang Roenninger
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
            @brief Gets the 2D project vector for the 3D Radar .
            @param myposition My position
            @param mydirection My viewing direction
            @param myorthonormal My orthonormalvector (pointing upwards through my head)
            @param otherposition The position of the other object
            @param mapangle The angle between line of sight on screen and the 3Dmap-x/z-plain in radian
            @param detectionlimit The limit in which objects are shown on the map
            @return The viewing direction
    */
    orxonox::Vector2 get3DProjection(const orxonox::Vector3& myposition, const orxonox::Vector3& mydirection, const orxonox::Vector3& myorthonormal, const orxonox::Vector3& otherposition, const float mapangle, const float detectionlimit)
    {
        // Orxonox Vectors: x_direction you are looking, y_direction points up, z_direction points to the right
        orxonox::Vector3 distance = otherposition - myposition; // get vector from Ship to object

        // new coordinate system: x_axsis: mydirection   (points front)
        //                        y_axsis: myorthonormal (points up)
        //                        z_axsis: myside        (points right)

        orxonox::Vector3 myside = mydirection.crossProduct(myorthonormal); // get 3. base vector

        distance = 4*distance / detectionlimit; // shrink vector on map
        if(distance.length() > 1.0f) // if object would wander outside of the map
        {
            distance = distance / distance.length();
        }

        // perform a coordinate transformation to get distance in relation of the position of the ship
        orxonox::Vector3 distanceShip = getTransformedVector(distance, mydirection, myorthonormal, myside);

        // calculate 2D vector on the map (with angle between x/z - plain and line of sight)
        float xcoordinate = distanceShip.z; // z; cause x direction on screen is to the right side
        float ycoordinate = distanceShip.x*sin(mapangle)+distanceShip.y*cos(mapangle);
        return orxonox::Vector2(xcoordinate , ycoordinate);
    }

    /**
               @brief Gets if a object is over the x/z - plain on map
               @param myposition My position
               @param mydirection My viewing direction
               @param myorthonormal My orthonormalvector (pointing upwards through my head)
               @param otherposition The position of the other object
               @param mapangle The angle you look on the 3Dmap in radian
               @return If distancevector to the other object has a positive y-coordinate

               Examples:
                Returns true if object is over x/z - plain
                Returns false if object is below x/z -plain
    */
    bool isObjectHigherThanShipOnMap(const orxonox::Vector3& myposition, const orxonox::Vector3& mydirection, const orxonox::Vector3& myorthonormal, const orxonox::Vector3& otherposition, const float mapangle)
    {
        // Orxonox Vectors: x_direction you are looking, y_direction points up, z_direction points to the right
        orxonox::Vector3 distance = otherposition - myposition;

        // new coordinate system: x_axsis: mydirection   (points front)
        //                        y_axsis: myorthonormal (points up)
        //                        z_axsis: myside        (points right)

        orxonox::Vector3 myside = mydirection.crossProduct(myorthonormal); // get vector from Ship to object


        // perform a coordinate transformation to get distance in relation of the position of the ship
        orxonox::Vector3 distanceShip = getTransformedVector(distance, mydirection, myorthonormal, myside);

        if(distanceShip.y >= 0)
            return true;
        else
            return false;
    }

    /**
                   @brief A value between 0 and 10, in order how other object is in front or in back
                   @param myposition My position
                   @param mydirection My viewing direction
                   @param myorthonormal My orthonormalvector (pointing upwards through my head)
                   @param otherposition The position of the other object
                   @param detectionlimit The limit in which objects are shown on the map
                   @return value between 0 and 100
    */
    int determineMap3DZOrder(const orxonox::Vector3& myposition, const orxonox::Vector3& mydirection, const orxonox::Vector3& myorthonormal, const orxonox::Vector3& otherposition, const float detectionlimit)
    {
        orxonox::Vector3 distance = otherposition - myposition; // get vector from Ship to object
        orxonox::Vector3 myside = mydirection.crossProduct(myorthonormal); // get vector to the side

        distance = 4*distance / detectionlimit; // shrink vector on map
        if(distance.length() > 1.0f) // if object would wander outside of the map
        {
            distance = distance / distance.length();
        }

        // perform a coordinate transformation to get distance in relation of the position of the ship
        orxonox::Vector3 distanceShip = getTransformedVector(distance, mydirection, myorthonormal, myside);

        return static_cast<int>(50 - 100*distanceShip.x);
    }


    /**
                @brief Gets the new vector after a coordinate transformation
                @param distance Vector which will be transformed
                @param mydirection New x basevector
                @param myorthonormal New y basevector
                @param otherposition New z basevector
                @return direction in the new coordinates

                x is vector in old coordinates
                y is vector in old coordinates
                T is transform matrix with:
                    T = (t1 , t2 , t3)
                    t1 = mydirection
                    t2 = myorthonormal
                    t3 = myside

                y = T^(-1)*x
            */
    orxonox::Vector3 getTransformedVector(const orxonox::Vector3& distance, const orxonox::Vector3& mydirection, const orxonox::Vector3& myorthonormal, const orxonox::Vector3& myside)
    {
        // inverse of the transform matrix
        float determinant = +mydirection.x * (myorthonormal.y*myside.z - myside.y*myorthonormal.z)
                            -mydirection.y * (myorthonormal.x*myside.z - myorthonormal.z*myside.x)
                            +mydirection.z * (myorthonormal.x*myside.y - myorthonormal.y*myside.x);
        float invdet = 1/determinant;

        // transform matrix
        orxonox::Vector3 xinvtransform;
        orxonox::Vector3 yinvtransform;
        orxonox::Vector3 zinvtransform;

        xinvtransform.x = (myorthonormal.y * myside.z        - myside.y        * myorthonormal.z)*invdet;
        xinvtransform.y = (mydirection.z   * myside.y        - mydirection.y   * myside.z       )*invdet;
        xinvtransform.z = (mydirection.y   * myorthonormal.z - mydirection.z   * myorthonormal.y)*invdet;
        yinvtransform.x = (myorthonormal.z * myside.x        - myorthonormal.x * myside.z       )*invdet;
        yinvtransform.y = (mydirection.x   * myside.z        - mydirection.z   * myside.x       )*invdet;
        yinvtransform.z = (myorthonormal.x * mydirection.z   - mydirection.x   * myorthonormal.z)*invdet;
        zinvtransform.x = (myorthonormal.x * myside.y        - myside.x        * myorthonormal.y)*invdet;
        zinvtransform.y = (myside.x        * mydirection.y   - mydirection.x   * myside.y       )*invdet;
        zinvtransform.z = (mydirection.x   * myorthonormal.y - myorthonormal.x * mydirection.y  )*invdet;

        // coordinate transformation
        orxonox::Vector3 distanceShip;
        distanceShip.x = xinvtransform.x * distance.x + yinvtransform.x * distance.y + zinvtransform.x * distance.z;
        distanceShip.y = xinvtransform.y * distance.x + yinvtransform.y * distance.y + zinvtransform.y * distance.z;
        distanceShip.z = xinvtransform.z * distance.x + yinvtransform.z * distance.y + zinvtransform.z * distance.z;

        return distanceShip;
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
        orxonox::Vector3 distance = targetposition - myposition;
        float a = distance.squaredLength();
        float b = 2 * (distance.x * targetvelocity.x + distance.y * targetvelocity.y + distance.z * targetvelocity.z);
        float c = targetvelocity.squaredLength() - projectilespeed * projectilespeed;

        float discriminant = b*b - 4*a*c;
        if (discriminant < 0)
            return orxonox::Vector3::ZERO;

        float solution = (-b + sqrt(discriminant)) / (2 * a);
        float time = 1.0f / solution;

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
