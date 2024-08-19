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

int main() {
	MyClass obj1(10);         // 调用普通构造函数
	MyClass obj2(std::move(obj1));  // 调用移动构造函数
	MyClass obj2 = MyClass(10);
	obj2.print();  // 打印 obj2 的内容
	// obj1 的资源已经被转移，data 现在为 nullptr，size 为 0
}


