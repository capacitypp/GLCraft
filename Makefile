OBJS=Block.o BlockAir.o BlockCVIP.o BlockDirt.o BlockLeaf.o BlockManager.o BlockMosaic.o BlockWire.o BlockWood.o BuiltInImage.o DisplayListManager.o Image.o ItemBlock.o ItemSlot.o ItemStack.o Main.o Mouse.o Player.o TextureManager.o Timer.o World.o WorldBlock.o
OBJPATHS=$(addprefix $(BUILDPATH)/, $(OBJS))
CPPFLAGS=$(INCLUDE) -Wall -O2 -std=c++14 -MMD
LDFLAGS=-lglut -lGLU -lGL
BUILDPATH=./build
CXX=ccache clang++

all : makefolder $(OBJS)
	$(CXX) -o Main $(LDFLAGS) $(OBJS)

$(BUILDPATH)/%.o : %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -o $@ -c $<

makefolder :
	@mkdir -p $(BUILDPATH)

clean :
	$(RM) Main
	$(RM) -r -f $(BUILDPATH)

-include $(BUILDPATH)/*.d

