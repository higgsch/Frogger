#include <string>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	double _l = 0;
	double _area = 0;
	double _w = 0;
	double _a = 0;
	double _temp_1 = 0;
	double _temp_2 = 0;
	double _temp_3 = 0;


	__LABEL_0:
		cout << ("Enter a length for rectangle\'s side l: ");
		goto __LABEL_4;

	__LABEL_1:
		cout << ("The area for the entered rectangle is: ");
		goto __LABEL_7;

	__LABEL_2:
		exit(0);
		goto __LABEL_1;

	__LABEL_3:
		cout << ("Enter a length for rectangle\'s side w: ");
		goto __LABEL_6;

	__LABEL_4:
		cin >> _temp_1;
		_l = ( _temp_1 );
		goto __LABEL_3;

	__LABEL_5:
		_area = (_l * _w);
		goto __LABEL_1;

	__LABEL_6:
		cin >> _temp_1;
		_w = ( _temp_1 );
		goto __LABEL_5;

	__LABEL_7:
		cout << (_area);
		goto __LABEL_2;

	__LABEL_8:
		cin >> _temp_1;
		cin >> _temp_2;
		cin >> _temp_3;
		_a = ( _temp_1  *  _temp_2  *  _temp_3 );
		goto __LABEL_2;


}
