void addx(int a, int b, __global char *str, int idx)
{
	str[idx] = a + b + 1;
}

__kernel void hello(__global unsigned char* ostr, __global int *width)
{
	unsigned int x = get_global_id(0);
	unsigned int y = get_global_id(1);
	//unsigned int index = x + (y * width[0]);
	unsigned int index = (get_global_id(1) * get_global_size(0)) + get_global_id(0);
	//unsigned int index = x;
	if(ostr[index] > 205) {
		ostr[index] = 10;
	}
	if(ostr[index] < 10) {
		ostr[index] = 205;
	}
}

__kernel void imageinvert(__global unsigned char* ostr, const unsigned int nsize)
{
	//int x = get_global_id(0);
	//int y = get_global_id(1);
	//convert (B, G, R) triples to (R, G, B)
//	for(cnt = 0; cnt < nsize[0]; cnt += 3)
//	{
//		unsigned char tmp = ostr[cnt]; 
//		ostr[cnt] = ostr[cnt + 2]; 
//		ostr[cnt+2] = tmp; 
//	}
	//now invert colors
	int cnt = 0;
	for(cnt = 0; cnt < nsize; cnt++)
	{
		if(ostr[cnt] > 205) {
			ostr[cnt] = 10;
		}
		if(ostr[cnt] < 10) {
			ostr[cnt] = 205;
		}
	}
}

__kernel void procimg(__read_only image2d_t srcImage, __write_only image2d_t destImage)
{
	//read_imagef()

}
