CXX=g++ # 使用 g++ 作为 C++ 编译器
CXXFLAGS=-Wall -std=c++11 # 编译器标志

# 获取所有的 .cpp 文件，并将它们转换成 .o 文件
OBJS=$(patsubst %cpp, %o, $(wildcard *.cpp))

# 定义 server 目标及其依赖的 .o 文件
server: $(OBJS)

# 编译 server 目标
server: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# 规则来编译单个 .cpp 文件为 .o 文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清除目标
clean:
	rm -f *.o server