SHELL       = /bin/sh

VARIANT      = release
ifeq "$(VARIANT)" "debug"
    GCC_DEBUG_FLAGS += -g -D_DEBUG
    SUFFIX = d
endif
STATIC = 

EXAMPLE     = frost
BINDIR      = $(PWD)
OBJDIR      = $(PWD)
ASSDIR      = $(PWD)/assets
LIBDIR      = /usr/lib/gcc4/x86
LIBFBXVERSION = 2012.1
INCDIR      = /usr/include -I/usr/include/kbaselib -I/usr/local/include/collada-dom -I/usr/local/include/collada-dom/1.4

TARGET      = $(BINDIR)/$(EXAMPLE)
OBJS        =  \
	window.o \
	userio.o \
	assets.o \
	mesh.o \
	fbxmesh.o \
	meshfactory.o \
	modelfactory.o \
	texture.o \
	polymesh.o \
	fbxparser.o \
	camera.o \
	model.o \
	renderer.o \
	client.o \
	targa.o \
	texturemanager.o \
	parseXML.o \
   collada.o \
	handleChars.o \
	animate.o \
	joint.o \
	skeleton.o \
	clock.o \
	lsystem.o \
	sound.o \
	zone.o \
	world.o \
	wob.o \
	mob.o \
	sceneobj.o \
	projectile.o \
	particlesystem.o \
	particle.o \
	particlefactory.o \
	particleeffects.o \
	player.o \
	hud.o \
	main.o

OBJSFINAL   =  \
	$(OBJDIR)/window.o \
	$(OBJDIR)/userio.o \
	$(OBJDIR)/assets.o \
	$(OBJDIR)/mesh.o \
	$(OBJDIR)/fbxmesh.o \
	$(OBJDIR)/meshfactory.o \
	$(OBJDIR)/modelfactory.o \
	$(OBJDIR)/texture.o \
	$(OBJDIR)/polymesh.o \
	$(OBJDIR)/fbxparser.o \
	$(OBJDIR)/camera.o \
	$(OBJDIR)/model.o \
	$(OBJDIR)/renderer.o \
	$(OBJDIR)/client.o \
	$(OBJDIR)/targa.o \
	$(OBJDIR)/texturemanager.o \
	$(OBJDIR)/parseXML.o \
   $(OBJDIR)/collada.o \
	$(OBJDIR)/handleChars.o \
	$(OBJDIR)/animate.o \
	$(OBJDIR)/joint.o \
	$(OBJDIR)/skeleton.o \
	$(OBJDIR)/clock.o \
	$(OBJDIR)/lsystem.o \
	$(OBJDIR)/sound.o \
	$(OBJDIR)/zone.o \
	$(OBJDIR)/world.o \
	$(OBJDIR)/wob.o \
	$(OBJDIR)/mob.o \
	$(OBJDIR)/sceneobj.o \
	$(OBJDIR)/projectile.o \
	$(OBJDIR)/particlesystem.o \
	$(OBJDIR)/particle.o \
	$(OBJDIR)/particlefactory.o \
	$(OBJDIR)/particleeffects.o \
	$(OBJDIR)/player.o \
	$(OBJDIR)/hud.o \
	$(OBJDIR)/main.o

LIBS        = -lxml2 -lpcre -lpcrecpp -lboost_filesystem -lcollada14dom -lSDL_ttf -lSDL_image -lSDL_gfx -lSDL_mixer -lGL -lGLU -lm -lexpat -lrt -luuid -lstdc++ -lpthread -ldl -lfbxsdk-$(LIBFBXVERSION)$(STATIC)$(SUFFIX)

CC          = g++
LD          = g++ 
RM          = /bin/rm -f

CXXFLAGS    = -g -Wall -m32 -O3 `sdl-config --cflags` `Magick++-config --ldflags --libs` -DFBXSDK_SHARED
LDFLAGS     = -m32 `sdl-config --libs` `Magick++-config --ldflags --libs` $(GCC_LINK_FLAGS)

.SUFFIXES: .cpp

all: $(TARGET)

.cpp.o:
	$(CC) $(CXXFLAGS) -I$(INCDIR) -c $< -o $*.o


$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJSFINAL) -L$(LIBDIR) $(LIBS) -Wl,-rpath $(LIBDIR)

clean:
	rm -rf *.o
	rm frost

