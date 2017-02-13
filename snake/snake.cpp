//------------declaracion de la libreria---------------------
#include<Windows.h>
#include<iostream>
#include<conio.h>
#include<stdlib.h>
#include <time.h>
//---------------declaracion de las constantes-----------------
#define ARRIBA 72 
#define DERECHA 77
#define IZQUIERDA 75
#define ABAJO 80
#define ESC 27
#define ENTER 13
//----------------variables-------------------------------------
int cuerpo[200][2];
int n = 1;
int tamanio = 10;
int x = 10 , y = 12;
int direccion = 3;
int xc = 80, yc = 5;
int velocidad = 100, h = 1;
int puntaje = 0;
int posicion = 1;

char tecla;//Checa la tecla que se pulsa..

void gotoxy(int x, int y)
{
	HANDLE hCon;
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hCon, dwPos);
}

char inicio[50][100] =
{
"                                                                      ",
"       A            AAAA AA   A AAAA A A   AAAA                      ",
"        A            A    A A  A A  A AAA   A                         ",
"        A            A    A  A A AAAA A  A  AAA                       ",
"      AAAAA          A    A   AA A  A A   A A                         ",
"     A AAA A     AAAAA    A   AA A  A A   A AAAA                      ",
"    AAAAAAAAA                                                         ",
"    AAAAAAAAA                                                         ",
"     AAAAAAA           AAAAAAAAAAA               AAAAAAAAAAAAAAAA     ",
"     AAAAAAA          AAAAAAAAAAAAAA            AAAAAAAAAAAAAAAAAA    ",
"      AAAAAAA        AAAAAAAAAAAAAAAA          AAAAAAAAAAAAAAAAAA     ",
"       AAAAAAA      AAAAAA     AAAAAAA        AAAAAAA                 ",
"        AAAAAAA    AAAAAA       AAAAAAA      AAAAAA                   ",
"         AAAAAAAAAAAAAAA         AAAAAAAAAAAAAAAAA                    ",
"          AAAAAAAAAAAAA           AAAAAAAAAAAAAAA                     ",
"           AAAAAAAAAAA             AAAAAAAAAAAAA                      ",
"                                                                      "
};
//------------------------------Establece la coordenada con rutinas en ensamblador-------------------------------
int coordenada(int numero, int suma)
{
	_asm
	{
		MOV EAX, suma
		ADD EAX, numero//Lo que tengamos en EAX se devuelve sin necesidad de un return.
	}
}
//-----------------------Metodo que llama a la presentacion inicial del snake-------------------------
void menuInicial()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	//Establece como se dibujara el inicio del snake. establecido en el char inicio.
	for(int i = 0;i < 17;i++)
	{
		for(int j = 0;j < 101;j++)
		{
			if(inicio[i][j] == 'A')//De acuerdo al formato de inicio si encuentra un A ampezara a rellenar con * el inicio.
			{          
				gotoxy(coordenada(j, 5), coordenada(i, 5));//Llama a la rutina en ensamblador.
				printf("*");//pinta con * lo que se dibujara de acuerdo al formato de inicio.
			}
		}
	}
	getch();//Se espera en esta parte del programa 
	//Borra lo anterior dibujado.
	for(int i = 0;i < 17;i++)
	{
		for(int j = 0;j < 100;j++)
		{
			if(inicio[i][j] == 'A')
			{
				gotoxy(coordenada(j, 5), coordenada(i, 5));
				printf(" ");
			}
		}
	}
}

//Dibuja el margen del snake.................................................
void dibujar()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);//Este maneja el color de las lineas.
	
	gotoxy(2, 3);
	printf("%c", 0);
	gotoxy(2, 300);
	printf("%c", 0);
	gotoxy(100, 3);
	printf("%c", 0);
	gotoxy(100, 23);
	printf("%c", 0);
}

void guardarPosicion()
{
	cuerpo[n][0] = x;
	cuerpo[n][1] = y;
	_asm
	{
		INC n//incrementa el indice del cuerpo...
		MOV EAX, tamanio
		CMP n, EAX
		JE Igual
		JMP Salir
		Igual:
		MOV n, 1
		JMP Salir
		Salir:
	}
}
//---------------dibuja el snake-----------------------------
void dibujarCuerpo()
{//                     
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	for(int i = 1;i < tamanio;i++)
	{
		gotoxy(cuerpo[i][0], cuerpo[i][1]);
		printf("%c",223);
	}
}
//Borra la trayectoria del snake..........
void borrarCuerpo()
{
	gotoxy(cuerpo[n][0], cuerpo[n][1]);
	printf(" ");
}

