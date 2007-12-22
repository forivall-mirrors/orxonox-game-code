#ifndef _Test3_H__
#define _Test3_H__

#include "BaseObject.h"

namespace orxonox
{
    class Test1;
    class Test2;

    class Test3 : public BaseObject
    {
        public:
            Test3();
            virtual ~Test3();

            void setConfigValues();

            void usefullClassesIsATest(Test1* test1);
            void usefullClassesIsATest(Test2* test2);

            void configOutput();

        private:
            int                 value_int_;
            unsigned int        value_uint_;
            char                value_char_;
            unsigned char       value_uchar_;
            float               value_float_;
            double              value_double_;
            bool                value_bool_;
            std::string         value_string_;
            const char*         value_constchar_;
            Vector2             value_vector2_;
            Vector3             value_vector3_;
            ColourValue         value_colourvalue_;
    };
}

#endif
