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
		cout << "�ļ����ܴ�" << endl;
	}
	else
	{
		// ����������ļ�
		cout << "Learning C++ is very useful." << endl;
		//FileInput << "Learning C++ is very useful." << endl;
		//�ر��ļ������
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