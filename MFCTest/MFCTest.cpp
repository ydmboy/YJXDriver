// MFCTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#define DEFINE_A L"abc"
#define DEFINE_B L"print"  DEFINE_A
#define DEFINE_ADEFINE_B "ddd"
#define CppConcat(x, y)			x##y
using namespace std;

int main()
{
	printf("%s\n",CppConcat(DEFINE_A,DEFINE_B));
	cout << DEFINE_B << endl;
	cout << CppConcat(DEFINE_A,DEFINE_B) << endl;

	//wprintf("%ws",DEFINE_B);
    //std::cout << "Hello World!\n";
}
