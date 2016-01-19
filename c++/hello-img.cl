void addx(int a, int b, __global char *str, int idx)
{
	str[idx] = a + b + 1;
}

__kernel void hello(__global unsigned char* ostr, __global int *nsize)
{
	int base = get_global_id(0);
	int cnt = 0;
	for(cnt = 0; cnt < nsize[0]; cnt += 3)
	{
		unsigned char tmp = ostr[cnt]; 
		ostr[cnt] = ostr[cnt + 2]; 
		ostr[cnt+2] = tmp - 30; 
	}
}

__kernel void otherfunc(__global char* ostr, __global char *istr)
{
	int base = get_global_id(0);
	if(istr[0] < 1) {
		ostr[0] = 'H';
	} else {
		ostr[0] = 'G';
	}
	ostr[1] = 'e';
	ostr[2] = 'l';
	ostr[3] = 'l';
	ostr[4] = 'o';
	ostr[5] = ',';
	ostr[6] = ' ';
	ostr[7] = 'W';
	ostr[8] = 'o';
	ostr[9] = 'r';
	ostr[10] = 'l';
	ostr[11] = 'd';
//	addx('d', base, ostr, 11);
	ostr[12] = '!';
	ostr[13] = '\0';
	ostr[32] = ostr[0] + 0;
	istr[0] = 'R';
}
