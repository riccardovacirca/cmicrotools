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
TEST_DIR:=test

all: $(RELEASE_DIR)/cm

$(RELEASE_DIR)/cm: $(OBJ_C) $(OBJ_CPP)
	@mkdir -p ../$(RELEASE_DIR)
	$(CXX) -o ../$@ $^ $(LDFLAGS)
	@rm -f *.o

debug: CFLAGS += -g -D_DEBUG
debug: CXXFLAGS += -g -D_DEBUG
debug: $(DEBUG_DIR)/cm

test: CFLAGS = -std=gnu11 -g -D_DEBUG
test: CXXFLAGS += -g -D_DEBUG
test: $(TEST_DIR)/cm

$(DEBUG_DIR)/cm: $(OBJ_C) $(OBJ_CPP)
	@mkdir -p /service/$(DEBUG_DIR)
	$(CXX) -o /service/$@ $^ $(LDFLAGS)
	@rm -f *.o

$(TEST_DIR)/cm: $(OBJ_C) $(OBJ_CPP)
	@mkdir -p /service/$(TEST_DIR)
	$(CXX) -o /service/$@ $^ $(LDFLAGS)
	@rm -f *.o

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run:
	mkdir -p /service/logs
	/service/$(DEBUG_DIR)/cm -h 0.0.0.0 -p 2310 -P 2443 -l /service/logs/cm.log

run-test:
	mkdir -p /service/logs
	/service/$(TEST_DIR)/cm -h 0.0.0.0 -p 2310 -P 2443 -l /service/logs/cm.log

clean:
	@rm -rf ../$(DEBUG_DIR) ../$(RELEASE_DIR)

.PHONY: all debug run run-test repo clean
