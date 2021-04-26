#include "Utils.h"

string FileProcess(string inString)
{
	// Toán tử một ngôi
	vector<string> unary_operator = { "~", "ror", "rol" };
	// Toán tử hai ngôi
	vector<string> binary_operator = { "+"," - ","*","/","<",">","<=",">=","==","&","|","^","<<",">>","rol","ror" };

	unsigned int i = 0, j = 0;
	int type_operator = 0; // Loại toán tử

	// Tìm toán tử một ngôi trong chuỗi
	while (j < unary_operator.size())
	{
		if (inString.find(unary_operator[j], 0) != string::npos)
		{
			type_operator = 1;
		}
		j++;
	}
	// Nếu không tìm thấy toán tử một ngôi, type_operator vẫn mang giá trị 0
	// Tiếp tục tìm toán tử hai ngôi
	if (type_operator == 0)
	{
		while (i < binary_operator.size())
		{
			if (inString.find(binary_operator[i], 0) != string::npos)
			{
				type_operator = 2;
				break;
			}
			i++;
		}
	}

	// Nếu không tìm thấy, trong chuỗi không chứa toán tử
	string p1, p2;

	// Cắt các tham số trong chuỗi
	string SEPERATOR = " ";
	vector<string> tokens;

	size_t start_pos = 0; // Vị trí bắt đầu tìm

	size_t pos = inString.find(SEPERATOR); // Thử tìm vị trí đầu tiên

	while (pos != string::npos) // Tìm thấy
	{
		tokens.push_back(inString.substr(start_pos, pos - start_pos));

		// Cập nhật vị trí bắt đầu tìm lại
		start_pos = pos + SEPERATOR.length();
		pos = inString.find(SEPERATOR, start_pos);
	}

	if (start_pos < inString.length())
		tokens.push_back(inString.substr(start_pos, inString.length() - start_pos));

	int n = tokens.size();
	p1 = tokens[0];

	// Kiểm tra trong chuỗi inString có chứa chỉ thị p2 không
	if (n - type_operator == 3)
		p2 = tokens[1];
	else
		p2 = p1;

	string opr = "";
	string pA, pB;
	switch (type_operator)
	{
	case 0:// Không có toán tử, chứa chỉ thị p2
		pA = tokens[2];
		break;
	case 1: // Toán tử 1 ngôi
		opr = tokens[1];
		pA = tokens[2];
		break;
	case 2: // Toán tử hai ngôi
		pA = tokens[1];
		opr = tokens[2];
		pB = tokens[3];
		break;
	}

	QInt A, B, res;
	int directive1 = stoi(p1);
	int directive2 = stoi(p2);
	try
	{
		A.ScanQInt(pA, directive1);
		if (opr == "")
		{
			string kt = A.ToQIntStr(directive2);
			int post(0);
			for (int i = 0; i < (int)kt.length(); i++)
			{
				if (kt[i] != '0')
				{
					post = i;
					break;
				}
			}
			kt.erase(0, post);
			return kt;
		}
		else if (opr == "~")
		{
			res = ~A;
		}
		else if (opr == "+")
		{
			B.ScanQInt(pB, directive1);
			res = A + B;
		}
		else if (opr == "-")
		{
			B.ScanQInt(pB, directive1);
			res = A - B;
		}
		else if (opr == "*")
		{
			B.ScanQInt(pB, directive1);
			res = A * B;

		}
		else if (opr == "/")
		{
			B.ScanQInt(pB, directive1);
			res = A / B;
		}
		else if (opr == "&")
		{
			B.ScanQInt(pB, directive1);
			res = A & B;
		}
		else if (opr == "|")
		{
			B.ScanQInt(pB, directive1);
			res = A | B;
		}
		else if (opr == "^")
		{
			B.ScanQInt(pB, directive1);
			res = A ^ B;
		}
		else if (opr == "<<")
		{
			int n = stoi(pB);
			res = A << n;
		}
		else if (opr == ">>")
		{
			int n = stoi(pB);
			res = A >> n;
		}
		else if (opr == "rol")
		{
			res = A.rol(1);
		}
		else if (opr == "ror")
		{
			res = A.ror(1);
		}
	}
	catch (const char* msg)
	{
		cout << msg;
	}
	string k = res.ToQIntStr(directive2);
	int position(0);
	for (int i = 0; i < (int)k.length(); i++)
	{
		if (k[i] != '0')
		{
			position = i;
			break;
		}
	}
	k.erase(0, position);
	return k;

}


// Đọc từng dòng dữ liệu vào vector list kiểu chuỗi
vector<string> ReadFile(string file_name_in)
{
	ifstream fin;
	vector<string> list;

	//Mở file
	fin.open(file_name_in, ios_base::in);

	if (fin.fail() == true)
		cout << "File is inavailable!\n";
	else
	{
		// Đọc từng dòng lệnh
		while (!fin.eof())
		{
			string line;
			getline(fin, line);
			list.push_back(line);
		}
	}
	fin.close();
	return list;
}

void WriteFile(string file_name_out, vector<string>& list)
{
	ofstream fout;
	fout.open(file_name_out, ios_base::out);

	if (fout.fail() == true)
		cout << "File is inavailable!" << endl;
	else
	{
		for (int i = 0; i < (int)list.size() - 1; i++)
		{
			fout << FileProcess(list[i]) << endl;
		}
	}

	fout.close();
}