
#CXX = c++
#CXX_FLAGS += -g -O1 -DNDEBUG -I ../inc
CXX_FLAGS += -O2 -DNDEBUG -I../inc
#
#CXX_FLAGS += -fvisibility=hidden -fvisibility-inlines-hidden -fPIC -Wl,-z,relro,-z,now
CXX_FLAGS += -std=c++11

ARCH = $(shell getconf LONG_BIT)
PLATFORM = $(shell uname)

LD_LIBS = ../../libdbc.a
#ifneq ($(find_string MINGW, $(PLATFORM)),"")
ifeq (Linux, $(PLATFORM))
    # PLATFORM linux
    ifeq (32, $(ARCH))
      LD_LIBS += ../../3rdParty/lib/libboost_regex_x86.a
    else
      LD_LIBS += ../../3rdParty/lib/libboost_regex.a
    endif
else
	# platform windows mingw32
  ifeq (32, $(ARCH))
	# 32bit
    CXX_FLAGS += -march=i686
    LD_LIBS +=  ../../3rdParty/lib/libboost_regex-mt.a
  else
    $(error 64bit mingw library has not defined!)
  endif
endif

target = main

all: $(target)

clean:
	rm -f $(target)

$(target): parserSample.cpp $(LD_LIBS)
	$(CXX) ${CXX_FLAGS} -o main parserSample.cpp -Wl,--start-group $(LD_LIBS) -Wl,--end-group

