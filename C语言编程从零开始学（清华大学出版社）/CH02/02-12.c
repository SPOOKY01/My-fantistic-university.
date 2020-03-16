#include<stdio.h>
int main()
{
    int a = -456;
	int b = 456;
	/*输出宽度为8位整数，右对齐
	  输出宽度为8位整数，左对齐
	  输出3位整数*/
	printf("a=%8d,a=%-8d,a=%3d\n", a, a, a);
	/*输出宽度为8位，精度是5位的整数，左对齐。
	  输出宽度为8位，精度是5位的整数，右对齐。
	  输出3位，精度是5位的整数。
	  不够5位的补充0*/
	printf("a=%8.5d,a=%-8.5d,a=%3.5d\n", a, a, a);
	/*输出b小写的16进制数
	  输出B大写的16进制数*/
	printf("b=%x,b=%X\n", b, b);
	// b=%#X, 是会在打印出来的十六进制数前面加上0X
	printf("b=%#x,b=%#X\n", b, b);
	/*输出a小写的十六进制
	  输出A大写的十六进制*/
	printf("a=%x,a=%X\n", a, a);
	/*输出宽度为16的十六进制数，精度为12位的b左对齐
	  输出宽度为16的十六进制数，精度为12位的a右对齐*/
	printf("b=%-#16.12x,a=%#16.12x\n", b, a);
	return 0;  
}
