#define MAX_LINE_SIZE 64

typedef struct
{
	unsigned char uiDataSize;
	unsigned short uiStartAddress;
	unsigned char ucDataType;
	unsigned char *pucData;
	FILE *file_in, *file_out;
}Format_HexFile;

void get_line(char* str, FILE *in);

extern void WriteLetter(void);
extern void ReadFileByChar(void);
extern void File_Read(void);