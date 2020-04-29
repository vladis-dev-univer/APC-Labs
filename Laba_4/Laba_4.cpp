#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>

void rnd_set(int bound) {

	outp(0x43, 0xb4);
	outp(0x42, bound % 255);
	outp(0x42, (bound % 65520) >> 8);
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
		if (*number <= 0 || *number >= 65520) {
			printf("Incorrect input. Enter right value (less then 65520): ");
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

#define TIMER_CLOCK		1193180
void set_frequence(unsigned frequence)
{
	unsigned result = 0;

	outp(0x43, 0xB6);

	result = TIMER_CLOCK / frequence;
	outp(0x42, result % 256);

	result /= 256;
	outp(0x42, result);
}
#undef TIMER_CLOCK

void play_sound(unsigned int *freqs, unsigned int *delays,unsigned int *pause, unsigned int size)
{
	for (unsigned i = 0; i < size; i++)
	{
		set_frequence(freqs[i]);

		outp(0x61, inp(0x61) | 3);
		delay(delays[i]);
		outp(0x61, inp(0x61) & 0xfc);

		delay(pause[i]);
	}

}
unsigned int pause[] =
{
	 30,  30,  30,  30,  30,  30,  150,  30,  30,  30,  30,  30,  30,  150,
	 30,  30,  30,  30,  30,  30,  150,  30,  30,  30,  30, 30, 150,
};
unsigned int freqs[] =
{
220, 164, 220, 164, 220, 207, 207, 207, 164, 207, 164, 207, 220, 220,
220, 164, 220, 164, 220, 207, 207, 207, 164, 207, 164, 207, 220,
};

unsigned int delays[] =
{
400, 400, 400, 400, 400, 400, 700, 400, 400, 400, 400, 400, 400, 700,
400, 400, 400, 400, 400, 400, 700, 400, 400, 400, 400, 400, 900,
};

void menu()
{
	printf("\n1. Play sound");
	printf("\n2. Print channels");
	printf("\n3. Print state words");
	printf("\n4. Get random digit");
	printf("\n5. Exit.");
	printf("\nYour choice: ");
	return;
}

int main() {
	char selection;
	long digit;

	while(1){
		system("cls");
		menu();
		selection = getchar();

		switch (selection) {
			case '1':
				play_sound(freqs, delays, pause, sizeof(freqs)/sizeof(unsigned)); //звук
				break;
			case '2':
				devision_ratio(); //коэффициент деления
				break;
			case '3':
				status_word(); //состояние слова для каждого канала
				break;
			case '4': //рандомные числа
				long digit;
				RandomBoardEnter(&digit);
				rnd_set(digit);
				printf("Press any key to get digit.\n");

				getch();
				digit = rnd_get();
				printf("\nRandomed digit: %ld\n", digit);
				break;
			case '5':
				return 0;
			default:
				printf("Error key! Please try again\n");
				break;
		}
		system("pause");
	}
}
