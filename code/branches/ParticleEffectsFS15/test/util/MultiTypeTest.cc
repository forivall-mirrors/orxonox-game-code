#include <gtest/gtest.h>
#include "util/MultiType.h"

namespace orxonox
{
    // x constructor()
    // x constructor(value)
    // x constructor(other)

    // x assignment(value)
    // ? assignment(pointer)
    // x assignment(other)

    // x set(value)
    // ? set(pointer)
    // x force<type>(value)

    // x convert<type>()
    // x reset
    // x reset<type>()
    // x resetValue

    // x isType<type>()

    // x (type) conversion
    // ? (pointer class) conversion
    // x getValue(pointer)
    // x get<type>()
    // ? getPointer()

    // x lastConversionSuccessful()
    // x null()

    /////////////////////////
    // Default Constructor //
    /////////////////////////
    TEST(MultiType, DefaultConstructor)
    {
        MultiType mt;
        EXPECT_TRUE(mt.null());
        EXPECT_TRUE(mt.isType<void>());
    }

    /////////////////
    // Constructor //
    /////////////////
    TEST(MultiType, ValueChar)              { char value = -100;                                MultiType mt(value);    EXPECT_TRUE(mt.isType<char>());                 EXPECT_EQ(-100, mt.get<char>()); }
    TEST(MultiType, ValueUnsignedChar)      { unsigned char value = 255u;                       MultiType mt(value);    EXPECT_TRUE(mt.isType<unsigned char>());        EXPECT_EQ(255u, mt.get<unsigned char>()); }
    TEST(MultiType, ValueShort)             { short value = -10000;                             MultiType mt(value);    EXPECT_TRUE(mt.isType<short>());                EXPECT_EQ(-10000, mt.get<short>()); }
    TEST(MultiType, ValueUnsignedShort)     { unsigned short value = 65535u;                    MultiType mt(value);    EXPECT_TRUE(mt.isType<unsigned short>());       EXPECT_EQ(65535u, mt.get<unsigned short>()); }
    TEST(MultiType, ValueInt)               { int value = -1000000000;                          MultiType mt(value);    EXPECT_TRUE(mt.isType<int>());                  EXPECT_EQ(-1000000000, mt.get<int>()); }
    TEST(MultiType, ValueUnsignedInt)       { unsigned int value = 4000000000u;                 MultiType mt(value);    EXPECT_TRUE(mt.isType<unsigned int>());         EXPECT_EQ(4000000000u, mt.get<unsigned int>()); }
    TEST(MultiType, ValueLong)              { long value = -1000000000;                         MultiType mt(value);    EXPECT_TRUE(mt.isType<long>());                 EXPECT_EQ(-1000000000, mt.get<long>()); }
    TEST(MultiType, ValueUnsignedLong)      { unsigned long value = 4000000000u;                MultiType mt(value);    EXPECT_TRUE(mt.isType<unsigned long>());        EXPECT_EQ(4000000000u, mt.get<unsigned long>()); }
    TEST(MultiType, ValueLongLong)          { long long value = -1000000000000000000L;          MultiType mt(value);    EXPECT_TRUE(mt.isType<long long>());            EXPECT_EQ(-1000000000000000000L, mt.get<long long>()); }
    TEST(MultiType, ValueUnsignedLongLong)  { unsigned long long value = 4000000000000000000UL; MultiType mt(value);    EXPECT_TRUE(mt.isType<unsigned long long>());   EXPECT_EQ(4000000000000000000UL, mt.get<unsigned long long>()); }
    TEST(MultiType, ValueFloat)             { float value = 0.123456f;                          MultiType mt(value);    EXPECT_TRUE(mt.isType<float>());                EXPECT_EQ(0.123456f, mt.get<float>()); }
    TEST(MultiType, ValueDouble)            { double value = 0.123456789;                       MultiType mt(value);    EXPECT_TRUE(mt.isType<double>());               EXPECT_EQ(0.123456789, mt.get<double>()); }
    TEST(MultiType, ValueLongDouble)        { long double value = 0.123456789123456789;         MultiType mt(value);    EXPECT_TRUE(mt.isType<long double>());          EXPECT_EQ(0.123456789123456789, mt.get<long double>()); }
    TEST(MultiType, ValueBool)              { bool value = true;                                MultiType mt(value);    EXPECT_TRUE(mt.isType<bool>());                 EXPECT_EQ(true, mt.get<bool>()); }
    TEST(MultiType, ValueVoidpointer)       { int* pointer = new int; void* value = pointer;    MultiType mt(value);    EXPECT_TRUE(mt.isType<void*>());                EXPECT_EQ(value, mt.get<void*>()); delete pointer; }
    TEST(MultiType, ValueSomepointer)       { int* value = new int;                             MultiType mt(value);    EXPECT_TRUE(mt.isType<void*>());                EXPECT_EQ(value, mt.get<int*>()); delete value; }
    TEST(MultiType, ValueString)            { std::string value = "Hello World";                MultiType mt(value);    EXPECT_TRUE(mt.isType<std::string>());          EXPECT_EQ("Hello World", mt.get<std::string>()); }
    TEST(MultiType, ValueVector2)           { Vector2 value = Vector2(11, 22);                  MultiType mt(value);    EXPECT_TRUE(mt.isType<Vector2>());              EXPECT_EQ(Vector2(11, 22), mt.get<Vector2>()); }
    TEST(MultiType, ValueVector3)           { Vector3 value = Vector3(11, 22, 33);              MultiType mt(value);    EXPECT_TRUE(mt.isType<Vector3>());              EXPECT_EQ(Vector3(11, 22, 33), mt.get<Vector3>()); }
    TEST(MultiType, ValueVector4)           { Vector4 value = Vector4(11, 22, 33, 44);          MultiType mt(value);    EXPECT_TRUE(mt.isType<Vector4>());              EXPECT_EQ(Vector4(11, 22, 33, 44), mt.get<Vector4>()); }
    TEST(MultiType, ValueColourValue)       { ColourValue value = ColourValue(11, 22, 33, 44);  MultiType mt(value);    EXPECT_TRUE(mt.isType<ColourValue>());          EXPECT_EQ(ColourValue(11, 22, 33, 44), mt.get<ColourValue>()); }
    TEST(MultiType, ValueQuaternion)        { Quaternion value = Quaternion(11, 22, 33, 44);    MultiType mt(value);    EXPECT_TRUE(mt.isType<Quaternion>());           EXPECT_EQ(Quaternion(11, 22, 33, 44), mt.get<Quaternion>()); }
    TEST(MultiType, ValueRadian)            { Radian value = Radian(0.123);                     MultiType mt(value);    EXPECT_TRUE(mt.isType<Radian>());               EXPECT_EQ(Radian(0.123), mt.get<Radian>()); }
    TEST(MultiType, ValueDegree)            { Degree value = Degree(123);                       MultiType mt(value);    EXPECT_TRUE(mt.isType<Degree>());               EXPECT_EQ(Degree(123), mt.get<Degree>()); }
    TEST(MultiType, ValueMbool)             { mbool value = mbool(true);                        MultiType mt(value);    EXPECT_TRUE(mt.isType<bool>());                 EXPECT_EQ(mbool(true), mt.get<bool>()); }
    TEST(MultiType, ValueCharPointer)       { const char* value = "Hello World";                MultiType mt(value);    EXPECT_TRUE(mt.isType<std::string>());          EXPECT_EQ("Hello World", mt.get<std::string>()); }

