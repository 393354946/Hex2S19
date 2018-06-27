#include "iostream"  
#include "fstream" 
#include <iomanip>  
#include "FileManage.h"
using namespace std;
/*
int main(void)
{
	ifstream FileInput;
	string content;
	FileInput.open("Bootloader.hex", ios::in , 0);
	FileInput >> content;
	if (!FileInput)
	{
		cout << "文件不能打开" << endl;
	}
	else
	{
		// 输出到磁盘文件
		cout << "Learning C++ is very useful." << endl;
		//FileInput << "Learning C++ is very useful." << endl;
		//关闭文件输出流
		FileInput.close();
	}
	cout << "Learning C++ is very useful." << endl;
	system("pause");
}
*/
int main(void)
{
	//WriteLetter();
	//ReadFileByChar();
	File_Read();
}