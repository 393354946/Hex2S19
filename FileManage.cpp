#include "iostream"  
#include "fstream" 
#include <iomanip>  
#include "FileManage.h"
#include "tools.h"

using namespace std;
string content;
string Updated_Content;
Format_HexFile  InputFormat;

/* This function just a example */
void WriteLetter(void)
{
	ofstream inFile;
	/*ios::trunc表示在打开文件前将文件清空,由于是写入,文件不存在则创建*/
	inFile.open("inFile.txt", ios::trunc | ios::in);

	int i;
	char a = 'a';
	for (i = 1; i <= 26; i++)//将26个数字及英文字母写入文件  
	{
		inFile << setw(2) << i << "\t" << a << "\n";
		a++;
		cout << "Learning C++ is very useful." << endl;
		cout << a << endl;
	}
	inFile.close();//关闭文件  
	system("pause");
}

void ReadFileByChar(void)
{
	fstream InputFile;
	char c;
	InputFile.open("Bootloader.hex", ios::in);
	while (!InputFile.eof())
	{
		InputFile >> c;
		cout << c;
	}
	InputFile.close();
}

void File_Read(void)
{
	ifstream InputFile;
	unsigned char aucLength[2],aucAddress[4],aucType[2],aucData[32];
	unsigned char aucTempLenth[1],aucTempAddress[2],aucTempType[1],aucTempData[16];
	char ch;
	int ret;
	unsigned int uidex;
	unsigned int i;
	
	/*line inputted from file*/
	char *p_line = NULL;
	/*记录行数*/
	unsigned int record_line = 0;

	/*line content*/
	unsigned int data_length;
	unsigned int address;
	unsigned int type;
	char *p_data = NULL;

	/*open the file*/
	InputFile.open("F:\\VC++\\HexToS19\\Hex2S19\\Hex2S19\\Bootloader.hex", ios::in);

	if (!InputFile)
	{
		cout << "文件读错误";
		system("pause");
		exit(1);
	}

	while(InputFile.get(ch))
	{
		content += ch;
		/*if (ch != '\n')
		{
			cout.put(ch);
		}*/
		cout.put(ch);//cout<<ch;这么写也是可以的  
	}

	for (uidex = 0; uidex < content.length(); uidex++)
	{
		memset(aucTempData, 0x00, 16);
		memset(aucData, 0x00, 32);
		if (content[uidex] == ':')
		{
			/*Get the data length*/
			memcpy(&aucLength, &content[uidex+1],2);
			CONV_AscHex(aucTempLenth, 1, aucLength, 2);
			InputFormat.uiDataSize = aucTempLenth[0];

			/*Get the start address*/
			memcpy(&aucAddress, &content[uidex + 3], 4);
			CONV_AscHex(aucTempAddress, 2, aucAddress, 4);
			InputFormat.uiStartAddress = (unsigned short)aucTempAddress[1] + ((unsigned short)aucTempAddress[0] << 8);

			/*Get the data type*/
			memcpy(&aucType, &content[uidex + 7], 2);
			CONV_AscHex(aucTempType, 1, aucType, 2);
			InputFormat.ucDataType = aucTempType[0];
			
			if(InputFormat.ucDataType == 0x04)
			{
				/*Get the data */
				memcpy(&aucData, &content[uidex + 9], 4);
				CONV_AscHex(aucTempData, 2, aucData, 4);
				InputFormat.pucData = aucTempData;
			}
			else if (InputFormat.ucDataType == 0x00)
			{
				/*Get the data */
				memcpy(&aucData, &content[uidex + 9], 32);
				CONV_AscHex(aucTempData, 16, aucData, 32);
				InputFormat.pucData = aucTempData;
			}
			
			
		}
		/*if (InputFormat.ucDataType == 0x00)
		{
			//memcpy(&Updated_Content[uidex], &content[uidex + 8], InputFormat.uiDataSize);
			Updated_Content[uidex] = content[uidex + 8];
		}*/
		cout << InputFormat.uiDataSize << InputFormat.uiStartAddress << InputFormat.ucDataType << endl;
	}

	cout << content.length();
	if (InputFile.eof())
	{
		cout << "文件内容已经全部读完" << endl;
	}
	InputFile.close();
	system("pause");
}

void get_line(char* str, FILE *in)
{
	char *result;
	result = fgets(str, MAX_LINE_SIZE, in);
	if ((NULL == result) && !feof(in))
		printf("Error occurred while reading from file\n");
}