    //////////////////////
    // Copy-Constructor //
    //////////////////////
    TEST(MultiType, CopyConstructorEmpty)
    {
        MultiType mt1;
        MultiType mt2(mt1);

        EXPECT_TRUE(mt2.null());
    }

    TEST(MultiType, CopyConstructorFloat)
    {
        MultiType mt1(0.1234f);
        MultiType mt2(mt1);

        EXPECT_TRUE(mt2.isType<float>());
        EXPECT_EQ(0.1234f, mt2.get<float>());
    }

    //////////////////////
    // Assignment Value //
    //////////////////////
    TEST(MultiType, AssignmentIntToEmpty)
    {
        MultiType mt;

        EXPECT_TRUE(mt.null());

        mt = 55;

        EXPECT_FALSE(mt.null());
        EXPECT_TRUE(mt.isType<int>());
        EXPECT_EQ(55, mt.get<int>());
    }

    TEST(MultiType, AssignmentFloatToInt)
    {
        MultiType mt(66);

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_EQ(66, mt.get<int>());

        mt = 77.7f; // will be converted to int

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_EQ(77, mt.get<int>());
        EXPECT_EQ(77.0f, mt.get<float>());
    }

    TEST(MultiType, AssignmentFloatToFloat)
    {
        MultiType mt(66.6f);

        EXPECT_TRUE(mt.isType<float>());
        EXPECT_EQ(66, mt.get<int>());
        EXPECT_EQ(66.6f, mt.get<float>());

        mt = 77.7f;

        EXPECT_TRUE(mt.isType<float>());
        EXPECT_EQ(77, mt.get<int>());
        EXPECT_EQ(77.7f, mt.get<float>());
    }

