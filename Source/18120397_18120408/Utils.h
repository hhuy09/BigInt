#pragma once
#include "QInt.h"
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

// Xử lí dòng dữ liệu trong file
string FileProcess(string inString);

// Đọc từng dòng dữ liệu vào vector list kiểu chuỗi
vector<string> ReadFile(string file_name_in);

// Viết từng dòng kết quả
void WriteFile(string file_name_out, vector<string>& list);

