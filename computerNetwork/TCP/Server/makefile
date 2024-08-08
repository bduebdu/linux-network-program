# 自定义变量
# 变量赋值
OBJ = $(patsubst %.cpp, %.o, $(wildcard ./*.cpp))
TARGET = server

LDFLAGS = -L./src_so -L./dataBase_so -L./hashTable_so
LIBS = -ljson-c -lsqlite3 -lSoMyTree -lSoMydataBase -lSoMyHash

# 使用$(TARGET)，必须要加$符号
$(TARGET): $(OBJ)
	$(CXX) -g $^ -o $@

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
