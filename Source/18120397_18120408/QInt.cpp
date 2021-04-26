#include "QInt.h"


QInt::QInt()
{
	// Điền các bit trong 16 bytes bằng bit 0
	fill(_arrBits, _arrBits + QINT_SIZE, 0);
}

QInt::~QInt()
{
}

QInt QInt::Zero()
{
	static QInt zero;
	static bool init = false; // Khởi tạo
	if (!init)
	{
		fill(zero._arrBits, zero._arrBits + QINT_SIZE, 0); // 0000 0000 ... 0000 0000
		init = true; // Khởi tạo thành công
	}

	return zero;
}

QInt QInt::One()
{
	static QInt one;
	static bool init = false; // Khởi tạo
	if (!init)
	{
		fill(one._arrBits, one._arrBits + QINT_SIZE, 0); // 0000 0000 ... 0000 0000
		init = true; // Khởi tạo thành công
	}
	one._arrBits[QINT_SIZE - 1] = 1; // Sửa phần tử cuối thành 1 => 0000 0000 ... 0000 0001

	return one;
}

bool QInt::GetBit(int pos) const
{
	int bytePos = pos / BITS; // Vị trí byte mà pos nằm trong đó (trái qua)
	int bitPos = pos % BITS; // Vị trí pos nằm trong byte (trái qua)
	return ((this->_arrBits[bytePos] >> (BITS - 1 - bitPos)) & 1); // Dịch bit để lấy bit tại pos
}

void QInt::SetBit(int pos, bool val)
{
	int bytePos = pos / BITS; // Vị trí byte mà pos nằm trong đó (trái qua)
	int bitPos = pos % BITS; // Vị trí pos nằm trong byte (trái qua)
	if (val) // true
	{
		this->_arrBits[bytePos] = ((1 << (BITS - 1 - bitPos)) | this->_arrBits[bytePos]); // set bit 1 vào vị trí pos
	}
	else // false
	{
		this->_arrBits[bytePos] = ((~(1 << (BITS - 1 - bitPos))) & this->_arrBits[bytePos]); // set bit 0 vào vị trí pos
	}
}

void QInt::ScanQInt(string qInt, int base)
{
	int bitPos = QINT_SIZE * BITS - 1; // Bit phải nhất
	switch (base)
	{
	case 2:
		*this = QInt::Zero();
		for (int i = qInt.length() - 1; i >= 0; i--)
		{
			SetBit(bitPos, qInt[i] == '1'); // Nếu qInt[i] == '1' là true thì set bit 1 vào vị trí đó
			bitPos--;
		}
		break;
	case 10:
		*this = DecToBin(qInt); // Chuyển chuỗi qInt từ Dec sang Bin lưu vào _arrBits
		break;
	case 16:
		*this = HexToBin(qInt); // Chuyển chuỗi qInt từ Hex sang Bin lưu vào _arrBits
	}
}

void QInt::PrintQInt(int base)
{
	cout << ToQIntStr(base); // Được cài đặt phía dưới
}

bool QInt::IsNegative() const
{
	return ((this->GetBit(0)) == 1);
	// Bit trái nhất _arrBits[0] là bit dấu
	// Nếu GetBit(0)) == 1 là true thì là số âm, ngược lại là dương
}

QInt QInt::ToInverse() const
{
	// Trường hợp số 0
	if (*this == QInt::Zero())
		return *this;

	QInt res;
	// Đảo bit
	res = ~(*this);

	// Cộng 1
	for (int i = QINT_SIZE - 1; i >= 0; i--)
	{
		if (res._arrBits[i] < 255) // 255: 1111 1111
		{
			res._arrBits[i] += 1;
			break;
		}
		else
			res._arrBits[i] = 0;
	}

	return res;
}

