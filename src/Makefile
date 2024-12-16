CC:=clang
CXX:=clang++
CFLAGS:=-std=gnu11 -D_MONGOOSE
CXXFLAGS:=-std=c++14
INCLUDES:=-I. -I /usr/include/apr-1.0 -I/usr/include/json-c
LDFLAGS:=-lapr-1 -laprutil-1 -ljson-c -lssl -lcrypto

C_SRC:=mongoose.c cmicrotools.c main.c
CPP_SRC:=service.cpp

OBJ_C:=$(C_SRC:.c=.o)
OBJ_CPP:=$(CPP_SRC:.cpp=.o)

DEBUG_DIR:=debug
RELEASE_DIR:=release

all: $(RELEASE_DIR)/cm

$(RELEASE_DIR)/cm: $(OBJ_C) $(OBJ_CPP)
	@mkdir -p ../$(RELEASE_DIR)
	$(CXX) -o ../$@ $^ $(LDFLAGS)
	@rm -f *.o

debug: CFLAGS += -g -D_DEBUG
debug: CXXFLAGS += -g -D_DEBUG
debug: $(DEBUG_DIR)/cm

$(DEBUG_DIR)/cm: $(OBJ_C) $(OBJ_CPP)
	@mkdir -p ../$(DEBUG_DIR)
	$(CXX) -o ../$@ $^ $(LDFLAGS)
	@rm -f *.o

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run:
	../$(DEBUG_DIR)/cm -h 0.0.0.0 -p 2310 -P 2443 -l cm.log

repo:
	@chmod +x repo.sh
	./repo.sh

clean:
	@rm -rf ../$(DEBUG_DIR) ../$(RELEASE_DIR)

.PHONY: all debug run repo clean
