#undef __cplusplus
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "LeapC.h"
#include "ExampleConnection.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define WINVER 0x0500

int64_t lastFreameID = 0;
/*
Structura do ³atwego przechowywania danych o przyspieszeniu
*/
typedef struct _Vel {
	float x;
	float y;
	float z;
} _Vel;

enum _Direction {
	up = 1,
	down = 2,
	right = 4,
	left = 8,
	forward = 16,
	backward = 32
};

enum _Dir_short{
	u,
	d,
	r,
	l,
	f,
	b
};

unsigned short int move = 0; 
/*
*	1 - up
*	2 - down
*	3 - right
*	4 - left
*	5 - forward
*	6 - backward
*
*		[32] [16] [8] [4] [2] [1]
*		 |    |	   |   |   |   |
*	 	 |    |    |   |   |   up
*	 	 |    |    |   |   |
*	     |    |    |   |  down
*	     |    |    |   |
*   	 |    |    | right
*		 |    |    |
*		 |    |  left
*		 |    |
*		 | forward
*		 |
*	 backward
*
*
*/

int main(int argv, char* argc[]) {
	time_t rawtime;
	struct tm * timeinfo;

	const float sensitivity = 300.0;
	const float margin = 25.0;

	_Vel vel;

	OpenConnection(); //Opening connection

	while (!IsConnected) //Waiting for connection accured
		millisleep(100);

	printf("Connected.\n");
	LEAP_DEVICE_INFO* deviceProps = GetDeviceProperties(); //Getting device infos
	if (deviceProps)
		printf("Using device %s.\n", deviceProps->serial);

	for (;;) { //Main loop
		LEAP_TRACKING_EVENT *frame = GetFrame();
		if (frame && (frame->tracking_frame_id > lastFreameID)) {
			lastFreameID = frame->tracking_frame_id;

			for (uint32_t h = 0; h < frame->nHands; h++) {
				LEAP_HAND* hand = &frame->pHands[h];
				vel.x = hand->palm.velocity.x;
				vel.y = hand->palm.velocity.y;
				vel.z = hand->palm.velocity.z;

				if ( vel.x > sensitivity || vel.x < -sensitivity || vel.y > sensitivity || vel.y < -sensitivity || vel.z > sensitivity || vel.z < -sensitivity ) {
					if (vel.x < (sensitivity - margin) || vel.x >(-sensitivity - margin) || vel.y < (sensitivity + margin) || vel.y >(-sensitivity - margin) || vel.z < (sensitivity + margin) || vel.z >(-sensitivity - margin)) {
						time(&rawtime);
						timeinfo = localtime(&rawtime);
						//printf("[%02d:%02d:%02d] %s reka rusza sie ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, (hand->type == eLeapHandType_Left ? "Lewa" : "Prawa"));

						if (vel.y > sensitivity) {
							//printf("w gore "); 
							move += up;
						}
						else if (vel.y < -sensitivity) {
							//printf("w dol "); 
							move += down;
						}
						if (vel.x > sensitivity) {
							//printf("w prawo ");
							move += right;
						}
						else if (vel.x < -sensitivity) {
							//printf("w lewo ");
							move += left;
						}
						if (vel.z > sensitivity) {
							//printf("do siebie ");
							move += backward;
						}
						else if (vel.z < -sensitivity) {
							//printf("od siebie ");
							move += forward;
						}

						//mamy nasz move
						//printf("\n");

						if (move & 1 << d && move & 1 << l && move & 1 << b) //dol, lewo, do siebie
							printf("Z");
						else if (move & 1 << d && move & 1 << r && move & 1 << b) // dol, prawo, do siebie
							printf("Y");
						else if (move & 1 << d && move & 1 << l && move & 1 << f) // dol, lewo, od siebie
							printf("X");
						else if (move & 1 << d && move & 1 << r && move & 1 << f) // dol, prawo, od siebie
							printf("W");
						else if (move & 1 << u && move & 1 << l && move & 1 << b) // gora, lewo, do siebie
							printf("V");
						else if (move & 1 << u && move & 1 << r && move & 1 << b) // gora, prawo, do siebie
							printf("U");
						else if (move & 1 << u && move & 1 << l && move & 1 << f) // gora, lewo, od siebie
							printf("T");
						else if (move & 1 << u && move & 1 << r && move & 1 << f) // gora, prawo, od siebie
							printf("S");
						else if (move & 1 << d && move & 1 << b) // dol, do siebie
							printf("R");
						else if (move & 1 << d && move & 1 << f) // dol, od siebie
							printf("Q");
						else if (move & 1 << u && move & 1 << b) // gora, do siebie
							printf("P");
						else if (move & 1 << u && move & 1 << f) // gora, od siebie
							printf("O");
						else if (move & 1 << b && move & 1 << l) // do siebie, lewo
							printf("N");
						else if (move & 1 << b && move & 1 << r) // do siebie, prawo
							printf("M");
						else if (move & 1 << f && move & 1 << l) // od siebie, lewo
							printf("L");
						else if (move & 1 << f && move & 1 << r) // od siebie, prawo
							printf("K");
						else if (move & 1 << d && move & 1 << l) // dol, lewo
							printf("J");
						else if (move & 1 << d && move & 1 << r) // dol, prawo
							printf("I");
						else if (move & 1 << u && move & 1 << l) // gora, lewo
							printf("H");
						else if (move & 1 << u && move & 1 << r) // gora, prawo
							printf("G");
						else if (move & 1 << b) // do siebie
							printf("F");
						else if (move & 1 << f) // od siebie
							printf("E");
						else if (move & 1 << l) // lewo
							printf("D");
						else if (move & 1 << r) // prawo
							printf("C");
						else if (move & 1 << d) //dol
							printf("B");
						else if (move & 1 << u) // gora
							printf("A");

						//koniec czytania ruchu i reset ruchu
						move = 0;

						printf("\n");
						millisleep(1000);
					}
				}
			}
		}
		millisleep(100);
	} // ctrl-c to exit

	return 0;
}