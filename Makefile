# 컴파일러 설정
CXX = g++
CXXFLAGS = -Wall -Iinc -I"C:/Program Files/mingw64/include"
LDFLAGS = -L"C:/Program Files/mingw64/lib" -lcurl

# 디렉토리 설정
SRCDIR = src
INCDIR = inc
BUILDDIR = build
TARGET = $(BUILDDIR)/main

# 소스 파일과 오브젝트 파일 설정
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))

# 기본 타겟
all: $(TARGET)

# 타겟을 어떻게 만드는지 정의
$(TARGET): $(OBJECTS)
	if not exist $(BUILDDIR) mkdir $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# 오브젝트 파일 만드는 방법 정의
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	if not exist $(BUILDDIR) mkdir $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	if exist $(BUILDDIR) rd /s /q $(BUILDDIR)

rebuild: clean all

# .PHONY 설정
.PHONY: all clean rebuild
