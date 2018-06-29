#include "iostream"  
#include "fstream" 
#include <iomanip>  
#include "FileManage.h"
#include "tools.h"
#include<stdio.h>
#include<string.h>

using namespace std;
string content;
string Data_string;
unsigned char Updated_Content[102400];
Format_HexFile  InputFormat;
unsigned int uiCounter = 0;
unsigned int data_length;
/*线性基地址*/
unsigned int Based_Address;
unsigned int uiIndex= 0;

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
	ofstream OutputFile;
	
	unsigned char aucLength[2],aucAddress[4],aucType[2],aucData[64];
	unsigned char aucTempLenth[1],aucTempAddress[2],aucTempType[1],aucTempData[16];
	char ch;
	unsigned int uidex;
	
	/*line inputted from file*/
	char *p_line = NULL;
	/*记录行数*/
	unsigned int record_line = 0;

	/*line content*/
	char *p_data = NULL;

	/*open the file*/
	InputFile.open("F:\\VC++\\HexToS19\\Hex2S19\\Hex2S19\\Bootloader.hex", ios::in);
	if (!InputFile)
	{
		cout << "文件读错误";
		system("pause");
		exit(1);
	}

	OutputFile.open("F:\\VC++\\HexToS19\\Hex2S19\\Hex2S19\\out.txt", ios::out | ios::app);
	if (!OutputFile)//或者写成myfile.fail()  
	{
		cout << "文件创建失败,磁盘不可写或者文件为只读!";
		system("pause");
		exit(1);
	}

	while(InputFile.get(ch))
	{
		content += ch;
		cout.put(ch);//cout<<ch;这么写也是可以的  
	}

	for (uidex = 0; uidex < content.length(); uidex++)
	{
		memset(aucTempData, 0x00, 16);
		memset(aucData, 0x00, 64);

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
				memcpy(&aucData, &content[uidex + 9], InputFormat.uiDataSize*2);
				CONV_AscHex(aucTempData, InputFormat.uiDataSize, aucData, InputFormat.uiDataSize*2);
				InputFormat.pucData = aucTempData;
				/*基地址*/
				Based_Address = (((unsigned short)aucTempData[0] << 8) + (unsigned short)aucTempData[1]) << 16;
			}
			if (InputFormat.ucDataType == 0x05)
			{
				/*Get the data */
				memcpy(&aucData, &content[uidex + 9], InputFormat.uiDataSize*2);
				CONV_AscHex(aucTempData, InputFormat.uiDataSize, aucData, InputFormat.uiDataSize*2);
				InputFormat.pucData = aucTempData;
			}
			else if (InputFormat.ucDataType == 0x00)
			{
				/*Get the data */
				memcpy(&aucData, &content[uidex + 9], InputFormat.uiDataSize*2);
				CONV_AscHex(aucTempData, InputFormat.uiDataSize, aucData, InputFormat.uiDataSize*2);
				InputFormat.pucData = aucTempData;

				if (InputFormat.uiDataSize == 0x10)
				{
					memcpy(&Updated_Content[uiCounter*InputFormat.uiDataSize], aucTempData, InputFormat.uiDataSize);
				}
				else
				{
					memcpy(&Updated_Content[uiCounter*0x10], aucTempData, InputFormat.uiDataSize);
					data_length = uiCounter * 0x10 + InputFormat.uiDataSize;
				}
				uiCounter++;
				Hex_to_S19(InputFormat.uiDataSize,aucData);
				OutputFile << aucData << endl;
			}
			else if (InputFormat.ucDataType == 0x01)
			{
				uiCounter = 0;
				if ((InputFormat.uiDataSize == 0x00) && (InputFormat.uiStartAddress == 0x0000))
				{
					OutputFile.close();
				}
			}
		}
	}
	/*输入的文件长度*/
	cout <<"输入的文件长度:" << content.length() << endl;
	/*除了类型、长度、地址和校验和之外的所有的数据长度*/
	cout <<"数据长度"<< data_length << endl;

	if (InputFile.eof())
	{
		cout << "文件内容已经全部读完" << endl;
	}
	InputFile.close();
	
	system("pause");
}

void Hex_to_S19(unsigned int Length, unsigned char *pData)
{
	unsigned int uiAddress;
	unsigned char ucIndex, ucLength, *pTempData, ucCheckSum, ucSumOfAddress, ucSumOfData;
	unsigned char aucS19Data[32],aucTempAddress[4], aucTempLenth[32], aucAddBuff[32],aucTempCheckSum[32];
	string DataType = "S3";
	
	pTempData = pData;
	ucSumOfData = 0x00;
	ofstream S19File;

	S19File.open("F:\\VC++\\HexToS19\\Hex2S19\\Hex2S19\\S19File.s19", ios::out | ios::app);
	if (!S19File)//或者写成myfile.fail()  
	{
		cout << "文件创建失败,磁盘不可写或者文件为只读!";
		system("pause");
		exit(1);
	}

	memset(aucTempLenth,0x00,32);
	memset(aucAddBuff, 0x00, 32);
	memset(aucS19Data, 0x00, 32);
	memset(aucTempCheckSum, 0x00, 32);

	ucLength = Length + 0x05;
	/* 0x25-->"25" */
	CONV_HexAsc(aucTempLenth, &ucLength, 2);

	/*转换地址数据十六进制数到ASCII码*/
	uiAddress = Based_Address + uiIndex * 16;
	aucTempAddress[0] = (unsigned char)(uiAddress >> 24);
	aucTempAddress[1] = (unsigned char)(uiAddress >> 16);
	aucTempAddress[2] = (unsigned char)(uiAddress >> 8);
	aucTempAddress[3] = (unsigned char)uiAddress;
	CONV_HexAsc(aucAddBuff, aucTempAddress, 8);

	/*计算地址数据和*/
	ucSumOfAddress = aucTempAddress[0] + aucTempAddress[1] + aucTempAddress[2] + aucTempAddress[3];

	/*转换数据ASCII码到十六进制数*/
	CONV_AscHex(aucS19Data,16, pTempData,32);
	/*计算数据和*/
	for (ucIndex=0x00; ucIndex<0x10; ucIndex++)
	{
		ucSumOfData += aucS19Data[ucIndex];
	}
	/*计算校验和*/
	ucCheckSum = 0xFF - (ucSumOfAddress + ucSumOfData);
	CONV_HexAsc(aucTempCheckSum, &ucCheckSum, 2);

	uiIndex++;
	/*将转换后的数据写入S19文件*/
	S19File << "S3" << aucTempLenth << aucAddBuff << pTempData << aucTempCheckSum<<endl;

}
void get_line(char* str, FILE *in)
{
	char *result;
	result = fgets(str, MAX_LINE_SIZE, in);
	if ((NULL == result) && !feof(in))
		printf("Error occurred while reading from file\n");
}