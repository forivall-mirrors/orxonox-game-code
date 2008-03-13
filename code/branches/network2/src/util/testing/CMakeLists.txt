INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES}
	../
)

ADD_EXECUTABLE(TestConverter
 TestConverter.cpp
)


ADD_TEST(TestConverter
	${EXECUTABLE_OUTPUT_PATH}/TestConverter
)
