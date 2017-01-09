#include <string>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	double _d = 0;
	double _b = 0;


	__LABEL_0:
		_d = (5);
		goto __LABEL_4;

	__LABEL_1:
		cout << ("(5*4)^2 = ");
		goto __LABEL_3;

	__LABEL_2:
		exit(0);
		goto __LABEL_0;

	__LABEL_3:
		cout << (_d * (_b * _d) * _b);
		goto __LABEL_2;

	__LABEL_4:
		_b = (4);
		goto __LABEL_1;


}