void accionTeclado(int comparar, int igual)
{   
	_asm
	{
		MOV EAX, comparar
		CMP direccion, EAX
		JNE Accion
		JMP Salir
		Accion:
		MOV EAX, igual
		MOV direccion, EAX
		Salir:
	}
}
//---------------------Establece que tecla se utiliza para que se mueva el snake-----------------
void pulsarTecla()
{
	if(kbhit())//retorna 0 si esta regristrda una tecla pulsada y si el valor 
	{
		tecla = getch();
		switch(tecla)
		{
		case ARRIBA:
			accionTeclado(2, 1);
			break;
		case ABAJO:
			accionTeclado(1, 2);
			break;
		case DERECHA:
			accionTeclado(4, 3);//(direccion de la tecla opuesta,La direccion de las teclas (decrementando en y))
			break;
		case IZQUIERDA:
			accionTeclado(3, 4);
			break;
		}
	}
}
//--------------cambia la velocidad del snake------------

void cambiarVelocidad()
{
	_asm
	{    //Cada 20 en score va aumentando la velocidad del snake....
		MOV EAX, h
		MOV EBX, 20
		MUL EBX
		CMP puntaje, EAX
		JE Igual
		JMP Salir
		Igual:
		SUB velocidad, 10 //Debes restarle para aumentar la velocidad que inicialmente es 100
		INC h
		Salir:
	}
}
//-------------------dibuja la comida del snake--------------------
void dibujarComida()
{
	if (x == xc && y == yc)
	{
		_asm
		{   //Acomoda la comida de acuerdo al margen donde se encuentra la culebra
			
				INC tamanio//Incrementa la snake
				ADD puntaje, 10//Incrementa le puntaje..
		}
		do {
			xc = (rand() % 97);
			yc = (rand() % 18);
			_asm{
					ADD xc, 4
					ADD yc, 4
			}
			
			if (!(yc == 3 || yc == 23 || yc > 3 && yc < 20 && xc == 20 || xc == 2 || xc == 100
				|| xc >= 10 && xc <= 20 && yc == 19 || yc == 9 && xc>3 && xc < 13
				|| yc == 10 && xc >= 21 && xc < 50 || yc == 7 && xc >= 21 && xc < 40 || xc == 49 && yc >= 5 && yc < 10
				|| yc == 17 && xc >= 21 && xc < 40 || yc == 14 && xc >= 30 && xc < 40 || xc == 30 && yc >= 14 && yc < 17
				|| xc == 65 && yc >= 8 && yc < 23 || yc == 8 && xc >= 66 && xc < 90 || xc == 90 && yc >= 8 && yc < 18 || yc == 17 && xc >= 77 && xc < 90
				|| xc == 77 && yc >= 12 && yc < 17)) {
				gotoxy(xc, yc);
				printf("%c", 111);
				cambiarVelocidad();//Cambia la velocidad cada vez que come el snake..	
				break;
			}
			
			
		} while (true);
		}
	
	
}


//-----------------------puntos que se acomuan cada vez que come el snake-------------------------------
void puntosSnake()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	gotoxy(3, 1);
	printf("Puntaje %d", puntaje);
}

int menuOpciones(void)
{
	if(kbhit())
	{
		tecla = getch();
		if(tecla == ARRIBA)
		{
			gotoxy(30, 12);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
			printf("%c   JUGAR DE NUEVO", 16);
			gotoxy(30, 15);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			printf("    SALIR DEL JUEGO");
			_asm
			{
				MOV posicion, 1
			}
		}
		if(tecla == ABAJO)
		{
			gotoxy(30, 12);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			printf("    JUGAR DE NUEVO");
			gotoxy(30, 15);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
			printf("%c   SALIR DEL JUEGO", 16);
			_asm
			{
				MOV posicion, 0
			}
		}
		if(tecla == ENTER)
			return 1;
	}
	return 0;
}

void escogerOpcion()
{
	borrarCuerpo();
	gotoxy(xc, yc);
	printf(" ");
	gotoxy(35, 1);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	printf(" PRESS ENTER");
	gotoxy(30, 12);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	printf("%c   JUGAR DE NUEVO", 16);
	gotoxy(30, 15);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	printf("    SALIR DEL JUEGO");
	do
	{
		menuOpciones();
	}
	while(!menuOpciones());
	gotoxy(35, 1);
	printf("            ");
	gotoxy(30, 12);
	printf("                    ");
	gotoxy(30, 15);
	printf("                    ");
}

