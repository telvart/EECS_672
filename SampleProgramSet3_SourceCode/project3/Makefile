CPP = g++ -std=c++11
INC = -I../cryphutil -I../fontutil -I../glslutil -I../imageutil -I../mvcutil
C_FLAGS = -fPIC -g -c -DGL_GLEXT_PROTOTYPES $(INC)

LINK = g++ -fPIC -g
LOCAL_UTIL_LIBRARIES = ../lib/libcryph.so ../lib/libfont.so ../lib/libglsl.so ../lib/libimage.so ../lib/libmvc.so
GL_LIB_LOC = -L/usr/lib/nvidia-375
GL_LIBRARIES = $(GL_LIB_LOC) -lglfw -lGLU -lGL

OBJS = project3.o ModelView_Additions.o PhongMaterial.o SceneElement.o Tree.o Trunk.o TreeTop.o Block.o House.o Pyramid.o PicnicTable.o Campfire.o

project3: $(OBJS) $(LOCAL_UTIL_LIBRARIES)
	$(LINK) -o project3 $(OBJS) $(LOCAL_UTIL_LIBRARIES) $(GL_LIBRARIES)

../lib/libcryph.so: ../cryphutil/AffVector.h ../cryphutil/AffVector.c++ ../cryphutil/AffPoint.h ../cryphutil/AffPoint.c++
	(cd ../cryphutil; make)

../lib/libfont.so: ../fontutil/CFont.h ../fontutil/CFont.c++ ../fontutil/CGLString.h ../fontutil/CGLString.c++
	(cd ../fontutil; make)

../lib/libglsl.so: ../glslutil/ShaderIF.h ../glslutil/ShaderIF.c++
	(cd ../glslutil; make)

../lib/libimage.so: ../imageutil/ImageReader.h ../imageutil/ImageReader.c++
	(cd ../imageutil; make)

../lib/libmvc.so: ../mvcutil/Controller.h ../mvcutil/Controller.c++ ../mvcutil/ModelView.h ../mvcutil/ModelView.c++
	(cd ../mvcutil; make)

project3.o: project3.c++
	$(CPP) $(C_FLAGS) project3.c++
ModelView_Additions.o: ModelView_Additions.c++
	$(CPP) $(C_FLAGS) ModelView_Additions.c++
PhongMaterial.o: PhongMaterial.c++
	$(CPP) $(C_FLAGS) PhongMaterial.c++
SceneElement.o: SceneElement.h SceneElement.c++
	$(CPP) $(C_FLAGS) SceneElement.c++
Tree.o: Tree.c++
	$(CPP) $(C_FLAGS) Tree.c++
Trunk.o: Trunk.c++
	$(CPP) $(C_FLAGS) Trunk.c++
TreeTop.o: TreeTop.c++
	$(CPP) $(C_FLAGS) TreeTop.c++
Block.o: Block.c++
	$(CPP) $(C_FLAGS) Block.c++
House.o: House.c++
	$(CPP) $(C_FLAGS) House.c++
Pyramid.o: Pyramid.c++
	$(CPP) $(C_FLAGS) Pyramid.c++
PicnicTable.o: PicnicTable.c++
	$(CPP) $(C_FLAGS) PicnicTable.c++
Campfire.o: Campfire.c++
	$(CPP) $(C_FLAGS) Campfire.c++
clean:
	rm *.o project3