    TEST(MultiType, AssignmentFloatToVector3)
    {
        MultiType mt(Vector3(1, 2, 3));

        EXPECT_TRUE(mt.isType<Vector3>());
        EXPECT_EQ(Vector3(1, 2, 3), mt.get<Vector3>());
        EXPECT_TRUE(mt.lastConversionSuccessful());

        mt = 77.7f;

        EXPECT_TRUE(mt.isType<Vector3>());
        EXPECT_EQ(Vector3::ZERO, mt.get<Vector3>());
        EXPECT_FALSE(mt.lastConversionSuccessful());
    }

    //////////////////////
    // Assignment Other //
    //////////////////////
    TEST(MultiType, AssignmentOther)
    {
        MultiType mt1(33);
        MultiType mt2(44.4f);

        EXPECT_TRUE(mt1.isType<int>());
        EXPECT_TRUE(mt2.isType<float>());

        EXPECT_EQ(33, mt1.get<int>());
        EXPECT_EQ(33.0f, mt1.get<float>());
        EXPECT_EQ(44.4f, mt2.get<float>());

        mt1 = mt2;

        EXPECT_TRUE(mt1.isType<int>());
        EXPECT_TRUE(mt2.isType<float>());

        EXPECT_EQ(44, mt1.get<int>());
        EXPECT_EQ(44.0f, mt1.get<float>());
        EXPECT_EQ(44.4f, mt2.get<float>());
    }

    ////////////////
    // set(value) //
    ////////////////
    TEST(MultiType, SetValueBoolToEmpty)
    {
        MultiType mt;

        mt.set(true);

        EXPECT_TRUE(mt.isType<bool>());
        EXPECT_EQ(true, mt.get<bool>());
    }

    TEST(MultiType, SetValueIntToString)
    {
        MultiType mt("Hello");

        EXPECT_TRUE(mt.isType<std::string>());
        EXPECT_EQ("Hello", mt.get<std::string>());

        mt.set(1234);

        EXPECT_TRUE(mt.isType<std::string>());
        EXPECT_EQ("1234", mt.get<std::string>());
    }

    //////////////////////
    // force<type>(value) //
    //////////////////////
    TEST(MultiType, SetValueWithTypeIntToString)
    {
        MultiType mt("Hello");

        EXPECT_TRUE(mt.isType<std::string>());
        EXPECT_EQ("Hello", mt.get<std::string>());

        mt.force<int>(1234);

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_EQ(1234, mt.get<int>());
    }

    TEST(MultiType, SetValueWithTypeIntAsStringToString)
    {
        MultiType mt("Hello");

        EXPECT_TRUE(mt.isType<std::string>());
        EXPECT_EQ("Hello", mt.get<std::string>());

        mt.force<int>("1234");

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_EQ(1234, mt.get<int>());
    }

    TEST(MultiType, SetValueWithTypeIntToInt)
    {
        MultiType mt(4321);

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_EQ(4321, mt.get<int>());

        mt.force<int>(1234);

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_EQ(1234, mt.get<int>());
    }

    /////////////////////
    // convert<type>() //
    /////////////////////
    TEST(MultiType, ConvertEmptyToInt)
    {
        MultiType mt;
        mt.convert<int>();

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_EQ(0, mt.get<int>());
    }

