
#include <iostream>
#include <conio.h>
#include <Windows.h>

#include "shared_memory.h"

using namespace std;

int main(int argc, char* argv[])
{
	int i;
	int n = 1000; // size of shared memory block (bytes)
	char name[] = "smem_joystick"; // name of shared memory block
	char *p; // pointer to shared memory

	int *q; // pointer to an array of integers
	// to be stored in the shared memory block

	// create/access shared memory
	p = shared_memory(name,n);

	// convert the character pointer to an int pointer
	q = (int *)p;

	while(1) {
		cout << "\nq[0] = " << q[0] << "\t q[1] = " << q[1];

		// suspend program B for 10ms allowing other
		// programs to operate
		Sleep(10);
	}

	cout << "\ndone.\n";
	getch();

	return 0;
}

