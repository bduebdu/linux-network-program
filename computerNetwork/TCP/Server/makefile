# 自定义变量
# 变量赋值
OBJ = $(patsubst %.cpp, %.o, $(wildcard ./*.cpp))
TARGET = server

LDFLAGS = -L./src_so -L./dataBase_so -L./hashTable_so
LIBS = -lsqlite3 -ljsoncpp -lpthread
# 使用$(TARGET)，必须要加$符号
$(TARGET): $(OBJ)
	$(CXX) -g $^ -o $@ $(LIBS)

%.o: %.cpp
	$(CXX) -g -c $^ -o $@

.PHONY: clean

clean:
	@$(RM) $(OBJ) $(TARGET)

show:
	@echo $(RM)
	@echo $(CC)
	@echo $(wildcard ./*.c)
	@echo $(OBJ)
	@echo $(CXX)