    TEST(MultiType, ConvertFloatToInt)
    {
        MultiType mt(1.234f);

        EXPECT_TRUE(mt.isType<float>());
        EXPECT_EQ(1.234f, mt.get<float>());
        EXPECT_EQ(1, mt.get<int>());

        mt.convert<int>();

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_EQ(1.0f, mt.get<float>());
        EXPECT_EQ(1, mt.get<int>());
    }

    TEST(MultiType, ConvertFloatToVector3)
    {
        MultiType mt(1.234f);

        EXPECT_TRUE(mt.isType<float>());
        EXPECT_EQ(1.234f, mt.get<float>());
        EXPECT_EQ(Vector3::ZERO, mt.get<Vector3>());

        mt.convert<Vector3>();

        EXPECT_TRUE(mt.isType<Vector3>());
        EXPECT_EQ(0.0f, mt.get<float>());
        EXPECT_EQ(Vector3::ZERO, mt.get<Vector3>());

        EXPECT_FALSE(mt.lastConversionSuccessful());
    }

    ///////////
    // reset //
    ///////////
    TEST(MultiType, Reset)
    {
        MultiType mt(10);

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_FALSE(mt.isType<void>());
        EXPECT_FALSE(mt.null());
        EXPECT_EQ(10, mt.get<int>());

        mt.reset();

        EXPECT_FALSE(mt.isType<int>());
        EXPECT_TRUE(mt.isType<void>());
        EXPECT_TRUE(mt.null());
        EXPECT_EQ(0, mt.get<int>());
    }

    ///////////////////
    // reset<type>() //
    ///////////////////
    TEST(MultiType, SetType)
    {
        MultiType mt(10);

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_FALSE(mt.isType<float>());
        EXPECT_EQ(10, mt.get<int>());

        mt.reset<float>();

        EXPECT_FALSE(mt.isType<int>());
        EXPECT_TRUE(mt.isType<float>());
        EXPECT_EQ(0, mt.get<int>());
    }

    ////////////////
    // resetValue //
    ////////////////
    TEST(MultiType, resetValue)
    {
        MultiType mt(10);

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_EQ(10, mt.get<int>());

        mt.resetValue();

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_EQ(0, mt.get<int>());
    }

    /////////////////////
    // type operator() //
    /////////////////////
    TEST(MultiType, ConversionOperatorFloatToFloat)
    {
        MultiType mt(12.34f);
        float value = mt;
        EXPECT_EQ(12.34f, value);
    }

    TEST(MultiType, ConversionOperatorFloatToInt)
    {
        MultiType mt(12.34f);
        int value = mt;
        EXPECT_EQ(12, value);
    }

    TEST(MultiType, ConversionOperatorIntToFloat)
    {
        MultiType mt(123);
        float value = mt;
        EXPECT_EQ(123, value);
    }

    TEST(MultiType, ConversionOperatorIntToString)
    {
        MultiType mt(123);
        std::string value = mt;
        EXPECT_EQ("123", value);
    }

    TEST(MultiType, ConversionOperatorStringToInt)
    {
        MultiType mt("321");
        int value = mt;
        EXPECT_EQ(321, value);
    }

    namespace
    {
        void helper_function(const std::string& a, float b, int c)
        {
            EXPECT_EQ("555.555", a);
            EXPECT_EQ(555.555f, b);
            EXPECT_EQ(555, c);
        }
    }

    TEST(MultiType, ConversionOperatorPassToFunction)
    {
        MultiType mt("555.555");

        helper_function(mt, mt, mt);
    }

    ///////////////////////
    // getValue(pointer) //
    ///////////////////////
    TEST(MultiType, GetValueIntToInt)
    {
        MultiType mt(33);

        int value = 0;
        EXPECT_TRUE(mt.getValue(&value));
        EXPECT_EQ(33, value);
    }

    TEST(MultiType, GetValueIntToFloat)
    {
        MultiType mt(33);

        float value = 0;
        EXPECT_TRUE(mt.getValue(&value));
        EXPECT_EQ(33.0f, value);
    }

    TEST(MultiType, GetValueIntToVector3)
    {
        MultiType mt(33);

        Vector3 value = Vector3::ZERO;
        EXPECT_FALSE(mt.getValue(&value));
        EXPECT_EQ(Vector3::ZERO, value);
    }

