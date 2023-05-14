#include <beeper.h>

void beeperSongs(int songNum) {
    switch (songNum) {
        case (1):
            zelda();
            break;
        case (2):
            FFVTheme();
            break;
        case (3):
            tetrisIntro();
            break;
        case (4):
            mortalKombat();
            break;
        default:
            printf("Song available are between 1 and 4");
            break;
    }
}

void beepSample() {
    call_to_beep(1000, 1000);
 }

void bootSound() {
	call_to_beep(261, 250);
    call_to_beep(392, 500);
}

void victory() {
    call_to_beep(261, 200); sleepms(50);
    call_to_beep(294, 125);
    call_to_beep(392, 625);
}

// canciones obtenidas de los repositorios: https://github.com/ShaneMcC/call_to_beeps y https://github.com/NaWer/call_to_beep

void zelda() {
    call_to_beep(880   ,500 ); sleepms(70);     call_to_beep(587.33 , 1000); sleepms(70); 
    call_to_beep(698.46,500 ); sleepms(70);     call_to_beep(880    , 500 ); sleepms(70); 
    call_to_beep(587.33,1000); sleepms(70);     call_to_beep(698.46 , 500 ); sleepms(70); 
    call_to_beep(880   ,250 ); sleepms(70);     call_to_beep(1046.50, 250 ); sleepms(70); 
    call_to_beep(987.77,500 ); sleepms(70);     call_to_beep(783.99 , 500 ); sleepms(70); 
    call_to_beep(698.46,250 ); sleepms(70);     call_to_beep(783.99 , 250 ); sleepms(70); 
    call_to_beep(880   ,500 ); sleepms(70);     call_to_beep(587.33 , 500 ); sleepms(70); 
    call_to_beep(523.25,250 ); sleepms(70);     call_to_beep(659.26 , 250 ); sleepms(70); 
    call_to_beep(587.33,750);
}

void FFVTheme() {
    call_to_beep(987,53 );  sleepms( 53 );    call_to_beep(987,53 );  sleepms( 53 );
    call_to_beep(987,53 );  sleepms( 53 );    call_to_beep(987,428);                
    call_to_beep(784,428);                    call_to_beep(880,428);                
    call_to_beep(987,107);  sleepms( 214);    call_to_beep(880,107);                
    call_to_beep(987,857);                    call_to_beep(740,428);                
    call_to_beep(659,428);                    call_to_beep(740,428);                
    call_to_beep(659,107);  sleepms( 107);    call_to_beep(880,428);                
    call_to_beep(880,107);  sleepms( 107);    call_to_beep(830,428);                
    call_to_beep(880,107);  sleepms( 107);    call_to_beep(830,428);                
    call_to_beep(830,107);  sleepms( 107);    call_to_beep(740,428);                
    call_to_beep(659,428);                    call_to_beep(622,428);                
    call_to_beep(659,107);  sleepms( 107);    call_to_beep(554,1714);                
    call_to_beep(740,428);                    call_to_beep(659,428);                
    call_to_beep(740,428);                    call_to_beep(659,107);  sleepms( 107);
    call_to_beep(880,428);                    call_to_beep(880,107);  sleepms( 107);
    call_to_beep(830,428);                    call_to_beep(880,107);  sleepms( 107);
    call_to_beep(830,428);                    call_to_beep(830,107);  sleepms( 107);
    call_to_beep(740,428);                    call_to_beep(659,428);                
    call_to_beep(740,428);                    call_to_beep(880,107);  sleepms( 107);
    call_to_beep(987,1714);                
}

