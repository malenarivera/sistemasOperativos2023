#include <xinu.h>
#include <stdio.h>
#include "titlescreen.h"
#include "playerImage.h"
#include "enemy.h"
#include "boss.h"
#include "gameover.h"
#include "shoot.h"

int puntajeOVida=0, puntaje=0, vidas=3;
int pidMatar, pidMostrar, pidJugar, pidPedirLetra;
char mensajePuntaje, mensajeVidas;

char tecla_actual;

typedef unsigned short u16;
#define RGB(r, g, b) (r | (g << 5) | (b << 10))
// #define REG_DISPCNT *(u16 *)0x4000000
#define extern videoBuffer
#define MODE3 3
#define BG2_ENABLE (1 << 10)
#define WHITE RGB(31, 31, 31)
#define BLACK RGB(0, 0, 0)
#define YELLOW RGB(255, 255, 0)

/*
#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)
#define tecla_actual==key)  (~(BUTTONS) & key)
*/
//#define BUTTONS *(volatile unsigned int *)0x4000130

#define BUTTON_A	0x24
#define BUTTON_B	0x25 
#define BUTTON_SELECT	0x03
#define BUTTON_START	0x2c // 'z'
#define BUTTON_RIGHT	0x20 // 'd'
#define BUTTON_LEFT	 0x1e	// 'a'
/*#define BUTTON_UP	0x11
#define BUTTON_DOWN 	0x1f	
#define BUTTON_R	'1'
#define BUTTON_L	'2'*/
#define BUTTON_ESC   0x1 // 'esc'
//#define tecla_actual==key)  (tecla_actual == key)

//variable definitions
#define playerspeed 2
#define enemyspeed 1
#define fastXSpeed 3
#define fastYSpeed 2


void setPixel(int x, int y, u16 color);
void drawRect(int x, int y, int width, int height, u16 color);
void drawHollowRect(int x, int y, int width, int height, u16 color);
void drawImage3(int x, int y, int width, int height, const u16* image);
void delay_galaga();
void waitForVBlank();

//helpers
void initialize();
void drawEnemies();
//void endGame();
int collision(u16 enemyX, u16 enemyY, u16 enemyWidth, u16 enemyHeight, u16 playerX, u16 playerY, u16 playerWidth, u16 playerHeight);
void actualizarEstado();
//objects
struct Players {
	volatile u16 playerX;
	volatile u16 playerY;
};
struct Enemy {
	volatile u16 enemyX;
	volatile u16 enemyY;
};
struct FastEnemy {
	volatile u16 fastX;
	volatile u16 fastY;
};

//ESTO LO MOVI
struct Enemy easyEnemies[9];
struct Enemy hardEnemies[9];

