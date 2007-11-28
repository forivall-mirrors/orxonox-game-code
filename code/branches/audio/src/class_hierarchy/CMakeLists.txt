PROJECT(Orxonox)

# create a few variables to simplify life
SET( CLASS_HIERARCHY_SRC
	BaseObject.cc
	ClassHierarchy.cc
	Identifier.cc
	IdentifierList.cc
	ObjectList.cc
	orxonox.cc
	OrxonoxClass.cc
	test1.cc
	test2.cc
	test3.cc
   )

ADD_LIBRARY(classHierarchy ${CLASS_HIERARCHY_SRC} )
