#include "stdio.h"
#include "stdlib.h"
#include "dirent.h"
#include "pdflib.h"
#include "libxl.h"
#include "iostream"

#define N 35

struct Student_List//助教手上的学生名单（完整）
{
	long num[N];
	const char* name[N];
	const char* path[N];
}; 

int main(void)
{
	int i;
	Student_List StudentList_class4;
	/*————————————————
	创建新excel表，并写入数据
	————————————————*/
	/*
	BookHandle book = xlCreateBook();
	if (book)
	{
		SheetHandle sheet = xlBookAddSheet(book, "Sheet1", NULL);
		if (sheet)
		{
			xlSheetWriteStr(sheet, 2, 1, "Hello,World!", NULL);
			xlSheetWriteStr(sheet, 4, 1, "加油，你是最棒的!", NULL);
			xlSheetWriteNum(sheet, 3, 1, 1000, NULL);
		}
		xlBookSave(book, "example.xls");
		xlBookRelease(book);
	}
	*/
	/*————————————————
	导入路径已知，文件名已知的excel表，并读取数据
	————————————————*/
	/*
	BookHandle book1 = xlCreateBook();
	if (book1)
	{
		if (xlBookLoad(book1, "18级计科4班.xls"))
		{
			SheetHandle sheet1 = xlBookGetSheet(book1, 0);
			if (sheet1)
			{
				long d;
				const char* s = xlSheetReadStr(sheet1, 4, 1, NULL);
				if (s) printf("%s\n", s);
				d = (long)xlSheetReadNum(sheet1, 3, 0, NULL);
				printf("%ld\n", d);
			}
		}
		xlBookRelease(book1);
	}
	return 0;
	*/
	/*————————————————
	已知学生名单文件名，读取数据
	————————————————*/
	/*
	BookHandle book1 = xlCreateBook();
	if (book1)
	{
		if (xlBookLoad(book1, "18级计科4班.xls"))
		{
			SheetHandle sheet1 = xlBookGetSheet(book1, 0);
			if (sheet1)
			{
				for (i = 0; i < 28; i++)
				{
					StudentList_class4.name[i] = xlSheetReadStr(sheet1, i+2, 1, NULL);
//					if (StudentList_class4.name[i])
//					printf("%s\t", StudentList_class4.name[i]);
					StudentList_class4.num[i] = (long)xlSheetReadNum(sheet1, i+2, 0, NULL);
//					printf("%ld\n", StudentList_class4.num[i]);
				}
			}//循环读取每名学生的学号和姓名，读取文件数据，写入结构体中
		}
		xlBookRelease(book1);
	}
*/
//测试代码，可删
/*	printf("\n\n");
	for (i = 0; i < 28; i++)
	{
		printf("%s\t", StudentList_class4.name[i]);
		printf("%ld\n", StudentList_class4.num[i]);
	}
*/
	/*————————————————
	system（）函数，调用系统命令行，读取指定目录下的文件名
	————————————————*/
	
	return 0;
};
