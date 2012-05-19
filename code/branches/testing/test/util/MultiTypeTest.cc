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

    // x setValue(value)
    // ? setValue(pointer)
    // x setValue<type>(value)

    // x convert<type>()
    // x reset
    // x reset<type>()
    // x resetValue

    // x isType<type>()

    // x (type) conversion
    // ? (pointer class) conversion
    // x getValue(pointer)
    // x getXXX()
    // ? getPointer()

    // x hasDefaultValue()
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
    TEST(MultiType, ValueChar)              { char value = -100;                                MultiType mt(value);    EXPECT_TRUE(mt.isType<char>());                 EXPECT_EQ(-100, mt.getChar()); }
    TEST(MultiType, ValueUnsignedChar)      { unsigned char value = 255u;                       MultiType mt(value);    EXPECT_TRUE(mt.isType<unsigned char>());        EXPECT_EQ(255u, mt.getUnsignedChar()); }
    TEST(MultiType, ValueShort)             { short value = -10000;                             MultiType mt(value);    EXPECT_TRUE(mt.isType<short>());                EXPECT_EQ(-10000, mt.getShort()); }
    TEST(MultiType, ValueUnsignedShort)     { unsigned short value = 65535u;                    MultiType mt(value);    EXPECT_TRUE(mt.isType<unsigned short>());       EXPECT_EQ(65535u, mt.getUnsignedShort()); }
    TEST(MultiType, ValueInt)               { int value = -1000000000;                          MultiType mt(value);    EXPECT_TRUE(mt.isType<int>());                  EXPECT_EQ(-1000000000, mt.getInt()); }
    TEST(MultiType, ValueUnsignedInt)       { unsigned int value = 4000000000u;                 MultiType mt(value);    EXPECT_TRUE(mt.isType<unsigned int>());         EXPECT_EQ(4000000000u, mt.getUnsignedInt()); }
    TEST(MultiType, ValueLong)              { long value = -1000000000;                         MultiType mt(value);    EXPECT_TRUE(mt.isType<long>());                 EXPECT_EQ(-1000000000, mt.getLong()); }
    TEST(MultiType, ValueUnsignedLong)      { unsigned long value = 4000000000u;                MultiType mt(value);    EXPECT_TRUE(mt.isType<unsigned long>());        EXPECT_EQ(4000000000u, mt.getUnsignedLong()); }
    TEST(MultiType, ValueLongLong)          { long long value = -1000000000000000000L;          MultiType mt(value);    EXPECT_TRUE(mt.isType<long long>());            EXPECT_EQ(-1000000000000000000L, mt.getLongLong()); }
    TEST(MultiType, ValueUnsignedLongLong)  { unsigned long long value = 4000000000000000000UL; MultiType mt(value);    EXPECT_TRUE(mt.isType<unsigned long long>());   EXPECT_EQ(4000000000000000000UL, mt.getUnsignedLongLong()); }
    TEST(MultiType, ValueFloat)             { float value = 0.123456f;                          MultiType mt(value);    EXPECT_TRUE(mt.isType<float>());                EXPECT_EQ(0.123456f, mt.getFloat()); }
    TEST(MultiType, ValueDouble)            { double value = 0.123456789;                       MultiType mt(value);    EXPECT_TRUE(mt.isType<double>());               EXPECT_EQ(0.123456789, mt.getDouble()); }
    TEST(MultiType, ValueLongDouble)        { long double value = 0.123456789123456789;         MultiType mt(value);    EXPECT_TRUE(mt.isType<long double>());          EXPECT_EQ(0.123456789123456789, mt.getLongDouble()); }
    TEST(MultiType, ValueBool)              { bool value = true;                                MultiType mt(value);    EXPECT_TRUE(mt.isType<bool>());                 EXPECT_EQ(true, mt.getBool()); }
    TEST(MultiType, ValueVoidpointer)       { int* pointer = new int; void* value = pointer;    MultiType mt(value);    EXPECT_TRUE(mt.isType<void*>());                EXPECT_EQ(value, mt.getPointer<void>()); delete pointer; }
    TEST(MultiType, ValueString)            { std::string value = "Hello World";                MultiType mt(value);    EXPECT_TRUE(mt.isType<std::string>());          EXPECT_EQ("Hello World", mt.getString()); }
    TEST(MultiType, ValueVector2)           { Vector2 value = Vector2(11, 22);                  MultiType mt(value);    EXPECT_TRUE(mt.isType<Vector2>());              EXPECT_EQ(Vector2(11, 22), mt.getVector2()); }
    TEST(MultiType, ValueVector3)           { Vector3 value = Vector3(11, 22, 33);              MultiType mt(value);    EXPECT_TRUE(mt.isType<Vector3>());              EXPECT_EQ(Vector3(11, 22, 33), mt.getVector3()); }
    TEST(MultiType, ValueVector4)           { Vector4 value = Vector4(11, 22, 33, 44);          MultiType mt(value);    EXPECT_TRUE(mt.isType<Vector4>());              EXPECT_EQ(Vector4(11, 22, 33, 44), mt.getVector4()); }
    TEST(MultiType, ValueColourValue)       { ColourValue value = ColourValue(11, 22, 33, 44);  MultiType mt(value);    EXPECT_TRUE(mt.isType<ColourValue>());          EXPECT_EQ(ColourValue(11, 22, 33, 44), mt.getColourValue()); }
    TEST(MultiType, ValueQuaternion)        { Quaternion value = Quaternion(11, 22, 33, 44);    MultiType mt(value);    EXPECT_TRUE(mt.isType<Quaternion>());           EXPECT_EQ(Quaternion(11, 22, 33, 44), mt.getQuaternion()); }
    TEST(MultiType, ValueRadian)            { Radian value = Radian(0.123);                     MultiType mt(value);    EXPECT_TRUE(mt.isType<Radian>());               EXPECT_EQ(Radian(0.123), mt.getRadian()); }
    TEST(MultiType, ValueDegree)            { Degree value = Degree(123);                       MultiType mt(value);    EXPECT_TRUE(mt.isType<Degree>());               EXPECT_EQ(Degree(123), mt.getDegree()); }
    TEST(MultiType, ValueMbool)             { mbool value = mbool(true);                        MultiType mt(value);    EXPECT_TRUE(mt.isType<bool>());                 EXPECT_EQ(mbool(true), mt.getBool()); }
    TEST(MultiType, ValueCharPointer)       { const char* value = "Hello World";                MultiType mt(value);    EXPECT_TRUE(mt.isType<std::string>());          EXPECT_EQ("Hello World", mt.getString()); }

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
        EXPECT_EQ(0.1234f, mt2.getFloat());
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
        EXPECT_EQ(55, mt.getInt());
    }

    TEST(MultiType, AssignmentFloatToInt)
    {
        MultiType mt(66);

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_EQ(66, mt.getInt());

        mt = 77.7f; // will be converted to int

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_EQ(77, mt.getInt());
        EXPECT_EQ(77.0f, mt.getFloat());
    }

    TEST(MultiType, AssignmentFloatToFloat)
    {
        MultiType mt(66.6f);

        EXPECT_TRUE(mt.isType<float>());
        EXPECT_EQ(66, mt.getInt());
        EXPECT_EQ(66.6f, mt.getFloat());

        mt = 77.7f;

        EXPECT_TRUE(mt.isType<float>());
        EXPECT_EQ(77, mt.getInt());
        EXPECT_EQ(77.7f, mt.getFloat());
    }

    TEST(MultiType, AssignmentFloatToVector3)
    {
        MultiType mt(Vector3(1, 2, 3));

        EXPECT_TRUE(mt.isType<Vector3>());
        EXPECT_EQ(Vector3(1, 2, 3), mt.getVector3());
        EXPECT_FALSE(mt.hasDefaultValue());

        mt = 77.7f;

        EXPECT_TRUE(mt.isType<Vector3>());
        EXPECT_EQ(Vector3::ZERO, mt.getVector3());
        EXPECT_TRUE(mt.hasDefaultValue());
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

        EXPECT_EQ(33, mt1.getInt());
        EXPECT_EQ(33.0f, mt1.getFloat());
        EXPECT_EQ(44.4f, mt2.getFloat());

        mt1 = mt2;

        EXPECT_TRUE(mt1.isType<int>());
        EXPECT_TRUE(mt2.isType<float>());

        EXPECT_EQ(44, mt1.getInt());
        EXPECT_EQ(44.0f, mt1.getFloat());
        EXPECT_EQ(44.4f, mt2.getFloat());
    }

    /////////////////////
    // setValue(value) //
    /////////////////////
    TEST(MultiType, SetValueBoolToEmpty)
    {
        MultiType mt;

        mt.setValue(true);

        EXPECT_TRUE(mt.isType<bool>());
        EXPECT_EQ(true, mt.getBool());
    }

    TEST(MultiType, SetValueIntToString)
    {
        MultiType mt("Hello");

        EXPECT_TRUE(mt.isType<std::string>());
        EXPECT_EQ("Hello", mt.getString());

        mt.setValue(1234);

        EXPECT_TRUE(mt.isType<std::string>());
        EXPECT_EQ("1234", mt.getString());
    }

    ///////////////////////////
    // setValue<type>(value) //
    ///////////////////////////
    TEST(MultiType, SetValueWithTypeIntToString)
    {
        MultiType mt("Hello");

        EXPECT_TRUE(mt.isType<std::string>());
        EXPECT_EQ("Hello", mt.getString());

        mt.setValue<int>(1234);

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_EQ(1234, mt.getInt());
    }

    TEST(MultiType, SetValueWithTypeIntAsStringToString)
    {
        MultiType mt("Hello");

        EXPECT_TRUE(mt.isType<std::string>());
        EXPECT_EQ("Hello", mt.getString());

        mt.setValue<int>("1234");

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_EQ(1234, mt.getInt());
    }

    TEST(MultiType, SetValueWithTypeIntToInt)
    {
        MultiType mt(4321);

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_EQ(4321, mt.getInt());

        mt.setValue<int>(1234);

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_EQ(1234, mt.getInt());
    }

    /////////////////////
    // convert<type>() //
    /////////////////////
    TEST(MultiType, ConvertEmptyToInt)
    {
        MultiType mt;
        mt.convert<int>();

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_EQ(0, mt.getInt());

//        EXPECT_TRUE(mt.hasDefaultValue());
    }

    TEST(MultiType, ConvertFloatToInt)
    {
        MultiType mt(1.234f);

        EXPECT_TRUE(mt.isType<float>());
        EXPECT_EQ(1.234f, mt.getFloat());
        EXPECT_EQ(1, mt.getInt());

        mt.convert<int>();

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_EQ(1.0f, mt.getFloat());
        EXPECT_EQ(1, mt.getInt());
    }

    TEST(MultiType, ConvertFloatToVector3)
    {
        MultiType mt(1.234f);

        EXPECT_TRUE(mt.isType<float>());
        EXPECT_EQ(1.234f, mt.getFloat());
        EXPECT_EQ(Vector3::ZERO, mt.getVector3());

        mt.convert<Vector3>();

        EXPECT_TRUE(mt.isType<Vector3>());
        EXPECT_EQ(0.0f, mt.getFloat());
        EXPECT_EQ(Vector3::ZERO, mt.getVector3());

//        EXPECT_TRUE(mt.hasDefaultValue());
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
        EXPECT_EQ(10, mt.getInt());

        mt.reset();

        EXPECT_FALSE(mt.isType<int>());
        EXPECT_TRUE(mt.isType<void>());
        EXPECT_TRUE(mt.null());
        EXPECT_EQ(0, mt.getInt());
    }

    ///////////////////
    // reset<type>() //
    ///////////////////
    TEST(MultiType, SetType)
    {
        MultiType mt(10);

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_FALSE(mt.isType<float>());
        EXPECT_EQ(10, mt.getInt());

        mt.reset<float>();

        EXPECT_FALSE(mt.isType<int>());
        EXPECT_TRUE(mt.isType<float>());
        EXPECT_EQ(0, mt.getInt());
    }

    ////////////////
    // resetValue //
    ////////////////
    TEST(MultiType, resetValue)
    {
        MultiType mt(10);

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_EQ(10, mt.getInt());

        mt.resetValue();

        EXPECT_TRUE(mt.isType<int>());
        EXPECT_EQ(0, mt.getInt());
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
    // getXXX() //
    //////////////
    TEST(MultiType, GetValueFromInt)
    {
        MultiType mt(256);

        EXPECT_EQ(0, mt.getChar());
        EXPECT_EQ(0u, mt.getUnsignedChar());
        EXPECT_EQ(256, mt.getShort());
        EXPECT_EQ(256u, mt.getUnsignedShort());
        EXPECT_EQ(256, mt.getInt());
        EXPECT_EQ(256u, mt.getUnsignedInt());
        EXPECT_EQ(256, mt.getLong());
        EXPECT_EQ(256u, mt.getUnsignedLong());
        EXPECT_EQ(256, mt.getLongLong());
        EXPECT_EQ(256u, mt.getUnsignedLongLong());
        EXPECT_EQ(256.0f, mt.getFloat());
        EXPECT_EQ(256.0, mt.getDouble());
        EXPECT_EQ(256.0, mt.getLongDouble());
        EXPECT_TRUE(mt.getBool());
        EXPECT_EQ("256", mt.getString());
    }

    TEST(MultiType, GetValueFromFloat)
    {
        MultiType mt(128.821);

        EXPECT_EQ(-128, mt.getChar());
        EXPECT_EQ(128u, mt.getUnsignedChar());
        EXPECT_EQ(128, mt.getShort());
        EXPECT_EQ(128u, mt.getUnsignedShort());
        EXPECT_EQ(128, mt.getInt());
        EXPECT_EQ(128u, mt.getUnsignedInt());
        EXPECT_EQ(128, mt.getLong());
        EXPECT_EQ(128u, mt.getUnsignedLong());
        EXPECT_EQ(128, mt.getLongLong());
        EXPECT_EQ(128u, mt.getUnsignedLongLong());
        EXPECT_EQ(128.821f, mt.getFloat());
        EXPECT_EQ(128.821, mt.getDouble());
        EXPECT_EQ(128.821, mt.getLongDouble());
        EXPECT_TRUE(mt.getBool());
        EXPECT_EQ("128.821", mt.getString());
    }

    TEST(MultiType, GetValueFromString)
    {
        MultiType mt("0.123");

        EXPECT_EQ('0', mt.getChar());
        EXPECT_EQ('0', mt.getUnsignedChar());
        EXPECT_EQ(0, mt.getShort());
        EXPECT_EQ(0u, mt.getUnsignedShort());
        EXPECT_EQ(0, mt.getInt());
        EXPECT_EQ(0u, mt.getUnsignedInt());
        EXPECT_EQ(0, mt.getLong());
        EXPECT_EQ(0u, mt.getUnsignedLong());
        EXPECT_EQ(0, mt.getLongLong());
        EXPECT_EQ(0u, mt.getUnsignedLongLong());
        EXPECT_EQ(0.123f, mt.getFloat());
        EXPECT_EQ(0.123, mt.getDouble());
        EXPECT_DOUBLE_EQ(0.123, mt.getLongDouble());
        EXPECT_FALSE(mt.getBool());
        EXPECT_EQ("0.123", mt.getString());
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

    ///////////////////////
    // hasDefaultValue() //
    ///////////////////////
    TEST(MultiType, HasDefaultValue)
    {
        MultiType mt;

//        EXPECT_FALSE(mt.hasDefaultValue());

        mt = 5.55;

        EXPECT_FALSE(mt.hasDefaultValue());

        mt.convert<int>();

        EXPECT_FALSE(mt.hasDefaultValue());

        mt.convert<Quaternion>();

//        EXPECT_TRUE(mt.hasDefaultValue());
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