// Hàm Hỗ trợ BinToDec
string mulByTwo(string src, int additive)
{
	string res = "";
	int mind = additive;

	for (int i = src.length() - 1; i >= 0; i--)
	{
		int temp = src[i] - '0';
		temp = temp * 2 + mind;
		res += (temp % 10 + '0');
		mind = temp / 10;
	}
	if (mind > 0)
		res += (mind + '0');

	reverse(res.begin(), res.end());

	return res;
}

// Hàm chia 1 số Dec cho 2 dạng chuỗi string - Hỗ trợ DecToBin
string divByTwo(string decStr)
{
	string res = "";
	int additive = 0;
	for (int i = 0; i < (int)decStr.length(); i++)
	{
		int digit = decStr[i] - '0';
		res += ((digit / 2 + additive) + '0');
		additive = ((digit % 2 == 0) ? 0 : 5);
	}

	if (res[0] == '0' && res.length() > 1)
		res.erase(0, 1);
	return res;
}

QInt DecToBin(string qInt)
{
	QInt res;
	bool isNegative = false;
	if (qInt[0] == '-') // Kiểm tra số âm
	{
		isNegative = true;
		qInt.erase(0, 1); // Xóa dấu trừ
	}

	string resBin = "";
	do
	{
		int last = qInt[qInt.length() - 1] - '0'; // Chuyển thành số dương
		resBin += ((last % 2 == 0) ? '0' : '1'); // Chuỗi nhị phân ngược
		qInt = divByTwo(qInt);
	} while (qInt != "0");

	reverse(resBin.begin(), resBin.end()); // Đảo chuỗi nhị phân --> Bin
	res.ScanQInt(resBin, 2); // Nhập resBin vào res

	if (isNegative) // Nếu âm thì lấy nhị phân của số đối
		res = res.ToInverse();
	return res;
}

string BinToDec(const QInt& qInt)
{
	string res = "";
	// Trường hợp số 0
	if (qInt == QInt::Zero())
	{
		res = "0";
		return res;
	}

	QInt f_qInt = qInt; // Fake QInt
	// Kiểm tra số âm
	if (qInt.IsNegative())
	{
		// Nếu âm thì lấy số dương
		f_qInt = f_qInt.ToInverse();
	}

	string binStr = f_qInt.ToBinStr(); // Chuyển QInt sang chuỗi Bin (luôn luôn dương do if phía trên)
	int pos = binStr.find('1', 0) + 1; // Tìm vị trí bit sau  bit 1 đầu tiên từ trái sang
	res = "1";
	while (pos < (int)binStr.length())
	{
		int additive = binStr[pos] - '0';
		res = mulByTwo(res, additive);
		pos++;
	}

	if (qInt.IsNegative()) // Số âm thì thê dấu trừ phía trước
		res = '-' + res;

	return res;
}

QInt HexToBin(string hexStr)
{
	QInt res;
	string resBin = "";
	for (int i = 0; i < (int)hexStr.length(); i++)
	{
		int pos = ((string)HEX_CHARS).find(hexStr[i], 0); // Dò từng phần tử là kí tự gì trong HEX_CHARS
		resBin += bitset<4>(pos).to_string(); // Chuyển phần tử đó thành chuỗi Bin (8 bits) cộng dồn vào resBin
	}
	res.ScanQInt(resBin, 2); // Nhập resBin vào res
	return res;
}

string BinToHex(const QInt& qInt)
{
	string res = "";
	for (int i = 0; i < QINT_SIZE; i++)
	{
		unsigned char lBits, rBits;
		lBits = (qInt._arrBits[i] >> 4);
		rBits = (qInt._arrBits[i] & 0x0F);
		res += HEX_CHARS[lBits];
		res += HEX_CHARS[rBits];
	}
	while (res[0] == '0' && res.length() > 1)
		res.erase(0, 1);
	return res;
}

string DecToHex(string src)
{
	// Dec -> Bin -> Hex
	return BinToHex(DecToBin(src));
}

