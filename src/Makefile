
CC:=clang
CXX:=clang++
CFLAGS:=-std=gnu11 -D_MONGOOSE
CXXFLAGS:=-std=c++14
INCLUDES:=-I. -I/usr/include/apr-1.0 -I/usr/include/json-c
LDFLAGS:=-lapr-1 -laprutil-1 -ljson-c -lssl -lcrypto

C_SRC:=mongoose.c cmicrotools.c main.c
CPP_SRC:=service.cpp

OBJ_C:=$(C_SRC:.c=.o)
OBJ_CPP:=$(CPP_SRC:.cpp=.o)

BUILD_DIR:=/usr/local/bin/
LOG_DIR:=/var/logs

all: $(BUILD_DIR)/cm

debug: CFLAGS += -g -D_DEBUG
debug: CXXFLAGS += -g -D_DEBUG
debug: $(BUILD_DIR)/cm

$(BUILD_DIR)/cm: $(OBJ_C) $(OBJ_CPP)
	@mkdir -p $(BUILD_DIR)
	$(CXX) -o $@ $^ $(LDFLAGS)
	@rm -f *.o

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run:
	mkdir -p $(LOG_DIR)
	$(BUILD_DIR)/cm -h 0.0.0.0 -p 2310 -P 2443 -l $(LOG_DIR)/cm.log

deb:
	chmod +x /app/package.sh
	/app/package.sh

clean:
	@rm -rf *.o

purge: clean
	rm -rf $(BUILD_DIR)/cm

.PHONY: all debug run clean purge
