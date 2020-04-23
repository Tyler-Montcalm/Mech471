#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>

#include <windows.h>

#include "serial_com.h"
#include "timer.h"
#include "shared_memory.h"

using namespace std;

#define KEY(c) ( GetAsyncKeyState((int)(c)) & (SHORT)0x8000 )

int main()
{
	/* SETTING UP SERIAL COMMUNICATION BETWEEN THE COMPUTER AND THE ARDUINO */
	HANDLE h2;
	const int NMAX = 64;
	char buffer[NMAX];
	int speed = 1;
	int size = 12;	//Size of package being sent is 3 ints of 4 bytes.

	/* SETTING UP SHARED MEMORY FOR USE WITH THE JOYSTICK */
	int n = 1000;
	char name[] = "smem_joystick";
	char *p_smem;

	p_smem = shared_memory(name, n);

	int *p_int;
	p_int = (int*)p_smem;

	open_serial("COM3", h2, speed);

	while (KEY('X') == 0)
	{

		serial_send(p_smem, size, h2);

		cout << p_int[0] << "\t" << p_int[2] << "\n";

		//Should make something to receive an error message

		Sleep(10);
	}

	close_serial(h2);

	return 0;
}//Main lesson: Make sure that the SIZE is correct