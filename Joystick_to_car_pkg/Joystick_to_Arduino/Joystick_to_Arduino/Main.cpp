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
	int size = 14;	//Size of package being sent is 3 ints of 4 bytes each + 1 char

	/* SETTING UP SHARED MEMORY FOR USE WITH THE JOYSTICK */
	int n = 1000;
	char name[] = "smem_joystick";
	char *p_smem;

	p_smem = shared_memory(name, n);
	
	open_serial("COM5", h2, speed);

		while (KEY('X') == 0)
		{			
			serial_send(p_smem, size, h2);

			Sleep(10);

			//The Arduino is Serial.printing() many things AFTER it receives the start of a message. Thus, it doesn't send to a buffer that has multiple elements.
			while (serial_available(h2) > 0)
			{
				serial_recv(buffer, 1, h2);
				cout <<buffer[0];
			}
		}

	close_serial(h2);

	return 0;
}
