/* Curse of the lost miner*/
extern unsigned char  CLM_DATA_CAVES;
extern unsigned char  CLM_DATA_CHARSET1;
extern unsigned char  CLM_DATA_CHARSET2;
extern unsigned char  CLM_DATA_CAVEDL;
extern unsigned char  CLM_DATA_PMGAREA;
extern unsigned char  CLM_DATA_CAVESCREEN;
extern unsigned char  CLM_DATA_STATUSBARSCREEN;

/*Caves*/
#define MAX_CAVE_INDEX (26)
#define NUMBER_OF_CAVES (27)
#define CAVESIZE (222)

/*Control speed*/
#define CTRL_DELAY (4)

/*Joystick*/
#define JS_LEFT (11)
#define JS_TOPLEFT (10)
#define JS_TOPRIGHT (6)
#define JS_RIGHT (7)
#define JS_UP (14)
#define JS_DOWN (13)
#define JS_C (15)


/*Movement when miner is falling*/
#define FALL_FLAG_NONE (0)
#define FALL_FLAG_LEFT (1)
#define FALL_FLAG_RIGHT (2)
#define FALL_FLAG_FALLING (4)
#define FALL_FLAG_LEFT_AND_RIGHT (3)

/*Game over type*/
#define GAME_OVER_NONE  (0)
#define GAME_OVER_DEATH  (1)
#define GAME_OVER_SUCCESS  (2)
#define GAME_OVER_USER_QUIT (3)

/*Game type*/
#define GAME_TYPE_NORMAL (0)

/*Game speed*/
#define GAME_SPEED_NORMAL (0)
#define GAME_SPEED_SLOW (1)

#include <stdio.h>
#include <conio.h>
#include <peekpoke.h>
#include <stdlib.h>
#include <string.h>
#include <atari.h>


/*Main game routine*/
void doGame();


/*Caves and cave elements*/
void paintElement(unsigned char x, unsigned char y, unsigned char elem);
void paintCave();
void rebuildCaveElementArray(unsigned char cv);

/*Miner - PMG*/
void pmgInit();
void setMinerPos(unsigned char x, unsigned char y);
void repaintMiner();

/*Time and timing*/
void delay(unsigned int w);
void adjustGameSpeed(unsigned char tvSystem);

/*Display enable and disable*/
void enableDisplay();
void disableDisplay();

/*Cave graphics mode display*/
void enterCaveGraphicsMode();

/*Text mode displays*/
void enterTextMode();
void displayCongratulations();
void displayGameOver();
void displayReturnToMenuScreen();

void displayMainMenu();
void displayPostLevelScreen();

/*SFX and music routines*/
void rmtAllStop();
void rmtSuspend();
void rmtResume();
void rmtSetVBI();
void rmtRestoreVBI();

#define MUSIC_MENU (0)
#define MUSIC_GAME_OVER (0x0B)
#define MUSIC_DUMMY (0x0E)

void playSongLine(unsigned char sl);
void rmtPlaySongLine();

#define SFX_DIAMOND (10)
#define SFX_ALL_PICKED (12)
#define SFX_DEATH (14)
#define SFX_CONGRATULATIONS (16)
#define SFX_JUMP (18)
#define SFX_SCORE (20)

void playSFX(unsigned char sfxCode);
void rmtPlaySFX();






/*Reboot*/
void asmReboot();

/*Miner movement*/
unsigned char moveLeft();
unsigned char moveRight();
void moveUp();
void moveDown();
unsigned char jumpUp(); /*Return 0 if OK, 1 if death*/
void fallDown();
void handleHighJump();
void updateStatusBar();
unsigned char checkTreasure();
void checkDeath();

unsigned char maxCaveReached; /*Max. warp*/
unsigned char startingCave; /*Warp*/
unsigned char dmactlStore; /*DMA CTL shadow Store*/
unsigned char lives; /*Lives*/

unsigned char currentCave; /*Current cave*/
unsigned char diamondsInCave; /*How many diamonds in cave*/
unsigned char diamondsCollected; /*How many diamonds collected*/
unsigned char caveDeath; /*1 for death*/
unsigned char caveAllPicked; /*1 for all picked*/
unsigned char stayHere; /*Stay in cave*/
unsigned char gameOverType;
unsigned char gameSpeed; /*Game speed*/
unsigned char gameType; /*Game type, normal or training*/

/*Scoring*/
unsigned long int highScore;
unsigned long int currentScore;


/*Menu displaylist adress*/
unsigned char menuDl1;
unsigned char menuDl2;

/*Temporary variables for general use*/
unsigned char x1;
unsigned char y1;
unsigned int i1;
unsigned int i2;
unsigned char z1;

/*Current cave status*/
unsigned char caveElements[20][22];
unsigned char caveBroken [20][22];

/*Miner location*/
unsigned char minerX, minerY;
int p0x, p0y;

/*Miner - PMG P0. Normal miner and jumping miner*/
unsigned char minerDataNormal[] = {60, 126, 90, 219, 255, 195, 102, 60};
unsigned char minerDataJump[] = {60, 126, 90, 219, 255, 195, 126, 0};

/*"Training" literal*/
unsigned char trainingLiteral[] = {52, 50, 33, 41, 46, 41, 46, 39};