void tetrisIntro() {
    call_to_beep(330,150);    call_to_beep(1  ,40 );    call_to_beep(494,159);    call_to_beep(1  ,40 );
    call_to_beep(660,150);    call_to_beep(1  ,40 );    call_to_beep(590,150);    call_to_beep(660,150);
    call_to_beep(494,100);    call_to_beep(494,100);    call_to_beep(523,150);    call_to_beep(1  ,40 );
    call_to_beep(440,150);    call_to_beep(1  ,40 );    call_to_beep(494,150);    call_to_beep(1  ,40 );
    call_to_beep(392,100);    call_to_beep(392,100);    call_to_beep(440,150);    call_to_beep(370,150);
    call_to_beep(1  ,40 );    call_to_beep(392,150);    call_to_beep(1  ,40 );    call_to_beep(330,100);
    call_to_beep(330,100);    call_to_beep(370,150);    call_to_beep(1  ,40 );    call_to_beep(294,150);
    call_to_beep(1  ,40 );    call_to_beep(330,150);    call_to_beep(247,100);    call_to_beep(247,100);
    call_to_beep(261,150);    call_to_beep(1  ,40 );    call_to_beep(311,150);    call_to_beep(1  ,40 );
    call_to_beep(330,150);    call_to_beep(1  ,40 );    call_to_beep(247,100);    call_to_beep(247,100);
    call_to_beep(262,150);    call_to_beep(1  ,40 );    call_to_beep(370,150);    call_to_beep(1  ,40 );
    call_to_beep(330,150);    call_to_beep(1  ,40 );    call_to_beep(494,159);    call_to_beep(1  ,40 );
    call_to_beep(660,150);    call_to_beep(1  ,40 );    call_to_beep(590,150);    call_to_beep(660,150);
    call_to_beep(494,100);    call_to_beep(494,100);    call_to_beep(523,150);    call_to_beep(1  ,40 );
    call_to_beep(440,150);    call_to_beep(1  ,40 );    call_to_beep(494,150);    call_to_beep(1  ,40 );
    call_to_beep(392,100);    call_to_beep(392,100);    call_to_beep(440,150);    call_to_beep(370,150);
    call_to_beep(1  ,40 );    call_to_beep(392,150);    call_to_beep(1  ,40 );    call_to_beep(330,100);
    call_to_beep(330,100);    call_to_beep(370,150);    call_to_beep(1  ,40 );    call_to_beep(294,150);
    call_to_beep(1  ,40 );    call_to_beep(330,150);    call_to_beep(247,100);    call_to_beep(247,100);
    call_to_beep(261,150);    call_to_beep(1  ,40 );    call_to_beep(311,150);    call_to_beep(1  ,40 );
    call_to_beep(330,150);    call_to_beep(1  ,40 );    call_to_beep(247,100);    call_to_beep(247,100);
    call_to_beep(262,150);    call_to_beep(1  ,40 );    call_to_beep(370,150);    call_to_beep(1  ,40 );
    call_to_beep(330,150);    call_to_beep(1  ,4);}


