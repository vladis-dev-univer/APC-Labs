#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>


void rnd_set(int bound) {

	outp(0x43, 0xb4);

	outp(0x42, bound & 0x00ff);
	outp(0x42, (bound & 0xff00) >> 8);

	outp(0x61, inp(0x61) | 1);
}

int rnd_get(void) {

	int i;

	outp(0x43, 0x86);

	i = inp(0x42);
	i = (inp(0x42) << 8) + i;

	return (i);
}

void RandomBoardEnter(long *number) {
	printf("\nEnter random boarder: ");
	for (;;) {
		while (scanf("%ld", number) != 1) {
			printf("Incorrect input. Enter only digits: ");
			while (getchar() != '\n');
			continue;
		}
		if (*number <= 0 || *number >= 65535) {
			printf("Incorrect input. Enter right value (less then 65535): ");
			continue;
		}
		if (getchar() != '\n') {
			printf("Incorrect input. Enter only digits: ");
			while (getchar() != '\n');
			continue;
		}
		else {
			break;
		}
	}
	return;
}

void devision_ratio() {
	int iChannel;
	long j;
	long kd_low, kd_high, kd, kd_max;

	for (iChannel = 0; iChannel < 3; iChannel++) {
		kd_max = 0;
		for (j = 0; j < 65535; j++) {
			switch (iChannel) {
			case 0: {
				outp(0x43, 0x0);
				kd_low = inp(0x40);
				kd_high = inp(0x40);
				kd = kd_high << 8 | kd_low;
				break;
			}
			case 1: {
				outp(0x43, 0x40);
				kd_low = inp(0x41);
				kd_high = inp(0x41);
				kd = kd_high << 8 | kd_low;
				break;
			}
			case 2: {
				outp(0x43, 0x80);
				kd_low = inp(0x42);
				kd_high = inp(0x42);
				kd = kd_high << 8 | kd_low;
				break;
			}
			}
			if (kd_max < kd) {
				kd_max = kd;
			}
		}
		switch (iChannel) {
		case 0: {
			printf("Channel 0x40 kd: %ld\n", kd_max);
			break;
		}
		case 1: {
			printf("Channel 0x41 kd: %ld\n", kd_max);
			break;
		}
		case 2: {
			printf("Channel 0x42 kd: %ld\n", kd_max);
			break;
		}
		}
	}
	return;
}

void print_bchar(unsigned char value)
{
	for (int i = 0; i < sizeof(unsigned char) * 8; i++)
	{
		printf("%d", (value & 0x80u) >> 7);
		value <<= 1;
	}
}

void status_word()
{
	unsigned char state;

	outp(0x43, 0xE2); 	//11100010  0x40
	state = inp(0x40);
	print_bchar(state);
	puts("");

	outp(0x43, 0xE4); 	//11100100  0x41
	state = inp(0x41);
	print_bchar(state);
	puts("");

	outp(0x43, 0xE8); 	//11101000  0x42
	state = inp(0x42);
	print_bchar(state);
	puts("");
}

void speaker(int turnOn)
{
	int result = inp(0x61);

	if (turnOn)
	{
		result |= 0x03;
	}
	else
	{
		result &= 0xFC;
	}
	outp(0x61, result);
}

#define TIMER_CLOCK		1193180
void set_frequence(unsigned frequence)
{
	unsigned result = 0;

	unsigned frequence_delimeter = TIMER_CLOCK / frequence;

	outp(0x43, 0xB6);

	result = frequence_delimeter & 0x00FF;
	outp(0x42, result);

	result = frequence_delimeter >> 8;	
	outp(0x42, result);
}
#undef TIMER_CLOCK

void play_sound(unsigned int *freqs, unsigned int *delays, unsigned int size)
{
	#define TURN_ON 1
	#define TURN_OFF 0
	for (unsigned i = 0; i < size; i++)
	{
		set_frequence(freqs[i]);
		speaker(TURN_ON);


		short tick1;
		short tick2;
		_asm {
			xor ah, ah
			int 1Ah
			mov tick1, cx
			mov tick2, dx
		}
		int *ptr = (int *)&tick1;

		*ptr /= 1000;

		_asm {
			mov cx, tick1
			mov dx, tick2
			mov ah, 01h
			int 1Ah
		}

		delay(delays[i]);
		speaker(TURN_OFF);
	}
	#undef TURN_ON
	#undef TURN_OFF
}

void menu()
{
	printf("\n1. Play sound");
	printf("\n2. Print channels");
	printf("\n3. Print state words");
	printf("\n4. Get random digit");
	printf("\n5. Exit.");
	printf("\n\Your choice: ");
	return;
}

void menu_choise(int *number) {
	for (; ; ) {
		while (scanf("%d", number) != 1) {
			printf("\nIncorrect input. Enter only digits: ");
			while (getchar() != '\n');
			continue;
		}
		if (*number != 1 && *number != 2 && *number != 3
			&& *number != 4 && *number != 5 && *number != 6) {
			printf("\nIncorrect input. Choose one of points in menu: ");
			continue;
		}
		if (getchar() != '\n') {
			printf("\nIncorrect input. Enter only digits: ");
			while (getchar() != '\n');
			continue;
		}
		else {
			break;
		}
	}
	return;
}

unsigned int freqs[] = {
392,
392,
293,
196,
196,
392,
392,
293,
196 };

unsigned int delays[] = {800, 400, 400, 400, 400, 400, 400, 400, 400};

int main() {
	int check_exit = 0;
	system("cls");
	menu();
	menu_choise(&check_exit);
	while (check_exit != 5) {
		switch (check_exit) {
		case 1: {
			play_sound(freqs, delays, sizeof(freqs) / sizeof(unsigned));
			break;
		}
		case 2: {
			devision_ratio();
			break;
		}
		case 3: {
			status_word();
			break;
		}
		case 4: {
			long digit;
			RandomBoardEnter(&digit);
			rnd_set(digit);
			printf("Press any key to get digit.\n");

			getch();
			digit = rnd_get();
			printf("\nRandomed digit: %ld\n", digit);
			break;
		}
		}
		check_exit = 0;
		system("pause");
		system("cls");
		menu();
		menu_choise(&check_exit);
	}
	return 0;
}