string HexToDec(string src)
{
	// Hex -> Bin -> Dec
	return BinToDec(HexToBin(src));
}

QInt Plus(const QInt& A, const QInt& B)
{
	QInt res;
	bool mind = 0; // Biến nhớ

	for (int i = QINT_SIZE * BITS - 1; i >= 0; i--)
	{
		int temp = A.GetBit(i) + B.GetBit(i) + mind;
		switch (temp)
		{
		case 0: // A=0, B=0, mind=0
			mind = 0;
			res.SetBit(i, 0);
			break;
		case 1: // A+B=1, mind=0 hoặc là A=0, B=0, mind=1; 
			mind = 0;
			res.SetBit(i, 1);
			break;
		case 2:	// A+B=0, mind=1 hoặc là A=1, B=1, mind=0; 
			mind = 1;
			res.SetBit(i, 0);
			break;
		case 3:	// A=1, B=1, mind=1
			mind = 1;
			res.SetBit(i, 1);
		}
	}

	return res;
}

void QInt::Div(const QInt divisor, QInt& div, QInt& mod) const
{
	mod = QInt::Zero();
	div = *this;
	QInt M = divisor;

	// Dấu của phép chia.
	bool isDNegative = div.IsNegative();
	bool isMDegative = divisor.IsNegative();

	// div, M dương để chia
	if (isDNegative)
	{
		div = div.ToInverse();
	}
	if (isMDegative)
	{
		M = M.ToInverse();
	}
	
	for (int i = 0; i < QINT_SIZE * BITS; i++) 
	{
		mod = mod << 1;
		mod.SetBit(QINT_SIZE * BITS - 1, div.GetBit(0)); // Lấy bit trái nhất của div đặt vào bit phải nhất của mod
		div = div << 1;

		QInt temp = mod - M;

		if (temp.GetBit(0) == false) // temp dương
		{
			mod = temp;
			div.SetBit(QINT_SIZE * BITS - 1, 1);
		}
	}

	if (isDNegative)		//So du va so bi chia cung dau
		mod = mod.ToInverse();
	if (isDNegative + isMDegative == 1) //Neu so bi chia va so chia trai dau thi doi dau thuong
		div = div.ToInverse();
}

QInt QInt::operator+(const QInt& B) const
{
	QInt res = Plus(*this, B);
	bool isANegative = this->IsNegative();
	bool isBNegative = B.IsNegative();

	if ((isANegative + isBNegative != 1) && (res.IsNegative() != isANegative)) //(A, B cung dau) va (res trai dau A)
	{
		throw "OVERFLOW!";
	}

	return res;
}

QInt QInt::operator-(const QInt& B) const
{
	// A-B = A+(-B)
	QInt res = Plus(*this, B.ToInverse());
	bool isANegative = this->IsNegative();
	bool isInvBNegative = !(B.IsNegative());

	// Trường hợp tràn số: (A, -B cung dau) va (res trai dau A)
	if ((isANegative + isInvBNegative != 1) && (res.IsNegative() != isANegative)) 
	{
		throw "OVERFLOW!";
	}

	return res;
}

QInt QInt::operator*(const QInt& B) const
{
	if (*this == QInt::Zero() || B == QInt::Zero())
	{
		return QInt();
	}

	// Trường hợp tràn số
	QInt min;
	min.SetBit(0, 1); // Min= -170141183460469231731687303715884105728
	QInt subOne;
	subOne.ScanQInt("-1", 10);
	// Max = 170141183460469231731687303715884105727
	// Min * (-1) = 170141183460469231731687303715884105728 (tràn số) 
	if ((*this == min && B == subOne) || (*this == subOne && B == min))
	{
		throw "OVERFLOW!";
	}

	QInt A; // A = 0
	QInt Q = B;
	bool Q_0 = false; // Gán Q_0 = 0 

	for (int i = 0; i < QINT_SIZE * BITS; i++)
	{
		bool lsbQ = Q.GetBit(QINT_SIZE * BITS - 1); // lay LSB cua Q

		if (lsbQ == false && Q_0 == true) // 01 -> A = A + *this;
		{
			A = A + *this;
		}
			
		if (lsbQ == true && Q_0 == false) // 10 -> A = A - *this;
		{
			A = A - *this;
		}

		// ShiftRight
		Q_0 = lsbQ;
		Q = Q >> 1;
		Q.SetBit(0, A.GetBit(QINT_SIZE * BITS - 1));
		A = A >> 1;
	}

	// Tràn số nếu tích / thừa số B khác *this
	if ((Q / B) != *this)
	{
		throw "OVERFLOW!";
		return QInt();
	}

	return Q;
}