    //////////////
    // get<type>() //
    //////////////
    TEST(MultiType, GetValueFromInt)
    {
        MultiType mt(256);

        EXPECT_EQ(0, mt.get<char>());
        EXPECT_EQ(0u, mt.get<unsigned char>());
        EXPECT_EQ(256, mt.get<short>());
        EXPECT_EQ(256u, mt.get<unsigned short>());
        EXPECT_EQ(256, mt.get<int>());
        EXPECT_EQ(256u, mt.get<unsigned int>());
        EXPECT_EQ(256, mt.get<long>());
        EXPECT_EQ(256u, mt.get<unsigned long>());
        EXPECT_EQ(256, mt.get<long long>());
        EXPECT_EQ(256u, mt.get<unsigned long long>());
        EXPECT_EQ(256.0f, mt.get<float>());
        EXPECT_EQ(256.0, mt.get<double>());
        EXPECT_EQ(256.0, mt.get<long double>());
        EXPECT_TRUE(mt.get<bool>());
        EXPECT_EQ("256", mt.get<std::string>());
    }

    TEST(MultiType, GetValueFromFloat)
    {
        MultiType mt(128.821);

        EXPECT_EQ(-128, mt.get<char>());
        EXPECT_EQ(128u, mt.get<unsigned char>());
        EXPECT_EQ(128, mt.get<short>());
        EXPECT_EQ(128u, mt.get<unsigned short>());
        EXPECT_EQ(128, mt.get<int>());
        EXPECT_EQ(128u, mt.get<unsigned int>());
        EXPECT_EQ(128, mt.get<long>());
        EXPECT_EQ(128u, mt.get<unsigned long>());
        EXPECT_EQ(128, mt.get<long long>());
        EXPECT_EQ(128u, mt.get<unsigned long long>());
        EXPECT_EQ(128.821f, mt.get<float>());
        EXPECT_EQ(128.821, mt.get<double>());
        EXPECT_EQ(128.821, mt.get<long double>());
        EXPECT_TRUE(mt.get<bool>());
        EXPECT_EQ("128.821", mt.get<std::string>());
    }

    TEST(MultiType, GetValueFromString)
    {
        MultiType mt("0.123");

        EXPECT_EQ('0', mt.get<char>());
        EXPECT_EQ('0', mt.get<unsigned char>());
        EXPECT_EQ(0, mt.get<short>());
        EXPECT_EQ(0u, mt.get<unsigned short>());
        EXPECT_EQ(0, mt.get<int>());
        EXPECT_EQ(0u, mt.get<unsigned int>());
        EXPECT_EQ(0, mt.get<long>());
        EXPECT_EQ(0u, mt.get<unsigned long>());
        EXPECT_EQ(0, mt.get<long long>());
        EXPECT_EQ(0u, mt.get<unsigned long long>());
        EXPECT_EQ(0.123f, mt.get<float>());
        EXPECT_EQ(0.123, mt.get<double>());
        EXPECT_DOUBLE_EQ(0.123, mt.get<long double>());
        EXPECT_FALSE(mt.get<bool>());
        EXPECT_EQ("0.123", mt.get<std::string>());
    }

    ////////////////////
    // isType<type>() //
    ////////////////////
    TEST(MultiType, IsTypeInt)
    {
        MultiType mt(1);

        EXPECT_FALSE(mt.isType<void>());
        EXPECT_FALSE(mt.isType<char>());
        EXPECT_FALSE(mt.isType<unsigned char>());
        EXPECT_FALSE(mt.isType<short>());
        EXPECT_FALSE(mt.isType<unsigned short>());
        EXPECT_TRUE(mt.isType<int>());
        EXPECT_FALSE(mt.isType<unsigned int>());
        EXPECT_FALSE(mt.isType<long>());
        EXPECT_FALSE(mt.isType<unsigned long>());
        EXPECT_FALSE(mt.isType<long long>());
        EXPECT_FALSE(mt.isType<unsigned long long>());
        EXPECT_FALSE(mt.isType<float>());
        EXPECT_FALSE(mt.isType<double>());
        EXPECT_FALSE(mt.isType<long double>());
        EXPECT_FALSE(mt.isType<bool>());
        EXPECT_FALSE(mt.isType<void*>());
        EXPECT_FALSE(mt.isType<std::string>());
        EXPECT_FALSE(mt.isType<orxonox::Vector2>());
        EXPECT_FALSE(mt.isType<orxonox::Vector3>());
        EXPECT_FALSE(mt.isType<orxonox::Vector4>());
        EXPECT_FALSE(mt.isType<orxonox::ColourValue>());
        EXPECT_FALSE(mt.isType<orxonox::Quaternion>());
        EXPECT_FALSE(mt.isType<orxonox::Radian>());
        EXPECT_FALSE(mt.isType<orxonox::Degree>());
    }

