#include <string>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	double _a = 0;


	__LABEL_0:
		cout << ("Hello World");
		goto __LABEL_3;

	__LABEL_1:
		_a = (5);
		goto __LABEL_1;

	__LABEL_2:
		cout << (_a / 3);
		goto __LABEL_0;

	__LABEL_3:
		exit(0);
		goto __LABEL_0;

	__LABEL_4:
		cout << ((_a + 2) * 19);
		goto __LABEL_2;


}