// Toan tu chia 2 so QInt
QInt QInt::operator/(const QInt& B) const
{
	// Số chia = 0
	if (B == QInt::Zero())
	{
		throw "DIVIDE BY ZERO!";
	}
	// Số chia = 1 hoặc là số bị chia = 0, trả về số bị chia
	if (B == QInt::One() || *this == QInt::Zero())
	{
		return *this;
	}

	// Tràn số min * (-1), tương tự như tràn số ở phép nhân
	QInt min;
	min.SetBit(QINT_SIZE * BITS - 1, 1);
	QInt subOne;
	subOne.ScanQInt("-1", 10);
	if (*this == min && B == subOne)
	{
		throw "OVERFLOW!";
		return QInt();
	}

	QInt div; // Thương
	QInt mod; // Số dư
	this->Div(B, div, mod);

	return div;
}

QInt QInt::operator&(const QInt& B) const
{
	QInt res;
	for (int i = 0; i < QINT_SIZE; i++)
	{
		res._arrBits[i] = this->_arrBits[i] & B._arrBits[i];
	}
	return res;
}

QInt QInt::operator|(const QInt& B) const
{
	QInt res;
	for (int i = 0; i < QINT_SIZE; i++)
	{
		res._arrBits[i] = this->_arrBits[i] | B._arrBits[i];
	}
	return res;
}

QInt QInt::operator^(const QInt& B) const
{
	QInt res;
	for (int i = 0; i < QINT_SIZE; i++)
	{
		res._arrBits[i] = this->_arrBits[i] ^ B._arrBits[i];
	}
	return res;
}

QInt QInt::operator~() const
{
	QInt res;
	for (int i = 0; i < QINT_SIZE; i++)
	{
		res._arrBits[i] = ~this->_arrBits[i];
	}
	return res;
}

QInt QInt::operator>>(int num) const
{
	QInt res = *this;
	bool sign = this->IsNegative(); // Bit dấu
	while (num > 0)
	{
		for (int i = QINT_SIZE - 1; i >= 1; i--) // Dịch bit sang phải từng phần tử trong _arrBits[16]
		{
			res._arrBits[i] = res._arrBits[i] >> 1;

			// Trường hợp phần tử trước có bits[0] là 1, thì đẩy 1 vào bits[7] của phần tử sau
			if (res._arrBits[i - 1] & 1)
			{
				res._arrBits[i] = ((1 << 7) | res._arrBits[i]);
			}
		}

		// Dịch bit sang trái phan tử đầu và set lại dấu
		res._arrBits[0] = res._arrBits[0] >> 1;
		res.SetBit(0, sign);
		num--;
	}

	return res;
}

QInt QInt::operator<<(int num) const
{
	QInt res = *this;
	while (num > 0)
	{
		for (int i = 0; i < QINT_SIZE - 1; i++) // Dịch bit sang trái từng phần tử trong _arrBits[16]
		{
			res._arrBits[i] = res._arrBits[i] << 1;

			// Trường hợp phần tử sau có bits[0] = 1, thì đẩy 1 vào bits[7] của phần tử trước
			if ((res._arrBits[i + 1] >> 7) & 1)
			{
				res._arrBits[i] = (1 | res._arrBits[i]);
			}
		}

		// Dịch bit sang phải phần tử cuối
		res._arrBits[QINT_SIZE - 1] = res._arrBits[QINT_SIZE - 1] << 1;
		num--;
	}

	return res;
}

