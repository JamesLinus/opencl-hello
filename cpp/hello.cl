__kernel void hello(__global char* string)
{
	int base = get_global_id(0);
//	string[0] = 'H';
	string[1] = 'e';
	string[2] = 'l';
	string[3] = 'l';
	string[4] = 'o';
	string[5] = ',';
	string[6] = ' ';
	string[7] = 'W';
	string[8] = 'o';
	string[9] = 'r';
	string[10] = 'l';
	string[11] = 'd' + base;
	string[12] = '!';
	string[13] = '\0';
	string[32] = string[0] + 0;
}
