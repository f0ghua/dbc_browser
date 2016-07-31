
#CXX = c++
AR = ar

3rdParty_INC_DIR 	= ../3rdParty/include
3rdParty_LIB_DIR 	= ../3rdParty/lib
TARGET_LIB_DIR		= ../

ARCH = $(shell getconf LONG_BIT)
PLATFORM = $(shell uname)

#CXX_FLAGS 	= -g -O1 -DNDEBUG
CXX_FLAGS 	= -O2 -DNDEBUG

#ifneq ($(find_string MINGW, $(PLATFORM)),"")
ifeq (Linux, $(PLATFORM))
    # PLATFORM linux
    ifeq (32, $(ARCH))
      LD_LIBS += $(3rdParty_LIB_DIR)/libboost_regex_x86.a
    else
      LD_LIBS += $(3rdParty_LIB_DIR)/libboost_regex.a
    endif
else
	# platform windows mingw32
  ifeq (32, $(ARCH))
	# 32bit
    CXX_FLAGS += -march=i686
    LD_LIBS +=  $(3rdParty_LIB_DIR)/libboost_regex-mt.a
  else
    $(error 64bit mingw library has not defined!)
  endif
endif

CXX_FLAGS 	+= -std=c++11 $(CXX_INCS)

CXX_DEFINES = -DVector_DBC_EXPORTS -D_FORTIFY_SOURCE=2
CXX_INCS 	= -Iinc -I$(3rdParty_INC_DIR)

CXX_LD_FLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -Wl,-z,relro,-z,now
LD_DY_FLAGS = -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -O3 -DNDEBUG

dy_target = $(TARGET_LIB_DIR)/libdbc.so
st_target = $(TARGET_LIB_DIR)/libdbc.a
targets = $(dy_target) $(st_target)

objects = \
	src/Attribute.o \
	src/AttributeDefinition.o \
	src/AttributeRelation.o \
	src/BitTiming.o \
	src/EnvironmentVariable.o \
	src/ExtendedMultiplexor.o \
	src/File.o \
	src/FileLoad.o \
	src/FileSave.o \
	src/Message.o \
	src/Network.o \
	src/Node.o \
	src/Signal.o \
	src/SignalGroup.o \
	src/SignalType.o \
	src/ValueTable.o

all: $(st_target)

clean:
	rm -rf $(targets) $(objects)

$(objects): %.o: %.cpp
	$(CXX) $(CXX_FLAGS) $(CXX_DEFINES) -c $< -o $@

$(dy_target): $(objects)
	$(CXX) $(LD_FLAGS) -shared -Wl,-soname,$@ -o libVector_DBC.so $^

$(st_target): $(LD_LIBS) $(objects)
	$(AR) -rc $@ $^
