#pragma once
#include <iostream>
#include <string>
#include <bitset>
using namespace std;

#define QINT_SIZE 16
#define BITS 8
#define HEX_CHARS "0123456789ABCDEF"

// Xây dựng:
// Chuỗi unsigned char 16 phần tử (16 bytes = 128 bits)
// usigned char[i] :	[0]			[1]			[2]			...			[14]		[15]	
// 127 bits        : XXXX XXXX	 XXXX XXXX	 XXXX XXXX	 	...		 XXXX XXXX	 XXXX XXXX
// bits[0]: bit trái nhất - bit dấu

class QInt
{
private:
	// Lưu dãy bits trên 16 bytes
	unsigned char _arrBits[QINT_SIZE];

public:
	QInt();
	~QInt();
public:
	// Số 0
	static QInt Zero();
	// Số 1
	static QInt One();

	// Get bit từ vị trí pos trái qua
	bool GetBit(int pos) const;
	// Set bit từ vị trí pos trái qua
	void SetBit(int pos, bool value);

	// Kiểm tra số âm
	bool IsNegative() const;
	// Lấy số đối
	QInt ToInverse() const;

	// Nhập theo cơ số
	void ScanQInt(string src, int base);
	// Xuất theo cơ số
	void PrintQInt(int base);

	// Chuyển cơ số Bin - Dec -Hex
	// Dec -> Bin
	friend QInt DecToBin(string src);
	// Bin -> Dec
	friend string BinToDec(const QInt& src);
	// Hex -> Bin
	friend QInt HexToBin(string src);
	// Bin -> Hex
	friend string BinToHex(const QInt& src);
	// Dec -> Hex
	friend string DecToHex(string src);
	// Hex -> Dec
	friend string HexToDec(string src);

	// Utils
	// Cộng 2 QInt -> dùng cho operator +, -
	friend QInt Plus(const QInt& A, const QInt& B);
	// Chia *this cho divisor ra thương div và số dư mod -> dùng cho operator /
	void Div(const QInt divisor, QInt& div, QInt& mod) const;

	// Các operator =, operator +, operator -, operator *, operator / trên các hệ cơ số
	QInt& operator= (const QInt& B);
	QInt operator+ (const QInt& B) const;
	QInt operator- (const QInt& B) const;
	QInt operator* (const QInt& B) const;
	QInt operator/ (const QInt& B) const;

	// Các toán tử AND "&", OR "|", XOR "^", NOT "~"
	QInt operator& (const QInt& B) const;
	QInt operator| (const QInt& B) const;
	QInt operator^ (const QInt& B) const;
	QInt operator~ () const;

	// Các toán tử : dịch trái " << ", dịch phải " >> " số học
	QInt operator>> (int num) const;
	QInt operator<< (int num) const;

	// Phép xoay trái "rol"
	QInt rol(int num) const;
	// Phép xoay phải "ror" 
	QInt ror(int num) const;

	// Operator ==, !=
	bool operator== (const QInt& B) const;
	bool operator!= (const QInt& B) const;

	// Chuỗi nhị phân không có số 0 ở đầu
	string ToBinStr() const;
	// Chuỗi QInt
	string ToQIntStr(int base) const;
};

QInt DecToBin(string src);
QInt HexToBin(string src);
string BinToDec(const QInt& src);
string BinToHex(const QInt& src);
QInt Plus(const QInt& A, const QInt& B);