    TEST(MultiType, IsTypeUnsignedInt)
    {
        MultiType mt(1u);

        EXPECT_FALSE(mt.isType<void>());
        EXPECT_FALSE(mt.isType<char>());
        EXPECT_FALSE(mt.isType<unsigned char>());
        EXPECT_FALSE(mt.isType<short>());
        EXPECT_FALSE(mt.isType<unsigned short>());
        EXPECT_FALSE(mt.isType<int>());
        EXPECT_TRUE(mt.isType<unsigned int>());
        EXPECT_FALSE(mt.isType<long>());
        EXPECT_FALSE(mt.isType<unsigned long>());
        EXPECT_FALSE(mt.isType<long long>());
        EXPECT_FALSE(mt.isType<unsigned long long>());
        EXPECT_FALSE(mt.isType<float>());
        EXPECT_FALSE(mt.isType<double>());
        EXPECT_FALSE(mt.isType<long double>());
        EXPECT_FALSE(mt.isType<bool>());
        EXPECT_FALSE(mt.isType<void*>());
        EXPECT_FALSE(mt.isType<std::string>());
        EXPECT_FALSE(mt.isType<orxonox::Vector2>());
        EXPECT_FALSE(mt.isType<orxonox::Vector3>());
        EXPECT_FALSE(mt.isType<orxonox::Vector4>());
        EXPECT_FALSE(mt.isType<orxonox::ColourValue>());
        EXPECT_FALSE(mt.isType<orxonox::Quaternion>());
        EXPECT_FALSE(mt.isType<orxonox::Radian>());
        EXPECT_FALSE(mt.isType<orxonox::Degree>());
    }

    TEST(MultiType, IsTypeDouble)
    {
        MultiType mt(1.0);

        EXPECT_FALSE(mt.isType<void>());
        EXPECT_FALSE(mt.isType<char>());
        EXPECT_FALSE(mt.isType<unsigned char>());
        EXPECT_FALSE(mt.isType<short>());
        EXPECT_FALSE(mt.isType<unsigned short>());
        EXPECT_FALSE(mt.isType<int>());
        EXPECT_FALSE(mt.isType<unsigned int>());
        EXPECT_FALSE(mt.isType<long>());
        EXPECT_FALSE(mt.isType<unsigned long>());
        EXPECT_FALSE(mt.isType<long long>());
        EXPECT_FALSE(mt.isType<unsigned long long>());
        EXPECT_FALSE(mt.isType<float>());
        EXPECT_TRUE(mt.isType<double>());
        EXPECT_FALSE(mt.isType<long double>());
        EXPECT_FALSE(mt.isType<bool>());
        EXPECT_FALSE(mt.isType<void*>());
        EXPECT_FALSE(mt.isType<std::string>());
        EXPECT_FALSE(mt.isType<orxonox::Vector2>());
        EXPECT_FALSE(mt.isType<orxonox::Vector3>());
        EXPECT_FALSE(mt.isType<orxonox::Vector4>());
        EXPECT_FALSE(mt.isType<orxonox::ColourValue>());
        EXPECT_FALSE(mt.isType<orxonox::Quaternion>());
        EXPECT_FALSE(mt.isType<orxonox::Radian>());
        EXPECT_FALSE(mt.isType<orxonox::Degree>());
    }