void iniciar_datos()
{
	for(int j = tamanio;j > 0;j--)
	{
		gotoxy(cuerpo[j][0], cuerpo[j][1]);
		printf(" ");
	}
	for(int j = 0;j <= tamanio;j++)
	{
		for(int i = 0;i < 2;i++)
		{
			cuerpo[j][i] = '\0';
		}
	}
	_asm
	{
		mov puntaje, 0//Establece el puntaje
		mov x, 3//Se establece donde empezara el snake.
		mov y, 5
		mov tamanio, 3//Establece el tamaño del snake al iniciar.
		mov direccion, 3//Establece la direccion en 3 que incrementa en x
		mov velocidad, 100//Establece la velocidad...
		mov h, 1//Este servira para establecer a cada cuanto de score aumentara la velocidad..
		mov n, 0//Puntero de cuerpo---
	}
}
void laberinto() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);//Este maneja el color de las lineas.
	for (int i = 2; i < 100; i++) //Dibuja el limite del snake superior e inferior horizontal
	{
		
		gotoxy(i, 3);
		printf("%c", 223);
		gotoxy(i, 23);
		printf("%c", 223);
		
	}
	for (int i = 4; i < 23; i++)//Dibuja el limite del snake derecho e izquierdo 
	{
		

		gotoxy(2, i);
		printf("%c", 219);
		gotoxy(100, i);
		printf("%c", 219);
	}
	for (int x = 3; x < 20; x++) {
		gotoxy(20, x);
		printf("%c", 219);
	}
	for (int x = 10; x < 20; x++) {
		gotoxy(x, 19);
		printf("%c", 219);
	}
	for (int x = 3; x < 13; x++) {
		gotoxy(x, 9);
		printf("%c", 223);
	}

	//Segunda parte
	for (int x = 21; x < 50; x++) {
		gotoxy(x, 10);
		printf("%c", 223);
	}
	for (int x = 21; x < 40; x++) {
		gotoxy(x, 7);
		printf("%c", 223);
	}
	for (int x = 5; x < 10; x++) {
		gotoxy(49, x);
		printf("%c", 219);
	}
	//-------tercer parte-----------------
	for (int x = 21; x < 40; x++) {
		gotoxy(x, 17);
		printf("%c", 223);
	}
	for (int x = 30; x < 40; x++) {
		gotoxy(x, 14);
		printf("%c", 223);
	}
	for (int x = 14; x < 17; x++) {
		gotoxy(30, x);
		printf("%c", 219);
	}
	//Cuarta parte..........
	for (int x = 8; x < 23; x++) {
		gotoxy(65, x);
		printf("%c", 219);
	}//
	for (int x = 66; x < 90; x++) {
		gotoxy(x, 8);
		printf("%c", 223);
	}
	for (int x = 8; x < 18; x++) {
		gotoxy(90, x);
		printf("%c", 219);
	}//
	for (int x = 77; x < 90; x++) {
		gotoxy(x, 17);
		printf("%c", 219);
	}//
	for (int x = 12; x < 17; x++) {
		gotoxy(77, x);
		printf("%c", 219);
	}	
}
bool comprobar() {
	return y == 3 || y == 23 || y>3 && y < 20 && x == 20 || x == 2 || x == 100
		|| x >= 10 && x <= 20 && y == 19 || y == 9 && x>3 && x < 13
		|| y == 10 && x >= 21 && x < 50 || y == 7 && x >= 21 && x < 40 || x == 49 && y >= 5 && y < 10
		|| y == 17 && x >= 21 && x < 40 || y == 14 && x >= 30 && x < 40 || x == 30 && y >= 14 && y < 17
		|| x == 65 && y >= 8 && y < 23 || y == 8 && x >= 66 && x < 90 || x == 90 && y >= 8 && y < 18 || y == 17 && x >= 77 && x < 90
		|| x == 77 && y >= 12 && y < 17;
}

bool gameOver(){
	if (comprobar())
		return false;
	for (int j = tamanio - 1; j > 0; j--)
	{
		if (cuerpo[j][0] == x && cuerpo[j][1] == y)
			return false;
	}
	return true;
}

int main()
{
	srand(time(NULL));//Genera una secuanecia de numeros aleatorios... 
	dibujar();//Dibuja el marco del snake.
	menuInicial();//Dibuja lo que va a aparecer en el snake.....
	do
	{
		iniciar_datos();
		laberinto();
		gotoxy(xc, yc);//Este inicializa la comida del snake
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);//Color de la comida
		printf("%c", 111);//Comida inicial
		do
		{
			puntosSnake();
			dibujarComida();
			cambiarVelocidad();
			borrarCuerpo();
			guardarPosicion();
			dibujarCuerpo();
			
			Sleep(velocidad);//Metodo que establece la velocidad....
			pulsarTecla();
			pulsarTecla();
			_asm
			{
				CMP direccion, 3 //Incrementa la posicion de x
				JE IncX
				CMP direccion, 1 //Decrementa la posicion de y
				JE DecY
				CMP direccion, 4 //Decrementa la posicion de x
				JE DecX
				CMP direccion, 2 //incrementa la posicion en y
				JE IncY
				IncX:
				INC x
				JMP Salir
				DecY:
				DEC y
				JMP Salir
				DecX:
				DEC x
				JMP Salir
				IncY:
				INC y
				JMP Salir
				Salir:
			}
		}
		while(tecla != ESC && gameOver());
		escogerOpcion();
	}
	while(posicion);
	return 0;
}