GCC=g++ -std=c++17

ifeq  ($(OS), Windows_NT)
	LIBPATH = "-Llib/windows/"
	INCLUDEPATH = -Iinclude/windows/
	GCCFLAG = $(INCLUDEPATH) $(LIBPATH) -lassimp -lglfw3 -lgdi32 -lopengl32

else
	UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
	#LIBPATH = "-Llib/linux/"
	GCCFLAG = $(INCLUDEPATH) $(LIBPATH) -lassimp -lGL -lGLU -lglfw3  -lXxf86vm -lXrandr -lpthread -lXi -ldl -lX11

else # mac
	GCCFLAG = $(INCLUDEPATH) $(LIBPATH) -lassimp -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
endif

endif

INCLUDEPATH += -Iinclude/common/
INCLUDEPATH += -Isrc/header
INCLUDEPATH += -Isrc/render
INCLUDEPATH += -Isrc/camera
INCLUDEPATH += -Isrc/model
INCLUDEPATH += -Isrc/image
INCLUDEPATH += -Isrc/util
INCLUDEPATH += -Isrc/game
INCLUDEPATH += -Isrc/

SRC = src/main.cpp
SRC += src/glad.c

renderSRC = src/render/render.cpp
renderSRC += src/render/shader.cpp

modelSRC = src/model/model.cpp
modelSRC += src/model/rawModel.cpp

imageSRC = src/image/image.cpp

gameSRC = src/game/game.cpp
gameSRC += src/game/menu.cpp
gameSRC += src/game/play.cpp
gameSRC += src/game/ending.cpp

cameraSRC = src/camera/camera.cpp

utilSRC = src/util/loader.cpp

OUTBIN = ./bin/main

GameOBJS = game.o menu.o play.o ending.o
RenderOBJS = render.o shader.o
ModelOBJS = model.o rawModel.o

OBJS = camera.o util.o image.o $(GameOBJS) $(RenderOBJS) $(ModelOBJS)

all: $(OBJS)
	$(GCC) $(SRC) $^ -o $(OUTBIN) $(GCCFLAG)
	./bin/main

$(RenderOBJS): $(renderSRC)
	$(GCC) $(renderSRC) -c $(GCCFLAG)

$(ModelOBJS): $(modelSRC)
	$(GCC) $^ -c $(GCCFLAG)

$(GameOBJS): $(gameSRC)
	$(GCC) $(gameSRC) -c $(GCCFLAG)

util.o: $(utilSRC)
	$(GCC) $^ -c -o $@ $(GCCFLAG)

camera.o: $(cameraSRC)
	$(GCC) $^ -c -o $@ $(GCCFLAG)

image.o: $(imageSRC)
	$(GCC) $^ -c -o $@ $(GCCFLAG)

rm_clean:
	rm *.o

clean:
	del -f *.o