    TEST(MultiType, IsTypeString)
    {
        MultiType mt("1");

        EXPECT_FALSE(mt.isType<void>());
        EXPECT_FALSE(mt.isType<char>());
        EXPECT_FALSE(mt.isType<unsigned char>());
        EXPECT_FALSE(mt.isType<short>());
        EXPECT_FALSE(mt.isType<unsigned short>());
        EXPECT_FALSE(mt.isType<int>());
        EXPECT_FALSE(mt.isType<unsigned int>());
        EXPECT_FALSE(mt.isType<long>());
        EXPECT_FALSE(mt.isType<unsigned long>());
        EXPECT_FALSE(mt.isType<long long>());
        EXPECT_FALSE(mt.isType<unsigned long long>());
        EXPECT_FALSE(mt.isType<float>());
        EXPECT_FALSE(mt.isType<double>());
        EXPECT_FALSE(mt.isType<long double>());
        EXPECT_FALSE(mt.isType<bool>());
        EXPECT_FALSE(mt.isType<void*>());
        EXPECT_TRUE(mt.isType<std::string>());
        EXPECT_FALSE(mt.isType<orxonox::Vector2>());
        EXPECT_FALSE(mt.isType<orxonox::Vector3>());
        EXPECT_FALSE(mt.isType<orxonox::Vector4>());
        EXPECT_FALSE(mt.isType<orxonox::ColourValue>());
        EXPECT_FALSE(mt.isType<orxonox::Quaternion>());
        EXPECT_FALSE(mt.isType<orxonox::Radian>());
        EXPECT_FALSE(mt.isType<orxonox::Degree>());
    }

    TEST(MultiType, IsTypeDegree)
    {
        MultiType mt(Degree(1));

        EXPECT_FALSE(mt.isType<void>());
        EXPECT_FALSE(mt.isType<char>());
        EXPECT_FALSE(mt.isType<unsigned char>());
        EXPECT_FALSE(mt.isType<short>());
        EXPECT_FALSE(mt.isType<unsigned short>());
        EXPECT_FALSE(mt.isType<int>());
        EXPECT_FALSE(mt.isType<unsigned int>());
        EXPECT_FALSE(mt.isType<long>());
        EXPECT_FALSE(mt.isType<unsigned long>());
        EXPECT_FALSE(mt.isType<long long>());
        EXPECT_FALSE(mt.isType<unsigned long long>());
        EXPECT_FALSE(mt.isType<float>());
        EXPECT_FALSE(mt.isType<double>());
        EXPECT_FALSE(mt.isType<long double>());
        EXPECT_FALSE(mt.isType<bool>());
        EXPECT_FALSE(mt.isType<void*>());
        EXPECT_FALSE(mt.isType<std::string>());
        EXPECT_FALSE(mt.isType<orxonox::Vector2>());
        EXPECT_FALSE(mt.isType<orxonox::Vector3>());
        EXPECT_FALSE(mt.isType<orxonox::Vector4>());
        EXPECT_FALSE(mt.isType<orxonox::ColourValue>());
        EXPECT_FALSE(mt.isType<orxonox::Quaternion>());
        EXPECT_FALSE(mt.isType<orxonox::Radian>());
        EXPECT_TRUE(mt.isType<orxonox::Degree>());
    }

    ////////////////////////////////
    // lastConversionSuccessful() //
    ////////////////////////////////
    TEST(MultiType, LastConversionSuccessful)
    {
        MultiType mt;

        EXPECT_TRUE(mt.lastConversionSuccessful());

        mt = 5.55;

        EXPECT_TRUE(mt.lastConversionSuccessful());

        mt.convert<int>();

        EXPECT_TRUE(mt.lastConversionSuccessful());

        mt.convert<Vector3>();

        EXPECT_FALSE(mt.lastConversionSuccessful());

        mt.convert<std::string>();

        EXPECT_TRUE(mt.lastConversionSuccessful());
    }

    ////////////
    // null() //
    ////////////
    TEST(MultiType, Null1)
    {
        MultiType mt;

        EXPECT_TRUE(mt.null());

        mt = 5;

        EXPECT_FALSE(mt.null());
    }

    TEST(MultiType, Null2)
    {
        MultiType mt(5);

        EXPECT_FALSE(mt.null());

        mt.reset();

        EXPECT_TRUE(mt.null());
    }
}