int shoots[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int curr_shot = 0;
#define N_SHOOTS 10


/*collision cambiado --> ahora devuelve 0 unicamente si no hubo colision en ningun sentido*/
int collision(u16 enemyX, u16 enemyY, u16 enemyWidth, u16 enemyHeight, u16 playerX, u16 playerY, u16  playerWidth, u16 playerHeight) {
if ( (enemyX > (playerX + playerWidth)) || 
    ((enemyX + enemyWidth) < playerX ) ||
   (enemyY > (playerY + playerHeight) ) ||
   ((enemyY + enemyHeight) < playerY ) ) {
   return 0;
}else
    return 1;   
}

int jugar(void){
	vidas = 3;
	puntaje = 0;
    for (int a = 0; a < 9; a++) {
		easyEnemies[a].enemyX = (28*a);
		easyEnemies[a].enemyY = 0;
	} 
	    easyEnemies[1].enemyX = 240;
	    easyEnemies[4].enemyX = 240;
	    easyEnemies[8].enemyX = 240;
	//difficult enemies setup
	for (int a = 0; a < 9; a++) {
		hardEnemies[a].enemyX = (28*a);
		hardEnemies[a].enemyY = 160;
	} 
	    hardEnemies[3].enemyX = 240;
	    hardEnemies[6].enemyX = 240;
	//player setup
	struct Players player;
	    player.playerX = 120;
	    player.playerY = 136;
	//fast enemy "boss" setup
	struct FastEnemy fast;
	    fast.fastX = 0;
	    fast.fastY = 30; 

	// REG_DISPCNT = MODE3 | BG2_ENABLE;
	//initalize title screen
	print_text_on_vga(10, 20, "GALAGA ");
	drawImage3(0, 0, 240, 160, titlescreen);

	while(1) {
		if (tecla_actual==BUTTON_START) {
			break;
		}
	}	
	//start black screen for drawing
	for (int i = 0; i < 240; i++) {
		for (int j = 0; j < 160; j++) {
			setPixel(i, j, BLACK);
		}
	}	
	while(1) {
		//go back to title screen if select button is pressed
		if (tecla_actual==BUTTON_SELECT) {
			//initialize();
			galaga();
		}
		//player shots 
		if (tecla_actual==BUTTON_A) {
			if (shoots[curr_shot] == 0) {
				shoots[curr_shot] = 136*240 + player.playerX+9; /* 24 widht player */
				curr_shot++;
				if (curr_shot >= N_SHOOTS)
					curr_shot = 0;
			};
		}
		//player movement input
		if ((tecla_actual==BUTTON_LEFT) && (player.playerX > 0)) {
			player.playerX -= playerspeed;
		}
		if ((tecla_actual==BUTTON_RIGHT) && (player.playerX <= 216)) {
			player.playerX += playerspeed;
		}
		/*
		if (tecla_actual==BUTTON_UP) && (player.playerY > 25)) {
			player.playerY -= playerspeed;
		}
		if (tecla_actual==BUTTON_DOWN) && (player.playerY <= 136)) {
			player.playerY += playerspeed;
		}
		*/
		
		//si quiere cortar el juego
		if (tecla_actual==BUTTON_ESC) {
			send(pidMatar, 3);
		}
		
		waitForVBlank();
		sleepms(50);
		//draw player
		drawImage3(player.playerX, player.playerY, 24, 24, playerImage);
		drawHollowRect(player.playerX - 1, player.playerY - 1, 26, 26, BLACK);
		drawHollowRect(player.playerX - 2, player.playerY - 2, 28, 28, BLACK);

		//draw easy enemies with downward movement
		for (int a = 0; a < 9; a++) {
			easyEnemies[a].enemyY += enemyspeed;
			drawImage3(easyEnemies[a].enemyX, easyEnemies[a].enemyY, 20, 20, enemy);
			if (collision(easyEnemies[a].enemyX, easyEnemies[a].enemyY, 20, 20, player.playerX, player.playerY,24,24)) {

					vidas=vidas-1;
					actualizarEstado(2,1,a);
					if(vidas==0){
						send(pidMatar,3);
					}

			}	
			if (easyEnemies[a].enemyY >= 160) {
				easyEnemies[a].enemyY = 0;
			}		
		}

		//draw hard enemies
		for (int a = 0; a < 9; a++) {
			hardEnemies[a].enemyY += enemyspeed;
			drawImage3(hardEnemies[a].enemyX, hardEnemies[a].enemyY, 20, 20, enemy);
			if (collision(hardEnemies[a].enemyX, hardEnemies[a].enemyY, 20, 20, player.playerX, player.playerY,24,24)) {

				vidas=vidas-1;
				actualizarEstado(2,2,a);
				if(vidas==0){
					send(pidMatar,3);
				}


			}	
			if (hardEnemies[a].enemyY >= 228) {
				hardEnemies[a].enemyY = 0;
			}
			if ((hardEnemies[a].enemyY >= 200) && (easyEnemies[a].enemyY <=45)) {
				hardEnemies[a].enemyY = 160;
			}	
			//space enemies apart
			if ((hardEnemies[a].enemyY >= 200) && (easyEnemies[a].enemyY <=45)) {
				hardEnemies[a].enemyY = 160;
			}		
			if ((easyEnemies[a].enemyY >= 120) && (hardEnemies[a].enemyY >=170)) {
				hardEnemies[a].enemyY = 160;
			}							
		}	


		//draw shots
		for (int i = 0; i < N_SHOOTS; i++) {
			if (shoots[i] != 0) {
				drawRect((shoots[i] % 240), (shoots[i] / 240)+4, 5, 5, BLACK);
				drawImage3((shoots[i] % 240), (shoots[i] / 240), 5, 5, shoot);
				shoots[i] = shoots[i]-240*4;
				if (shoots[i] <=0)   shoots[i]=0;
				// check hits of shoots
				for (int j = 0; j < 9; j++) {
					if (collision(easyEnemies[j].enemyX, easyEnemies[j].enemyY, 20, 20, shoots[i] % 240, shoots[i] / 240,5,5)) {
						drawRect(easyEnemies[j].enemyX, easyEnemies[j].enemyY,  20, 20, BLACK);
						drawRect((shoots[i] % 240), (shoots[i] / 240)+4, 5, 5, BLACK);
						easyEnemies[j].enemyY = 0;
						shoots[i] = 0;

						puntaje+=100;
						actualizarEstado(1,0,0);


					}

					if (collision(hardEnemies[j].enemyX, hardEnemies[j].enemyY, 20, 20, shoots[i] % 240, shoots[i] / 240,5,5)) {
						drawRect(hardEnemies[j].enemyX, hardEnemies[j].enemyY,  20, 20, BLACK);
						drawRect((shoots[i] % 240), (shoots[i] / 240)+4, 5, 5, BLACK);
						hardEnemies[j].enemyY = 0;
						shoots[i] = 0;

						puntaje+=500;//Matar enemigos dificiles da mas puntos
						actualizarEstado(1,0,0);


					}
				}
			}
		}
		
	}	
	return 0;
}

void actualizarEstado(int cambio,int tipoEnemigo, int posicionArreglo){
	//Tipo de enemigo = si es 1 es easy, si es 2 es hard
	puntajeOVida=cambio;
	if(cambio == 2){
		if(tipoEnemigo == 1){
			drawRect(easyEnemies[posicionArreglo].enemyX, easyEnemies[posicionArreglo].enemyY, 20, 20, BLACK);
			easyEnemies[posicionArreglo].enemyX= -1; //Lo saca de la pantalla
		}else{
			drawRect(hardEnemies[posicionArreglo].enemyX, hardEnemies[posicionArreglo].enemyY, 20, 20, BLACK);
			hardEnemies[posicionArreglo].enemyX= -1; //Lo saca de la pantalla
		}
	}
	send(pidMostrar, 2);
	if(receive()==1){}
}

void mostrarVidasPuntaje(){
while(1){
    print_text_on_vga(260, 40, sprintf(mensajeVidas, "VIDAS RESTANTES:  %d", vidas ));
    print_text_on_vga(260, 20, sprintf(mensajePuntaje, "PUNTAJE:  %d", puntaje ));
	if(receive()==2){	

	/*si la variable es 1 -> gana puntos*/
	/*si la variable es 2 -> perdio una vida*/
	if(puntajeOVida==1){
		print_text_on_vga(260, 20, sprintf(mensajePuntaje, "PUNTAJE:  %d", puntaje ));	
	}else{
		print_text_on_vga(260, 40, sprintf(mensajeVidas, "VIDAS RESTANTES:  %d", vidas ));
	}
	send(pidJugar, 1);
	}	
}

}

void limpiarPantalla(){
	drawRect(0, 0, 240, 180, YELLOW);
	print_text_on_vga(260, 20, sprintf(mensajePuntaje, "                 "));
	print_text_on_vga(260, 40, sprintf(mensajeVidas, "                   "));
}

void endGame() {
	//start Game Over State
	drawImage3(0, 0, 240, 160, gameover);
	drawHollowRect(0, 0, 240, 160, WHITE);
	int esperando = 1;
	while(esperando) {
		if (tecla_actual==BUTTON_SELECT) {
			empezarJugar();
		}
		if (tecla_actual==BUTTON_START)	{
			empezarJugar();
		}
		if(tecla_actual==BUTTON_ESC){
			esperando = 0;
			limpiarPantalla();
		}	
	}
}

void pedirLetra(){
	while(1){
	open(KEYBOARD,0,0);
	read(KEYBOARD,&tecla_actual,1);
	close(KEYBOARD);
	}	
}


void empezarJugar(){
	pidJugar= create(jugar, 1024, 20, "proceso 1",0);
	pidMostrar= create (mostrarVidasPuntaje, 1024, 20, "proceso 2", 0);
	pidPedirLetra= create(pedirLetra, 1024, 20, "proceso n que pide letras", 0);
	resume(pidPedirLetra);
	resume(pidJugar);
	resume(pidMostrar);

	if(receive()==3){
	kill(pidJugar);
	kill(pidMostrar);
	if(vidas==0){
		endGame();
	}else{
		limpiarPantalla();
	}
	}	

}

void galaga(void) {
	pidMatar= create(empezarJugar, 1024, 20, "proceso control",0);
	resume(pidMatar);
}
