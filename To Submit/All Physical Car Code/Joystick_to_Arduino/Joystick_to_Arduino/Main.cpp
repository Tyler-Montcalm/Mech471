//Written by: Thomas Savage (40016675)
//Written for MECH 471
//Last modified: 28th April 2020

//This code collects the data from shared memory with the modified "joystick" function.
//The actual message being sent is two of the normal messages plus two starting markers.
//This ensures that if I find the starting marker in "Transmitter" I am guaranteed to have the complete message.
//Note: The data going into shared memory was modified by me by tweaking "joystick" starting on line 373. I like using the two front
//variable "buttons" (LT and RT on an Xbox controller) for throttle control. I didn't change the layout of shared memory, so I have 
//a piece of data in my signal that doesn't do anything for the moment (q[1]) while q[0] and q[2] are used.

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
	int size = 26;	//Size of package being sent is 2x 3 ints of 4 bytes each + 1 char

	/* SETTING UP SHARED MEMORY FOR USE WITH THE JOYSTICK */
	int n = 1000;
	char name[] = "smem_joystick";
	char *p_smem;
	char data_out[NMAX];

	p_smem = shared_memory(name, n);
	
	open_serial("COM6", h2, speed);

		while (KEY('X') == 0)
		{	
			data_out[0] = '\n';
			data_out[13] = '\n';

			for (int i = 1; i <= 12; i++)
			{
				data_out[i] = p_smem[i-1];	//The hope is that this happens fast enough that the data in the shared memory doesn't change during this write.
			}
			for (int i = 14; i <= 25; i++)
			{
				data_out[i] = p_smem[i - 14];	//The hope is that this happens fast enough that the data in the shared memory doesn't change during this write.
			}

			serial_send(data_out, size, h2);

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