unsigned char* tvSystemString;

/*High jump*/
unsigned char hijs;
unsigned char hiJump;

/*Movement control - allocated in asm source*/
extern unsigned char mvDelay;
extern unsigned char dliHandler;

/*Colors changed in DLI*/
extern unsigned char colorStore1;
extern unsigned char colorStore2;

/*Pointer to current 'look and feel' of the miner*/
unsigned char* minerData = minerDataNormal;

/*Movement speed setup. The higher the number, the slower the movement*/
unsigned char brokenSpeed;
unsigned char hijumpSpeedA;
unsigned char hijumpSpeedB;
unsigned char controlDelay;
unsigned char fallSpeed;

/*Sounds and music*/
extern unsigned char requestedSFXCode;
extern unsigned char songLine;


/*Cave elements*/

/*These elements do not require translation*/
#define E_BLANK (0)
#define E_ROCK_FULL (1)
#define E_ROCK_TL (2)
#define E_ROCK_TR (3)
#define E_ROCK_BL (4)
#define E_ROCK_BR (5)
#define E_ROCK_UNSTABLE (6)
#define E_LADDER (7)
#define E_DEATH_BOTTOM_TOP (8)
#define E_DEATH_TOP_BOTTOM (9)

/*Diamonds. In external representation, there is one code. 
 *Internally, we have three types of diamonds
 */
#define EXT_E_DIAM (14)
#define E_DIAM_F (10)
#define E_DIAM_L (12)

/*Broken rock has one external code and 8 internals for different
 states of decay
 */
#define EXT_E_ROCK_BROKEN (15)
#define E_ROCK_BROKEN_F (13)
#define E_ROCK_BROKEN_L (20)

/*The skull is a display-only element*/
#define E_SKULL (21)
#define E_SKULL_2 (22)

/*Element attribute masks*/

/*                          0  0  0  0  0  0  0  0  0  0  1  1  1  1  1  1  1  1  1  1  2  
                            0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0 */