QInt QInt::rol(int num) const
{
	QInt res = *this;
	while (num > 0)
	{
		bool additive = ((res._arrBits[0] >> 7) & 1); // Lấy bit trái nhất

		for (int i = 0; i < QINT_SIZE - 1; i++)
		{
			res._arrBits[i] = res._arrBits[i] << 1; // Dịch tất cả các bit sang trái, thêm bit 0 ở bit phải nhất
			// Nếu bit trái nhất của byte thứ i + 1 là 1, sau khi xoay trái, bit phải nhất của byte thứ i đứng trước nó
			// Phải là 1.
			if ((res._arrBits[i + 1] >> 7) & 1) // Phép dịch phải cơ số 7, sau đó & 1 nhằm lấy bit trái nhất của byte thứ i + 1
			{
				res._arrBits[i] = (1 | res._arrBits[i]); // Phép or nhằm set bit phải nhất của byte thứ i là 1
			}
		}
		res._arrBits[QINT_SIZE - 1] = res._arrBits[QINT_SIZE - 1] << 1; // Dịch byte cuối cùng
		if (additive) // bit trái nhất là 1
			res._arrBits[QINT_SIZE - 1] = (1 | res._arrBits[QINT_SIZE - 1]); // Bit trái nhất thành bit phải nhất
		num--;
	}
	return res;
}

QInt QInt::ror(int num) const
{
	QInt res = *this;
	while (num > 0)
	{
		bool additive = (res._arrBits[QINT_SIZE - 1] & 1); //lấy bit phải nhất
		for (int i = QINT_SIZE - 1; i >= 1; i--)
		{
			res._arrBits[i] = res._arrBits[i] >> 1; // dịch tất cả các bit của từng byte sang phải
			if (res._arrBits[i - 1] & 1) // lấy bit phải nhất của byte đứng trước byte thứ i, nếu bit này là 1
			{

				// bit phải nhất của byte thứ i - 1 thành bit trái nhất của byte thứ i
				res._arrBits[i] = ((1 << 7) | res._arrBits[i]);
			}
		}
		res._arrBits[0] = res._arrBits[0] >> 1; // dịch byte cuối cùng
		if (additive) // nếu bit phải nhất của nums ban đầu là 1
			res._arrBits[0] = ((1 << 7) | res._arrBits[0]); // ta chuyển bit phải nhất đó thành bit trái nhất và đứng đầu chuỗi bit
		num--;
	}

	return res;
}

QInt& QInt::operator=(const QInt& B)
{
	for (int i = 0; i < QINT_SIZE; i++)
	{
		this->_arrBits[i] = B._arrBits[i];
	}
	return *this;
}

bool QInt::operator==(const QInt& B) const
{
	for (int i = 0; i < QINT_SIZE; i++)
	{
		if (this->_arrBits[i] != B._arrBits[i])
			return false;
	}
	return true;
}

bool QInt::operator!=(const QInt& B) const
{
	return !(*this == B);
}

string QInt::ToBinStr() const
{
	string res = "";
	for (int i = 0; i < QINT_SIZE; i++)
	{
		res += bitset<BITS>(this->_arrBits[i]).to_string();
	}
	return res;
}

string QInt::ToQIntStr(int base) const
{
	string res;
	switch (base)
	{
	case 2:
		res = ToBinStr();
		while (res[0] == '0' && res.length() > 1)
			res.erase(0, 1);
		return res;
		break;
	case 10:
		return BinToDec(*this);
		break;
	case 16:
		return BinToHex(*this);
		break;
	}
	return string();
}
