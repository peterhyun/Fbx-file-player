LINK_TARGET = FBXLoader

OBJS = glad.o FBXModel.o main.o

REBUILDABLES = $(OBJS) $(LINK_TARGET)

INCLUDES = -I ./includes

CPP = g++

DIR = $(CURDIR)

all : $(LINK_TARGET)
	cp ./libraries/libfbxsdk.so /usr/lib
	cp ./libraries/libBulletDynamics.so.2.88 /usr/lib
	cp ./libraries/libBulletCollision.so.2.88 /usr/lib
	cp ./libraries/libLinearMath.so.2.88 /usr/lib
	echo All successfully linked!

$(LINK_TARGET) : $(OBJS)
	ln -sfn $(DIR)/libraries/libBulletCollision.so.2.88 $(DIR)/libraries/libBulletCollision.so
	ln -sfn $(DIR)/libraries/libBulletDynamics.so.2.88 $(DIR)/libraries/libBulletDynamics.so
	ln -sfn $(DIR)/libraries/libLinearMath.so.2.88 $(DIR)/libraries/libLinearMath.so
	ln -sfn $(DIR)/libraries/libX11.so.6.3.0 $(DIR)/libraries/libX11.so
	ln -sfn $(DIR)/libraries/libXxf86vm.so.1.0.0 $(DIR)/libraries/libXxf86vm.so
	ln -sfn $(DIR)/libraries/libXrandr.so.2.2.0 $(DIR)/libraries/libXrandr.so
	ln -sfn $(DIR)/libraries/libXi.so.6.1.0 $(DIR)/libraries/libXi.so
	ln -sfn $(DIR)/libraries/libdl-2.27.so $(DIR)/libraries/libdl.so
	ln -sfn $(DIR)/libraries/libXinerama.so.1.0.0 $(DIR)/libraries/libXinerama.so
	ln -sfn $(DIR)/libraries/libXcursor.so.1.0.2 $(DIR)/libraries/libXcursor.so
	


	$(CPP) -o $@ $^ -L ./libraries -lBulletCollision -lBulletDynamics -lLinearMath -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lfbxsdk -ldl -lXinerama -lXcursor $(INCLUDES)

main.o : main.cpp
	$(CPP) -c main.cpp $(INCLUDES)

glad.o : glad.c
	$(CPP) -c glad.c $(INCLUDES)

FBXModel.o : FBXModel.o
	$(CPP) -c FBXModel.cpp $(INCLUDES)

clean :
	rm -f $(REBUILDABLES)
	echo Clean Done!
