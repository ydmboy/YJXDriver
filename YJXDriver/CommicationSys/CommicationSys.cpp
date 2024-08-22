#include <iostream>

class MyClass {
public:
	// 构造函数
	MyClass(int size) : size(size), data(new int[size]) {
		std::cout << "Constructor called\n";
	}

	// 移动构造函数
	MyClass(MyClass&& other) noexcept : size(other.size), data(other.data) {
		std::cout << "Move constructor called\n";

		// 转移资源所有权
		other.size = 0;
		other.data = nullptr;
	}
	MyClass(int x)
	{

	}

	// 析构函数
	~MyClass() {
		delete[] data;
	}

	// 打印内容
	void print() const {
		std::cout << "Size: " << size << "\n";
	}

private:
	int size;
	int* data;
};

class fClass
{
public:
	fClass();
	fClass(fClass& x) {
		m_x = x.m_x;
	}
private:
	int m_x;
};


typedef struct _ObjectTypeInfo {
    char* Name;           // 对象类型的名称
} ObjectTypeInfo;

int main() {
	_ObjectTypeInfo ObTypeINf;
	ObTypeINf.Name = "asd";
}


