#define _CRT_SECURE_NO_WARNINGS
#include "QInt.h"
#include "Utils.h"

int main(int argc, char** argv)
{
	string inPath, outPath;

	if (argc < 3)
		cout << "Khong du 3 tham so dau vao";
	else
	{
		inPath = string(argv[1]); // input.txt
		outPath = string(argv[2]);// 18120397_18120408_output.txt

		vector<string> list_s;
		list_s = ReadFile(inPath);
		WriteFile(outPath, list_s);
	}

	return 0;
}