unsigned char passable[] = {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned char notJump [] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned char broken[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1};
/*                          B  R           R  U  L  D  D  D  D  D  B
 *                          L  O           O  N  A  E  E  I  I  I  R
 *                          A  C           C  S  D  A  A  A  A  A  O
 *                          N  K           K  T  D  D  D  M  M  M  K
 *                          K                       BT TB          1  2  3  4  5  6  7  8
 */

/*Map cave elements to character pairs*/
unsigned char elem2CharMap[] = {

    0, /*BLANK*/

    64 + 128, /*ROCK FULL*/

    72 + 128, /*ROCK TL*/
    74 + 128, /*ROCK TR*/
    70 + 128, /*ROCK BL*/
    68 + 128, /*ROCK BR*/

    90 + 128, /*ROCK UNSTABLE*/

    78, /*LADDER*/

    66 + 128, /*DEATH BOTTOM TOP*/
    76 + 128, /*DEATH TOP BOTTOM*/

    84, /*DIAM 1*/
    86, /*DIAM 2*/
    88, /*DIAM 3*/

    97 + 128, /*BROKEN 1*/
    99 + 128, /*BROKEN 2*/
    101 + 128, /*BROKEN 3*/
    103 + 128, /*BROKEN 4*/
    105 + 128, /*BROKEN 5*/
    107 + 128, /*BROKEN 6*/
    109 + 128, /*BROKEN 7*/
    111 + 128, /*BROKEN 8*/

    80, /*SKULL*/
    82 /*SKULL 2*/
};

int main() {

    POKE(559,0);
    delay(1);
    
    /*Initialize*/
    gameOverType = GAME_OVER_NONE;
    clrscr();
    maxCaveReached = 0;
    startingCave = 0; 
    gameSpeed = GAME_SPEED_NORMAL;

    /*Force COLD start after RESET*/
    POKE(580, 1);

    if (get_tv() == AT_PAL) {
        tvSystemString = "PAL ";
    } else {
        tvSystemString = "NTSC";
    }

    /*Initialize our VBI routine*/
    rmtSuspend();
    rmtSetVBI();

    menuDl1 = PEEK(560);
    menuDl2 = PEEK(561);

    cursor(0);
    POKE(731, 1);

    /*Init the PMG*/
    pmgInit();
    POKE(559,62);

    /*Enclosing loop*/
    while (1) {

        /*Switch to text mode*/
        enterTextMode();

        /*How the game ended before?*/
        switch (gameOverType) {

            case GAME_OVER_DEATH:
            {
                rmtSuspend();
                playSongLine(MUSIC_GAME_OVER);
                rmtResume();
                displayGameOver();
                break;
            }

            case GAME_OVER_SUCCESS:
            {
                rmtSuspend();
                playSongLine(MUSIC_DUMMY);
                rmtResume();
                displayCongratulations();
                break;
            }
            case GAME_OVER_USER_QUIT:
            {
                rmtSuspend();
                playSongLine(MUSIC_DUMMY);
                rmtResume();
                displayReturnToMenuScreen();
                break;
            }

        }

        disableDisplay();

        /*Menu music*/
        rmtSuspend();
        playSongLine(MUSIC_MENU);
        rmtResume();

        /*Draw main menu*/
        setMinerPos(1, 6);
        displayMainMenu();

        /*Reset keys and joystick input*/
        if (kbhit()) cgetc();
        while (PEEK(644) == 0 || PEEK(632) != JS_C) {
        }


        /*The menu loop*/
        while (1) {

            /*Start game ?*/
            if (PEEK(644) == 0) {
                POKE(77, 0);
                gameType = GAME_TYPE_NORMAL;
                break;
            }

            /*ESC - quit*/
            if (kbhit()) {
                x1 = cgetc();
                if (x1 == 27) {
                    rmtAllStop();
                    rmtRestoreVBI();
                    asmReboot();
                    return 0;
                }

            }


        }
        /*Set gameover*/
        gameOverType = GAME_OVER_NONE;

        /*Set game speed*/
        adjustGameSpeed(get_tv());


        /*Setup the music*/
        rmtSuspend();
        rmtAllStop();
        playSongLine(MUSIC_DUMMY);
        rmtResume();

        /*Game routine*/
        doGame();

    }/*Enclosing loop*/
}

void doGame() {

    /*Joystick*/
    unsigned char js;
    unsigned char strig;

    /*Elements near miner*/
    unsigned char probeBelow;
    unsigned char probeMiner;

    /*Gravity*/
    unsigned char fallTimer;
    unsigned char fallCounter;
    unsigned char fallLength;
    unsigned char fallMovementFlags;
    unsigned char landLock;


    /*Broken rock timer*/
    unsigned char breakTimer;

    /*Quit flag*/
    unsigned char caveQuit;

    /*High jump in progress flag*/
    unsigned char hijump;



    /*Set current cave and number of lives and score*/
    currentCave = startingCave;
    currentScore = 0L;
    lives = 12;

    /*Cave loop*/
    while (1) {

        /*Setup screen*/
        enterCaveGraphicsMode();

        /*Rebuild cave aray*/
        rebuildCaveElementArray(currentCave);
        diamondsCollected = 0;

        /*Paint the cave and update status bar*/
        if ((currentCave & 0x03) < 2) {
            if (get_tv() == AT_PAL) {
                POKE(709, 12); /*White*/
                POKE(710, 134); /*Blue*/
                POKE(708, 242); /*Dark brown*/
                POKE(711, 244); /*Lighter brown*/
            } else {
                POKE(709, 12); /*White*/
                POKE(710, 0x96); /*Blue*/
                POKE(708, 0x32); /*Dark brown*/
                POKE(711, 0x34); /*Lighter brown*/
            }

        } else {
            if (get_tv() == AT_PAL) {
                POKE(709, 12); /*White*/
                POKE(710, 184); /*Green*/
                POKE(708, 84); /*Dark brown*/
                POKE(711, 102); /*Lighter brown*/
            } else {
                POKE(709, 12); /*White*/
                POKE(710, 0xD8); /*Green*/
                POKE(708, 0x54); /*Dark brown*/
                POKE(711, 0x56); /*Lighter brown*/
            }
        }

        /*Store the colors for DLI routine*/
        colorStore1 = PEEK(709);
        colorStore2 = PEEK(710);


        paintCave();
        updateStatusBar();

        /*Place the miner*/
        minerData = minerDataNormal;
        setMinerPos(minerX, minerY);



        /*Initialize game status variables*/
        stayHere = 1;
        fallCounter = 0;
        caveDeath = 0;
        caveAllPicked = 0;
        hijump = 0;
        fallLength = 0;
        fallMovementFlags = FALL_FLAG_NONE;
        landLock = 0;
        caveQuit = 0;
        probeBelow = E_ROCK_FULL;

        enableDisplay();

        /*Reset both timers*/
        POKE(20, 0);
        POKE(19, 0);

        /*Controls and physics loop*/
        while (stayHere) {

            /*Check CONSOLE keys*/
            if (PEEK(53279U) != 7) {
                /*START Key - Return to main menu*/
                if (PEEK(53279U) == 6) {
                    stayHere = 0;
                    caveQuit = 1;
                    POKE(77, 0);
                    break;
                }

                /*OPTION key - Commit Suicide*/
                if (PEEK(53279U) == 3) {
                    caveDeath = 1;
                    stayHere = 0;
                    POKE(77, 0);
                    break;
                }
            }

            /*Whats is behind the miner a what is below the miner?*/
            probeMiner = caveElements[minerX][minerY];
            probeBelow = caveElements[minerX][minerY + 1];

            /*Gravity - If there is nothing below the miner and the miner is not on a ladder, he falls down.*/
            if (passable[probeBelow] == 1 && probeMiner != E_LADDER && probeBelow != E_LADDER) {
                if (fallTimer != PEEK(20)) {
                    fallCounter++;
                    fallTimer = PEEK(20);
                    if (fallCounter == fallSpeed) {
                        fallDown();
                        fallLength++;
                        checkDeath();
                        fallCounter = 0;
                        if (fallLength > 6) {
                            stayHere = 0;
                            caveDeath = 1;
                            continue;
                        }
                    }
                }
                fallMovementFlags |= FALL_FLAG_FALLING;
                landLock = 0;
            } else {
                if ((fallMovementFlags & (FALL_FLAG_LEFT_AND_RIGHT)) != 0) {
                    landLock = 2;
                }
                fallCounter = 0;
                fallLength = 0;
                fallMovementFlags = FALL_FLAG_NONE;
            }

            /*There is a broken rock under the miner. It decays*/
            if (broken[probeBelow] == 1) {
                y1 = minerY + 1;
                if (breakTimer != PEEK(20)) {
                    caveBroken[minerX][y1]++;
                    breakTimer = PEEK(20);
                }
                if (caveBroken[minerX][y1] == brokenSpeed) {
                    caveBroken[minerX][y1] = 0;
                    if (probeBelow < E_ROCK_BROKEN_L) {
                        caveElements[minerX][y1]++;
                        paintElement(minerX, y1, caveElements[minerX][y1]);
                    } else {
                        caveElements[minerX][y1] = E_BLANK;
                        paintElement(minerX, y1, caveElements[minerX][y1]);
                    }
                }

            }

            /*Unstable rock under the miner*/
            if (probeBelow == E_ROCK_UNSTABLE) {
                caveElements[minerX][minerY + 1] = E_BLANK;
                paintElement(minerX, minerY + 1, E_BLANK);
            }

            /*Controls*/
            if (mvDelay == 0) {

                js = PEEK(632);
                strig = PEEK(644);

                switch (js) {

                        /*Joystick right*/
                    case JS_TOPRIGHT:
                    case JS_RIGHT:
                    {


                        /*With trigger - Medium jump to right*/
                        if (strig == 0 && !(notJump[probeBelow])) {
                            playSFX(SFX_JUMP);
                            POKE(77, 0);
                            /*Display miner as jumping one*/
                            minerData = minerDataJump;
                            fallLength = 0;
                            if (jumpUp()) break;
                            delay(CTRL_DELAY);
                            if (jumpUp()) break;
                            delay(CTRL_DELAY);
                            moveRight();
                            delay(CTRL_DELAY);
                            moveRight();
                            delay(CTRL_DELAY);
                            moveRight();
                            delay(CTRL_DELAY);
                            checkDeath();
                            /*Display miner as normal*/
                            minerData = minerDataNormal;
                            repaintMiner();
                            break;
                        }
                        /*Without trigger - Move to the right*/

                        /*When falling - allow move to the right just once*/
                        if ((fallMovementFlags & FALL_FLAG_FALLING) == FALL_FLAG_FALLING) {
                            if ((fallMovementFlags & FALL_FLAG_RIGHT) == 0) {
                                if (moveRight()) fallMovementFlags |= FALL_FLAG_RIGHT;
                            }
                        }/*Otherwise just move to the right*/
                        else {
                            if (landLock == 0) {
                                moveRight();
                            } else {
                                --landLock;
                                mvDelay = controlDelay;
                            }
                        }


                        POKE(77, 0);
                        checkDeath();
                        break;
                    }

                        /*Joystick left*/
                    case JS_TOPLEFT:
                    case JS_LEFT:
                    {
                        /*With trigger - Medium jump to the left*/
                        if (strig == 0 && !(notJump[probeBelow])) {
                            /*Medium jump to left*/
                            playSFX(SFX_JUMP);
                            POKE(77, 0);

                            /*Display miner as jumping one*/
                            minerData = minerDataJump;

                            fallLength = 0;
                            if (jumpUp()) break;
                            delay(CTRL_DELAY);
                            if (jumpUp()) break;
                            delay(CTRL_DELAY);
                            moveLeft();
                            delay(CTRL_DELAY);
                            moveLeft();
                            delay(CTRL_DELAY);
                            moveLeft();
                            delay(CTRL_DELAY);
                            checkDeath();

                            /*Display miner as normal one*/
                            minerData = minerDataNormal;
                            repaintMiner();
                            break;
                        }
                        /*Without trigger - Simply movet to the left*/

                        /*When falling - allow move to the left just once*/
                        if ((fallMovementFlags & FALL_FLAG_FALLING) == FALL_FLAG_FALLING) {
                            if ((fallMovementFlags & FALL_FLAG_LEFT) == 0) {
                                if (moveLeft()) fallMovementFlags |= FALL_FLAG_LEFT;
                            }
                        }/*Otherwise just move to the left*/
                        else {
                            if (!landLock) {
                                moveLeft();
                            } else {
                                --landLock;
                                mvDelay = controlDelay;
                            }
                        }
                        POKE(77, 0);
                        checkDeath();
                        break;
                    }


                        /*Joystick down, move down (ladder only)*/
                    case JS_DOWN:
                    {
                        moveDown();
                        POKE(77, 0);
                        checkDeath();
                        break;
                    }

                        /*Joystick up, move up (ladder only) or jump high*/
                    case JS_UP:
                    {
                        if (strig == 0) {
                            if (notJump[probeBelow]) break;
                            fallLength = 0;
                            playSFX(SFX_JUMP);
                            POKE(77, 0);
                            handleHighJump();
                            break;
                        }
                        moveUp();
                        POKE(77, 0);
                        checkDeath();
                        break;
                    }

                    default:
                    {
                        landLock = 0;
                        mvDelay = 0;
                    }

                }/*End switch js*/
            }

        }/*End of controls and physics loop*/

        /* Return to main menu by user request*/
        if (caveQuit) {
            /*Hide the miner*/
            setMinerPos(-8, 32);
            gameOverType = GAME_OVER_USER_QUIT;
            break;
        }

        /* Treasure collected. Advance to next cave or complete the game*/
        if (caveAllPicked) {

            /*Display cave score*/
            setMinerPos(-8, 32);
            enterTextMode();
            playSFX(SFX_ALL_PICKED);
            displayPostLevelScreen();

            currentCave++;
            if (currentCave == NUMBER_OF_CAVES) {
                /*Hide the miner*/
                setMinerPos(-8, 32);
                /*Show congratulations screen before menu*/
                gameOverType = GAME_OVER_SUCCESS;
                break;
            } else {
                if (currentCave > maxCaveReached) maxCaveReached = currentCave;
            }
            /*Play positive sound*/

            continue;
        }
        if (caveDeath) {

            /*Let the miner fall to the ground if possible*/
            if ((fallMovementFlags & FALL_FLAG_FALLING) == FALL_FLAG_FALLING) {
                while (minerY < 22 && (caveElements[minerX][minerY + 1] == E_BLANK)) {
                    minerY++;
                    setMinerPos(minerX, minerY);
                    delay(3);
                }
            }

            /*Let the miner dissapear*/
            setMinerPos(-8, 32);

            /*Display a skull with blinking eye*/
            paintElement(minerX, minerY, E_SKULL);
            playSFX(SFX_DEATH);
            delay(15);
            paintElement(minerX, minerY, E_SKULL_2);
            delay(15);
            paintElement(minerX, minerY, E_SKULL);
            delay(5);

            /*Check remaining lives lives*/
            if (lives == 0) {
                gameOverType = GAME_OVER_DEATH;
                break;
            }
            lives--;

            continue;
        }



    }/*End of outer loop*/

}

/*Paint element at specific location*/
void paintElement(unsigned char x, unsigned char y, unsigned char elem) {

    /*Target memory*/
    i2 = (unsigned)(&CLM_DATA_CAVESCREEN) + (y * 40)+(x << 1);

    /*Mapping for element*/
    z1 = elem2CharMap[elem];

    POKE(i2, z1);
    POKE(i2 + 1, z1 + 1);

}

/*Paint whole cave*/
void paintCave() {

    unsigned char x;
    unsigned char y;

    unsigned char* dMemPtr = (unsigned char*) &CLM_DATA_CAVESCREEN;

    for (y = 0; y < 22; ++y) {
        for (x = 0; x < 20; ++x) {
            z1 = elem2CharMap[caveElements[x][y]];
            *dMemPtr = z1;
            ++dMemPtr;
            *dMemPtr = z1 + 1;
            ++dMemPtr;
        }
    }

}

/*Rebuild cave*/
void rebuildCaveElementArray(unsigned char cv) {

    /*Elements*/
    unsigned char elems[2];
    int ec = 0;


    /*Element Pointer*/
    unsigned char* p;

    /*Coordinates*/
    unsigned char x, y;

    /*Determine character set (even cave or odd cave)*/
    if ((cv & 0x01U) != 0) {
        POKE(756, ((unsigned)(&CLM_DATA_CHARSET2))>>8);
    } else {
        POKE(756, ((unsigned)(&CLM_DATA_CHARSET1))>>8);
    }

    /*Point to the cave beginning*/
    p = (unsigned char*) &CLM_DATA_CAVES;
    p += cv*CAVESIZE;

    /*Reset number of diamonds in the cave*/
    diamondsInCave = 0;

    /*Determine miner position*/
    minerY = *p;
    p++;
    minerX = *p;
    p++;

    for (y = 0; y < 22; ++y) {
        for (x = 0; x < 20; x += 2) {

            elems[0] = (*(p) >> 4);
            elems[1] = (*(p)&0x0F);

            for (ec = 0; ec < 2; ec++) {

                /*Translate special elements*/
                switch (elems[ec]) {
                    case EXT_E_DIAM:
                    {
                        diamondsInCave++;
                        elems[ec] = E_DIAM_F + (diamondsInCave % 3);
                        break;
                    }
                    case EXT_E_ROCK_BROKEN:
                    {
                        elems[ec] = E_ROCK_BROKEN_F;
                        break;
                    }
                }
            }

            caveElements[x][y] = elems[0];
            caveElements[x + 1][y] = elems[1];
            p++;

        }
    }

    /*diamondsInCave=1;*/ /*@!!@*/

    /*Clear the broken array*/
    memset(caveBroken, 0, 440);
}

/*Player missile graphics*/
void pmgInit() {

    /*Set PMG memory start*/
    POKE(54279U, ((unsigned)(&CLM_DATA_PMGAREA))>>8);

    /*PMG single line resolution*/
    POKE(559, 62);

    /*Clear all PMG memory*/
    memset((void*) &CLM_DATA_PMGAREA, 0U, 2048);

    /*Enable PMG*/
    POKE(53277U, 2);

    /*Player 0 will be green*/
    if (get_tv() == AT_PAL) {
        POKE(704, 168);
    } else {
        POKE(704, 0xC8);
    }

    /*Initial coordinates*/
    p0x = 0;
    POKE(53248U, p0x);
    p0y = 0;

}

/*Place miner at given coordinates*/
void setMinerPos(unsigned char x, unsigned char y) {

    p0x = 48 + (x << 3);
    //memset(((unsigned char*) p0y + ((unsigned char*)&CLM_DATA_PMGAREA) + ((unsigned char*)1024), 0, 8);
    memset(p0y+&CLM_DATA_PMGAREA+1024,0,8);
    p0y = 32 + (y << 3);

    POKE(53248U, p0x);
    //memcpy((unsigned char*) p0y + ((unsigned char*)&CLM_DATA_PMGAREA) + ((unsigned char*))1024, minerData, 8);
    memcpy(p0y+&CLM_DATA_PMGAREA+1024, minerData, 8);
}

/*Just repaint the miner*/
void repaintMiner() {
    memcpy(((unsigned char*) (32 + (minerY << 3) + ((unsigned char*)&CLM_DATA_PMGAREA) + 1024)), minerData, 8);
}

/*Wait for some time*/
void delay(unsigned int w) {
    unsigned int i = 0;
    for (i = 0; i < w; i++) {
        unsigned char a = PEEK(20);
        while (PEEK(20) == a) {
        }
    }
}

/*Move commands with range and pass checking*/
unsigned char moveLeft() {
    if (minerX == 0 || passable[caveElements[minerX - 1][minerY]] == 0) return 0;
    minerX--;
    setMinerPos(minerX, minerY);
    checkTreasure();
    mvDelay = controlDelay;
    return 1;
}

unsigned char moveRight() {
    if (minerX == 19 || passable[caveElements[minerX + 1][minerY]] == 0) return 0;
    minerX++;
    setMinerPos(minerX, minerY);
    checkTreasure();
    mvDelay = controlDelay;
    return 1;
}

void moveDown() {
    if (minerY == 21) return;
    x1 = caveElements[minerX][minerY + 1];
    if (passable[x1] == 1) {
        minerY++;
        setMinerPos(minerX, minerY);
        checkTreasure();
        mvDelay = controlDelay;
    }
}

void fallDown() {
    if (minerY == 21) return;
    x1 = caveElements[minerX][minerY + 1];
    if (passable[x1] == 1) {
        minerY++;
        setMinerPos(minerX, minerY);
        checkTreasure();
    }
}

void moveUp() {
    if (minerY == 0) return;
    x1 = caveElements[minerX][minerY - 1];
    /*Not free*/
    if (passable[x1] == 0) return;

    /*We can move up only when we are on the ladder a passable element is above*/
    if (caveElements[minerX][minerY] == E_LADDER) {
        /*Into death*/
        if (x1 == E_DEATH_TOP_BOTTOM) {
            stayHere = 0;
            caveDeath = 1;
            return;
        }
        minerY--;
        setMinerPos(minerX, minerY);
        checkTreasure();
        mvDelay = controlDelay;

    }

}

unsigned char jumpUp() {
    if (minerY == 0) return 0;
    x1 = caveElements[minerX][minerY - 1];
    /*Into death*/
    if (x1 == E_DEATH_TOP_BOTTOM) {
        stayHere = 0;
        caveDeath = 1;
        return 1;
    }
    /*Not free*/
    if (passable[x1] == 0) return 0;

    minerY--;
    setMinerPos(minerX, minerY);
    checkTreasure();
    return 0;
}

/*This is special function to handle high jump.*/
void handleHighJump() {

    unsigned char hjMaxTicks = hijumpSpeedA;
    unsigned char hjTicks = 0;
    unsigned char hjFlipFlop;

    /*Jumping miner*/
    minerData = minerDataJump;


    y1 = 0; /*Side move flag*/
    hiJump = 3; /*Jump power - 3 steps*/
    mvDelay = 0; /*Reset Movement delay*/

    /*When there is still jump power*/
    while (hiJump > 0) {

        /*If the miner is blocked - jump is complete*/
        if (jumpUp()) break;

        /*Time window for side movement*/
        while (hjTicks < hjMaxTicks) {

            /*Time to allow controls*/
            if (mvDelay == 0) {
                hijs = PEEK(632);

                /*Allow only single left or right move during the jump*/
                switch (hijs) {
                    case (JS_TOPLEFT):
                    case (JS_LEFT):
                    {
                        if (y1) break;
                        if (moveLeft()) y1 = 1;
                        break;
                    }
                    case (JS_TOPRIGHT):
                    case (JS_RIGHT):
                    {
                        if (y1) break;
                        if (moveRight()) y1 = 1;
                        break;
                    }
                    default:
                    {
                        mvDelay = 0;
                        break;
                    }
                }/*End of switch hiJS*/

            }
            if (hjFlipFlop != PEEK(20)) {
                hjFlipFlop = PEEK(20);
                hjTicks++;
            }
        }
        if (hiJump == 2) hjMaxTicks = hijumpSpeedB;
        hjTicks = 0;
        hiJump--;
    }
    /*Jump is ended*/
    /*Check for death*/
    checkDeath();

    /*Normal miner*/
    minerData = minerDataNormal;
    repaintMiner();

}

void checkDeath() {
    x1 = caveElements[minerX][minerY + 1];
    if (x1 == E_DEATH_BOTTOM_TOP) {
        stayHere = 0;
        caveDeath = 1;
    }
}

unsigned char checkTreasure() {
    x1 = caveElements[minerX][minerY];
    if (x1 >= E_DIAM_F && x1 <= E_DIAM_L) {
        diamondsCollected++;
        caveElements[minerX][minerY] = E_BLANK;
        paintElement(minerX, minerY, E_BLANK);
        playSFX(SFX_DIAMOND);
        if (diamondsCollected == diamondsInCave) {
            stayHere = 0;
            caveAllPicked = 1;
        }
        return 1;
    }
    return 0;
}

void updateStatusBar() {

    /*Clear*/
    memset((unsigned char*) &CLM_DATA_STATUSBARSCREEN, 0, 40);

    /*Lives*/
    for (y1 = 0; y1 < lives; y1++) {
        POKE((unsigned)&CLM_DATA_STATUSBARSCREEN + y1, 123);
    }

    /*Current cave*/
    x1 = (40 - NUMBER_OF_CAVES) + currentCave + 1;
    for (y1 = 40 - NUMBER_OF_CAVES; y1 < x1; y1++) {
        POKE((unsigned)&CLM_DATA_STATUSBARSCREEN + y1, 96);
    }


}

void playSFX(unsigned char sfxCode) {
    requestedSFXCode = sfxCode;
    rmtPlaySFX();
}

void playSongLine(unsigned char sl) {
    songLine = sl;
    rmtPlaySongLine();
}

/*Display main menu*/
void displayMainMenu() {

    unsigned char px;

    clrscr();
    cursor(0);

    /*Game title*/
    revers(1);
    /*             123456789012345678901234567890123456789 */
    cputsxy(1, 1, "       CURSE OF THE  LOST MINER       ");
    cputsxy(1, 2, "    UNDERGROUND  MADNESS CHALLENGE    ");
    cputsxy(1, 3, "          BAHA Software 2020          ");
    revers(0);

    /*Lines above and below the title*/
    for (px = 1; px < 39; ++px) {
        cputcxy(px, 0, CH_HLINE);
        cputcxy(px, 4, CH_HLINE);
        cputcxy(px, 10, CH_HLINE);
        cputcxy(px, 14, CH_HLINE);
        cputcxy(px, 22, CH_HLINE);
    }


    /*Instructions*/
    cputsxy(2, 15, "Guide the miner through 27 dangerous");
    cputsxy(2, 16, "caves. Avoid obstacles and collect");
    cputsxy(2, 17, "all diamonds in each cave.");

    cputsxy(2, 19, "To maximize your score, complete the");
    cputsxy(2, 20, "caves quickly, leave broken rocks");
    cputsxy(2, 21, "intact, and do not lose lives.");

    cputsxy(35, 23, "v1.1");

    /*Description of menu controls*/
    cputsxy(2, 8, "Press");
    cputsxy(13, 8, "to start game");
    revers(1);
    cputsxy(8, 8, "FIRE");
    revers(0);

    /*Scores*/
    gotoxy(2, 11);
    cprintf("Last score: %07ld", currentScore);
    gotoxy(2, 12);
    cprintf("High score: %07ld", highScore);
    cputsxy(22, 11, tvSystemString);


    /*Reenable DMA*/
    enableDisplay();
}

void adjustGameSpeed(unsigned char tvSystem) {
    /*Normal game speed*/

    if (tvSystem == AT_PAL) {
        brokenSpeed = 14;
        hijumpSpeedA = 5;
        hijumpSpeedB = 17;
        controlDelay = 7;
        fallSpeed = 3;
    } else {
        brokenSpeed = 17;
        hijumpSpeedA = 6;
        hijumpSpeedB = 20;
        controlDelay = 9;
        fallSpeed = 4;
    }
}

/*Show Congratulations*/
void displayCongratulations() {

    clrscr(); /*1234567890123456789012345678901234567890*/
    cputsxy(2, 4, "Congratulations !");
    cputsxy(2, 6, "The curse of the lost miner");
    cputsxy(2, 7, "has been broken.");
    cputsxy(2, 11, "Thank you for playing this game.");
    cputsxy(2, 12, "Now you should create your own caves.");

    enableDisplay();

    delay(50);

    for (x1 = 0; x1 < 3; x1++) {
        playSFX(SFX_CONGRATULATIONS);
        delay(50);
    }

    delay(100);

    cputsxy(2, 20, "Press");
    revers(1);
    cputsxy(8, 20, "FIRE");
    revers(0);
    cputsxy(13, 20, "to continue");
    delay(10);

    while (PEEK(644)) {
    }


}

/*Show game over*/
void displayGameOver() {

    clrscr(); /*1234567890123456789012345678901234567890*/
    cputsxy(14, 8, "GAME IS OVER");

    revers(1);
    gotoxy(4, 11);
    cprintf("TOTAL SCORE              %07ld", currentScore);
    revers(0);

    /*Show the screen again*/
    enableDisplay();
    delay(150);

    cputsxy(9, 14, "Press");
    revers(1);
    cputsxy(15, 14, "FIRE");
    revers(0);
    cputsxy(20, 14, "to continue");

    while (PEEK(644)) {
    }

}

/*Display "Returning to the main menu ...*/
void displayReturnToMenuScreen() {

    /*Just blank screen*/
    clrscr();
    cputsxy(5, 8, "Returning to the main menu ...");

    /*Show score*/
    revers(1);
    gotoxy(4, 11);
    cprintf("TOTAL SCORE              %07ld", currentScore);
    revers(0);

    /*Show the screen again*/
    enableDisplay();

    /*Wait for a while*/
    delay(125);
}

/*Display post level screen*/
void displayPostLevelScreen() {

    unsigned int rawScoreTime = PEEK(20) + PEEK(19)*256;

    unsigned int scoreTime;
    unsigned int scoreBrokenRocks;
    unsigned int scoreLivesLeft;
    unsigned int scoreCave;
    unsigned int maxTime;
    unsigned int cx;
    unsigned int cy;
    unsigned int ce;
    unsigned int tDelay;

    if (get_tv() == AT_PAL) {
        tDelay = 16;
    } else {
        tDelay = 20;
    }


    clrscr();
    gotoxy(12, 2);
    cprintf("Cave %02d completed", currentCave + 1);
    enableDisplay();

    /*Determine maximum cave time*/
    if (currentCave > 12) {
        maxTime = 60 * 5 * 60;
    } else if (currentCave > 4) {
        maxTime = 60 * 3 * 60;
    } else {
        maxTime = 60 * 2 * 60;
    }

    /*Calculate time score*/
    if (rawScoreTime > maxTime) {
        scoreTime = 0;
    } else {
        scoreTime = (maxTime - rawScoreTime) / 2;
    }

    /*Calculate score for lives left*/
    scoreLivesLeft = 100 * lives;

    /*Calculate score for broken rocks left*/
    scoreBrokenRocks = 0;
    for (cx = 0; cx < 20; ++cx) {
        for (cy = 0; cy < 22; ++cy) {
            ce = caveElements[cx][cy];
            if (ce >= E_ROCK_BROKEN_F && ce <= E_ROCK_BROKEN_L) {
                scoreBrokenRocks += ((E_ROCK_BROKEN_L - ce)) + 1;
            }
        }
    }
    scoreBrokenRocks *= 4;

    /*Calculate total cave score and current score*/
    scoreCave = scoreTime + scoreLivesLeft + scoreBrokenRocks;
    currentScore += scoreCave;

    delay(tDelay * 2);

    /*Time score*/
    cputsxy(4, 6, "Time");
    gotoxy(30, 6);
    cprintf("%05d", scoreTime);
    playSFX(SFX_SCORE);
    delay(tDelay);

    /*Lives left */
    cputsxy(4, 8, "Lives left");
    gotoxy(30, 8);
    cprintf("%05d", scoreLivesLeft);
    playSFX(SFX_SCORE);
    delay(tDelay);

    /*Broken rocks */
    cputsxy(4, 10, "Broken rocks left");
    gotoxy(30, 10);
    cprintf("%05d", scoreBrokenRocks);
    playSFX(SFX_SCORE);
    delay(tDelay);

    /*Total cave score*/
    cputsxy(4, 12, "CAVE SCORE");
    gotoxy(30, 12);
    cprintf("%05d", scoreCave);
    playSFX(SFX_SCORE);
    delay(tDelay);


    revers(1);
    cputsxy(4, 15, "TOTAL SCORE");
    gotoxy(28, 15);
    cprintf("%07ld", currentScore);
    revers(0);
    playSFX(SFX_SCORE);
    delay(tDelay);

    if (currentScore > highScore) {
        highScore = currentScore;
        cputsxy(37, 15, "*");
    }

    /*Extra lives when cave 13 completed*/
    if (currentCave == 13 && lives < 12) {
        lives += 5;
        if (lives > 12) lives = 12;
        cputsxy(6, 18, "You are awarded  extra lives");
    }


    cputsxy(9, 20, "Press");
    revers(1);
    cputsxy(15, 20, "FIRE");
    revers(0);
    cputsxy(20, 20, "to continue");

    /*Clear any previous presses*/
    while (PEEK(644) == 0) {
    }

    /*Wait for fire*/
    while (PEEK(644)) {
    }


}

/*Enter text mode*/
void enterTextMode() {

    /*Disable display*/
    POKE(559, 0);
    delay(1);
    POKE(54286U, 96);

    /*Use text mode display list*/
    POKE(560, menuDl1);
    POKE(561, menuDl2);

    /*Set colors*/
    POKE(709, 14);
    if (get_tv() == AT_PAL) {
        POKE(710, 48);
    } else {
        POKE(710, 50);
    }

    /*Use text mode character set*/
    POKE(756, 224);
}

void enterCaveGraphicsMode() {

    /*Set display list and character set*/
    POKE(559, 0);
    delay(1);
    POKE(560, (unsigned)(&CLM_DATA_CAVEDL) & 0x00FF);
    POKE(561, (unsigned)(&CLM_DATA_CAVEDL) >> 8);

    /*Set DLI and enable it*/
    POKE(512, ((unsigned int) &dliHandler) % 256);
    POKE(513, ((unsigned int) &dliHandler) / 256);
    POKE(54286U, 192);
}

/* Enable text mode display*/
void enableDisplay() {
    POKE(559, 62);
}

void disableDisplay() {
    POKE(559, 0);
}
