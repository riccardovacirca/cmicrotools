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

BUILD_DIR:=/app/src/build
LOG_DIR:=/var/log

MAIN_BRANCH=main
DEVELOP_BRANCH=develop

all: $(BUILD_DIR)/cm

debug: CFLAGS += -g -D_DEBUG
debug: CXXFLAGS += -g -D_DEBUG
debug: $(BUILD_DIR)/cm

test: $(BUILD_DIR)/test/cm

$(BUILD_DIR)/cm: $(OBJ_C) $(OBJ_CPP)
	@mkdir -p $(BUILD_DIR)
	$(CXX) -o $@ $^ $(LDFLAGS)
	@rm -f *.o

$(BUILD_DIR)/test/cm: $(OBJ_C) $(OBJ_CPP)
	@mkdir -p $(BUILD_DIR)/test
	$(CXX) -o $@ $^ $(LDFLAGS)
	@rm -f *.o

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run:
	mkdir -p $(LOG_DIR)
	$(BUILD_DIR)/cm -h 0.0.0.0 -p 2310 -P 2443 -l $(LOG_DIR)/cm.log

merge:
	@git checkout $(MAIN_BRANCH) && \
	git pull origin $(MAIN_BRANCH) && \
	git merge $(DEVELOP_BRANCH) && \
	git status && \
	echo "Done."

deb:
	chmod +x /app/package.sh
	/app/package.sh

clean:
	@rm -rf *.o

purge: clean
	rm -rf $(BUILD_DIR)

.PHONY: all debug test run merge clean purge
