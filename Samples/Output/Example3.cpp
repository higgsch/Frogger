#include <string>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	double _six = 0;
	double _eight = 0;
	double _some_double = 0;
	double _thirty_one = 0;
	double _two_pow_fifth = 0;


	__LABEL_0:
		_six = (6);
		goto __LABEL_1;

	__LABEL_1:
		_eight = (2 + _six);
		goto __LABEL_10;

	__LABEL_2:
		cout << ("2 ^(5) = ");
		goto __LABEL_11;

	__LABEL_3:
		cout << (_some_double);
		goto __LABEL_6;

	__LABEL_4:
		cout << ("30/13 = ");
		goto __LABEL_3;

	__LABEL_5:
		_thirty_one = (_thirty_one + (_two_pow_fifth / _eight) - 3);
		goto __LABEL_1;

	__LABEL_6:
		exit(0);
		goto __LABEL_6;

	__LABEL_7:
		_some_double = (_thirty_one / 13);
		goto __LABEL_2;

	__LABEL_8:
		cout << ("31/13 = ");
		goto __LABEL_4;

	__LABEL_9:
		cout << ("\n");
		goto __LABEL_4;

	__LABEL_10:
		_two_pow_fifth = (_eight * 4);
		goto __LABEL_13;

	__LABEL_11:
		cout << (_two_pow_fifth);
		goto __LABEL_9;

	__LABEL_12:
		cout << ("Ready to take an advil yet? I am!");
		goto __LABEL_3;

	__LABEL_13:
		_thirty_one = (_two_pow_fifth - 2);
		goto __LABEL_7;


}