void mortalKombat() {
    call_to_beep( 220, 250); call_to_beep( 220, 250); call_to_beep( 261, 250); call_to_beep( 220, 250); 
    call_to_beep( 293, 250); call_to_beep( 220, 250); call_to_beep( 329, 250); call_to_beep( 293, 250); 
    call_to_beep( 261, 250); call_to_beep( 261, 250); call_to_beep( 329, 250); call_to_beep( 261, 250); 
    call_to_beep( 392, 250); call_to_beep( 261, 250); call_to_beep( 329, 250); call_to_beep( 261, 250); 
    call_to_beep( 196, 250); call_to_beep( 196, 250); call_to_beep( 246, 250); call_to_beep( 196, 250); 
    call_to_beep( 261, 250); call_to_beep( 196, 250); call_to_beep( 293, 250); call_to_beep( 261, 250); 
    call_to_beep( 174, 250); call_to_beep( 174, 250); call_to_beep( 220, 250); call_to_beep( 174, 250); 
    call_to_beep( 261, 250); call_to_beep( 174, 250); call_to_beep( 261, 250); call_to_beep( 246, 250); 
    call_to_beep( 220, 250); call_to_beep( 220, 250); call_to_beep( 261, 250); call_to_beep( 220, 250); 
    call_to_beep( 293, 250); call_to_beep( 220, 250); call_to_beep( 329, 250); call_to_beep( 293, 250); 
    call_to_beep( 261, 250); call_to_beep( 261, 250); call_to_beep( 329, 250); call_to_beep( 261, 250); 
    call_to_beep( 392, 250); call_to_beep( 261, 250); call_to_beep( 329, 250); call_to_beep( 261, 250); 
    call_to_beep( 196, 250); call_to_beep( 196, 250); call_to_beep( 246, 250); call_to_beep( 196, 250); 
    call_to_beep( 261, 250); call_to_beep( 196, 250); call_to_beep( 293, 250); call_to_beep( 261, 250); 
    call_to_beep( 174, 250); call_to_beep( 174, 250); call_to_beep( 220, 250); call_to_beep( 174, 250); 
    call_to_beep( 261, 250); call_to_beep( 174, 250); call_to_beep( 261, 250); call_to_beep( 246, 250); 
    call_to_beep( 220, 375); call_to_beep( 220, 375); call_to_beep( 220, 375); call_to_beep( 220, 375); 
    call_to_beep( 196, 250); call_to_beep( 261, 250); call_to_beep( 220, 375); call_to_beep( 220, 375); 
    call_to_beep( 220, 375); call_to_beep( 220, 375); call_to_beep( 196, 250); call_to_beep( 164, 250); 
    call_to_beep( 220, 375); call_to_beep( 220, 375); call_to_beep( 220, 375); call_to_beep( 220, 375); 
    call_to_beep( 196, 250); call_to_beep( 261, 250); call_to_beep( 220, 375); call_to_beep( 220, 375); 
    call_to_beep( 220, 375); call_to_beep( 220, 375); call_to_beep( 220, 375); call_to_beep( 220, 375); 
    call_to_beep( 220, 375); call_to_beep( 220, 375); call_to_beep( 220, 375); call_to_beep( 196, 250); 
    call_to_beep( 261, 250); call_to_beep( 220, 375); call_to_beep( 220, 375); call_to_beep( 220, 375); 
    call_to_beep( 220, 375); call_to_beep( 196, 250); call_to_beep( 164, 250); call_to_beep( 220, 375); 
    call_to_beep( 220, 375); call_to_beep( 220, 375); call_to_beep( 220, 375); call_to_beep( 196, 250); 
    call_to_beep( 261, 250); call_to_beep( 220, 375); call_to_beep( 220, 375); call_to_beep( 220, 375); 
    call_to_beep( 220, 375); call_to_beep( 220, 375); call_to_beep( 220, 125); call_to_beep( 329, 250); 
    call_to_beep( 220, 125); call_to_beep( 261, 250); call_to_beep( 220, 125); call_to_beep( 246, 250); 
    call_to_beep( 220, 125); call_to_beep( 261, 250); call_to_beep( 220, 125); call_to_beep( 246, 125); 
    call_to_beep( 196, 250); call_to_beep( 220, 125); call_to_beep( 329, 250); call_to_beep( 220, 125); 
    call_to_beep( 261, 250); call_to_beep( 220, 125); call_to_beep( 246, 250); call_to_beep( 220, 125); 
    call_to_beep( 261, 250); call_to_beep( 220, 125); call_to_beep( 246, 125); call_to_beep( 196, 250); 
    call_to_beep( 220, 125); call_to_beep( 329, 250); call_to_beep( 220, 125); call_to_beep( 261, 250); 
    call_to_beep( 220, 125); call_to_beep( 246, 250); call_to_beep( 220, 125); call_to_beep( 261, 250); 
    call_to_beep( 220, 125); call_to_beep( 246, 125); call_to_beep( 196, 250); call_to_beep( 220, 125); 
    call_to_beep( 329, 250); call_to_beep( 220, 125); call_to_beep( 261, 250); call_to_beep( 196, 125); 
    call_to_beep( 196, 250); call_to_beep( 196, 125); call_to_beep( 220, 250); call_to_beep( 220, 125); 
    call_to_beep( 329, 250); call_to_beep( 220, 125); call_to_beep( 261, 250); call_to_beep( 220, 125); 
    call_to_beep( 246, 250); call_to_beep( 220, 125); call_to_beep( 261, 250); call_to_beep( 220, 125); 
    call_to_beep( 246, 125); call_to_beep( 196, 250); call_to_beep( 220, 125); call_to_beep( 329, 250); 
    call_to_beep( 220, 125); call_to_beep( 261, 250); call_to_beep( 220, 125); call_to_beep( 246, 250); 
    call_to_beep( 220, 125); call_to_beep( 261, 250); call_to_beep( 220, 125); call_to_beep( 246, 125); 
    call_to_beep( 196, 250); call_to_beep( 220, 125); call_to_beep( 329, 250); call_to_beep( 220, 125); 
    call_to_beep( 261, 250); call_to_beep( 220, 125); call_to_beep( 246, 250); call_to_beep( 220, 125); 
    call_to_beep( 261, 250); call_to_beep( 220, 125); call_to_beep( 246, 125); call_to_beep( 196, 250); 
    call_to_beep( 220, 125); call_to_beep( 329, 250); call_to_beep( 220, 125); call_to_beep( 261, 250); 
    call_to_beep( 196, 125); call_to_beep( 196, 250); call_to_beep( 196, 125); call_to_beep( 220, 25);
}


void mario() {
    call_to_beep(130 , 100); call_to_beep(262 , 100); call_to_beep(330 , 100); call_to_beep(392 , 100); 
    call_to_beep(523 , 100); call_to_beep(660 , 100); call_to_beep(784 , 300); call_to_beep(660 , 300);
    call_to_beep(146 , 100); call_to_beep(262 , 100); call_to_beep(311 , 100); call_to_beep(415 , 100);
    call_to_beep(523 , 100); call_to_beep(622 , 100); call_to_beep(831 , 300); call_to_beep(622 , 300);
    call_to_beep(155 , 100); call_to_beep(294 , 100); call_to_beep(349 , 100); call_to_beep(466 , 100);
    call_to_beep(588 , 100); call_to_beep(699 , 100); call_to_beep(933 , 300); call_to_beep(933 , 100);
    call_to_beep(933 , 100); call_to_beep(933 , 100); call_to_beep(1047, 400);
}