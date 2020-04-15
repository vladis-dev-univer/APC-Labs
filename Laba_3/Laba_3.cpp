#include <dos.h>
int attribute = 0x00; //начальный цвет

struct VIDEO
{
	unsigned char symb;
	unsigned char attr;
};

void get_reg();         			//получить данные из регистров
void print(int offset, int);		//заполнить указатель видеоизображения

//IRQ0-7
void interrupt(*int8) (...);// IRQ 0  - таймер		
void interrupt(*int9) (...);// IRQ 1  - клавиатура
void interrupt(*intA) (...);// IRQ 2  - каскадное переключение второго контроллера
void interrupt(*intB) (...);// IRQ 3  - последовательный порт COM 2
void interrupt(*intC) (...);// IRQ 4  - последовательный порт COM 1
void interrupt(*intD) (...);// IRQ 5  - параллельный порт LPT 2
void interrupt(*intE) (...);// IRQ 6  - контроллер флоппи-дисковод
void interrupt(*intF) (...);// IRQ 7  - параллельный порт LPT 1

//IRQ8-15
void interrupt(*int0) (...);// IRQ 8  - часы реального времени
void interrupt(*int1) (...);// IRQ 9  - свободный
void interrupt(*int2) (...);// IRQ 10 - контроллер видеоадаптера
void interrupt(*int3) (...);// IRQ 11 - свободный
void interrupt(*int4) (...);// IRQ 12 - мышь PS/2
void interrupt(*int5) (...);// IRQ 13 - математический сопроцессор
void interrupt(*int6) (...);// IRQ 14 - первый контроллер жёсткого диска
void interrupt(*int7) (...);// IRQ 15 - второй контроллер жёсткого диска


// new interrupt handlers
void interrupt  new8(...) { get_reg(); int8(); } //int8
void interrupt  new9(...) { attribute++; get_reg(); int9(); }	//keyboard int9
void interrupt  newA(...) { get_reg(); intA(); } //intA
void interrupt  newB(...) { get_reg(); intB(); } //intB
void interrupt  newC(...) { get_reg(); intC(); } //intC
void interrupt  newD(...) { get_reg(); intD(); } //intD
void interrupt  newE(...) { get_reg(); intE(); } //intE
void interrupt  newF(...) { get_reg(); intF(); } //intF

void interrupt  new0(...) { get_reg(); int0(); } //int0
void interrupt  new1(...) { get_reg(); int1(); } //int1
void interrupt  new2(...) { get_reg(); int2(); } //int2
void interrupt  new3(...) { get_reg(); int3(); } //int3
void interrupt  new4(...) { get_reg(); int4(); } //int4
void interrupt  new5(...) { get_reg(); int5(); } //int5
void interrupt  new6(...) { get_reg(); int6(); } //int6
void interrupt  new7(...) { get_reg(); int7(); } //int7

//заполнить указатель видеоизображения
void print(int offset, int val)
{
	char temp; //текущий бит
	int i;
	VIDEO far* screen = (VIDEO far *)MK_FP(0xB800, 0); //получить видеобуфер
	screen += offset;
	for (i = 7; i >= 0; i--)        //проходим по регистру и ищем бит
	{
		temp = val % 2;   			//получить последний бит
		val /= 2;         			//fill the screen
		screen->symb = temp + '0';  //сщхранить бит как символ('0' или '1')
		screen->attr = attribute;   //устанавливаем цвет
		screen++; 					//к следующему символу
	}
}

//получить данные из регистров
void get_reg()
{
	int reg;
	//ведущий контроллер
	reg = inp(0x21);  	//получить регистр масок ведущего контроллера
	print(0, reg);

	outp(0x20, 0x0B);	//переключение на регистр обслуживания ведущего контроллера
	reg = inp(0x20);	//получить регистр обслуживания ведущего контроллера
	print(9, reg);

	outp(0x20, 0x0A);	//переключение на регистр запросов ведущего контроллера
	reg = inp(0x20);	//получить регистр запросов ведущего контроллера         
	print(18, reg);

	reg = inp(0xA1);	//получить регистр масок ведомого контроллера
	print(80, reg);

	outp(0xA0, 0x0B);	//переключение на регистр обслуживания ведомого контроллера
	reg = inp(0xA0);	//получить регистр обслуживания ведомого контроллера  
	print(80 + 9, reg);

	outp(0xA0, 0x0A);	//переключение на регистр запросов ведомого контроллера
	reg = inp(0xA0);	//получить регистр запросов ведомого контроллера
	print(80 + 18, reg);
}

void init(){
	
	//IRQ0-7
	int8 = getvect(0x08); // IRQ 0
	int9 = getvect(0x09); // IRQ 1
	intA = getvect(0x0A); // IRQ 2
	intB = getvect(0x0B); // IRQ 3
	intC = getvect(0x0C); // IRQ 4
	intD = getvect(0x0D); // IRQ 5
	intE = getvect(0x0E); // IRQ 6
	intF = getvect(0x0F); // IRQ 7

	//IRQ8-15
	int0 = getvect(0x70); // IRQ 8
	int1 = getvect(0x71); // IRQ 9
	int2 = getvect(0x72); // IRQ 10
	int3 = getvect(0x73); // IRQ 11
	int4 = getvect(0x74); // IRQ 12
	int5 = getvect(0x75); // IRQ 13
	int6 = getvect(0x76); // IRQ 14
	int7 = getvect(0x77); // IRQ 15
	
	//переопределение IRQ 0-7 (08-0F)
	setvect(0x08, new8);
	setvect(0x09, new9);
	setvect(0x0A, newA);
	setvect(0x0B, newB);
	setvect(0x0C, newC);
	setvect(0x0D, newD);
	setvect(0x0E, newE);
	setvect(0x0F, newF);

	//переопределение IRQ 8-15 (B0-B7)
	setvect(0xB0, new0);
	setvect(0xB1, new1);
	setvect(0xB2, new2);
	setvect(0xB3, new3);
	setvect(0xB4, new4);
	setvect(0xB5, new5);
	setvect(0xB6, new6);
	setvect(0xB7, new7);
	
	_disable(); // запрет прерывания (Clear Interrupt Flag)
	
	/*Макрос outp выводит байт, определённый (0x11) в порт, который определяется параметром (0x20)*/
	//Инициализация прерываний для ведущего контроллера
	outp(0x20, 0x11);	//ICW1 – инициализация ведущего контроллера
	outp(0x21, 0x08);	//ICW2 – базовый вектор для ведущего
	outp(0x21, 0x04);	//ICW3 – порт бит ведомого контроллера (в двоичном формате)
	outp(0x21, 0x01);	//ICW4 – по умолчанию

    	//Инициализация прерываний для ведомого контроллера
	outp(0xA0, 0x11);  	//ICW1 – инициализация ведомого контроллера
	outp(0xA1, 0xB0);	//ICW2 – базовый вектор для ведомого
	outp(0xA1, 0x02);	//ICW3 – количество подключенных портов к ведущему
	outp(0xA1, 0x01);	//ICW4 – по умолчанию

	_enable(); // возобновление возможности прерываний
}

int main()
{
	unsigned far *fp; //объявляем указатель
	init();
	
	FP_SEG(fp) = _psp; //получаем сегмент
	FP_OFF(fp) = 0x2c; //получаем смещение сегмента данных с переменными среды
	_dos_freemem(*fp); //освобождение памяти под них

	_dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);//оставляем резидентной, первый параметр – код 
	//завершения, второй – объём памяти, который должен быть зарезервирован 
	//для программы после её завершения
	return 0;
}

