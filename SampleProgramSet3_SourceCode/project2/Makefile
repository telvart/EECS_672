CPP = g++ -std=c++11
INC = -I../cryphutil -I../fontutil -I../glslutil -I../mvcutil
C_FLAGS = -fPIC -g -c -DGL_GLEXT_PROTOTYPES $(INC)

LINK = g++ -fPIC -g
LOCAL_UTIL_LIBRARIES = ../lib/libcryph.so ../lib/libfont.so ../lib/libglsl.so ../lib/libmvc.so
GL_LIB_LOC = -L/usr/lib/nvidia-375
GL_LIBRARIES = $(GL_LIB_LOC) -lglfw -lGLU -lGL

OBJS = project2.o TEMPLATE_Subclass.o ModelView_Additions.o Tree.o Trunk.o TreeTop.o Plane.o House.o Pyramid.o PicnicTable.o

project2: $(OBJS) $(LOCAL_UTIL_LIBRARIES)
	$(LINK) -o project2 $(OBJS) $(LOCAL_UTIL_LIBRARIES) $(GL_LIBRARIES)

../lib/libcryph.so: ../cryphutil/AffVector.h ../cryphutil/AffVector.c++ ../cryphutil/AffPoint.h ../cryphutil/AffPoint.c++
	(cd ../cryphutil; make)

../lib/libfont.so: ../fontutil/CFont.h ../fontutil/CFont.c++ ../fontutil/CGLString.h ../fontutil/CGLString.c++
	(cd ../fontutil; make)

../lib/libglsl.so: ../glslutil/ShaderIF.h ../glslutil/ShaderIF.c++
	(cd ../glslutil; make)

../lib/libmvc.so: ../mvcutil/Controller.h ../mvcutil/Controller.c++ ../mvcutil/ModelView.h ../mvcutil/ModelView.c++
	(cd ../mvcutil; make)

project2.o: project2.c++
	$(CPP) $(C_FLAGS) project2.c++
ModelView_Additions.o: ModelView_Additions.c++
	$(CPP) $(C_FLAGS) ModelView_Additions.c++
TEMPLATE_Subclass.o: TEMPLATE_Subclass.h TEMPLATE_Subclass.c++
	$(CPP) $(C_FLAGS) TEMPLATE_Subclass.c++
Tree.o: Tree.c++
	$(CPP) $(C_FLAGS) Tree.c++
Trunk.o: Trunk.c++
	$(CPP) $(C_FLAGS) Trunk.c++
TreeTop.o: TreeTop.c++
	$(CPP) $(C_FLAGS) TreeTop.c++
Plane.o: Plane.c++
	$(CPP) $(C_FLAGS) Plane.c++
House.o: House.c++
	$(CPP) $(C_FLAGS) House.c++
Pyramid.o: Pyramid.c++
	$(CPP) $(C_FLAGS) Pyramid.c++
PicnicTable.o: PicnicTable.c++
	$(CPP) $(C_FLAGS) PicnicTable.c++


clean:
	rm *.o project2
