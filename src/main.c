// Hailstorm CE 0.0.1
// Author: epsilon5
// License: none
// Description: a fun game


//suggestions: add a powerup that wipes the whole screen of asteroids
#include "sprites.h"

/* Keep these headers */
#include <tice.h>

/* Standard headers - it's recommended to leave them included */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
/* Other available headers */
// including stdarg.h, setjmp.h, assert.h, ctype.h, float.h, iso646.h, limits.h, errno.h
/* Available libraries */
// including lib/ce/graphc.h, lib/ce/fileioc.h, and lib/ce/keypadc.h
// Sample: #include <lib/ce/graphc.h>
#include <graphx.h>
#include <fileioc.h>
#include <keypadc.h>
#include <intce.h>
#include <tice.h>
#include <math.h>
/* Put your function prototypes here */
void WhiText(void) {
    gfx_SetTextBGColor(0);
    gfx_SetTextFGColor(255);
    gfx_SetTextTransparentColor(0);
}

void BlaText(void) {
    gfx_SetTextBGColor(255);
    gfx_SetTextFGColor(0);
    gfx_SetTextTransparentColor(255);
}

void Loading(short amount) {
    gfx_Blit(0);
    gfx_ZeroScreen();
    gfx_SetTransparentColor(255);
    gfx_ScaledTransparentSprite_NoClip(logo, 85, 85, 3, 2);
    gfx_SetColor(255);
    gfx_Rectangle(99, 139, 122, 22);
    gfx_Rectangle(100, 140, 120, 20);
    gfx_SetColor(20);
    gfx_HorizLine(50, 110, 220);
    gfx_FillRectangle(102, 142, amount * 29, 16);
    gfx_SwapDraw();
}


bool Confirm(uint8_t pause, bool menuwrapping, char title[]) {
    int y;
    bool keypress;
    char str1[9];
    gfx_Blit(0);
    y = 0;
    keypress = true;
    while (kb_AnyKey()) kb_Scan();
    while (!((kb_Data[6] & kb_Enter) || (kb_Data[1] & kb_2nd))) {
        kb_Scan();
        gfx_SetColor(0);
        gfx_FillRectangle(90, 90, 140, 60);
        WhiText();
        gfx_SetTextScale(2, 2);
        gfx_PrintStringXY(title, 160 - gfx_GetStringWidth(title) / 2, 95);
        gfx_SetTextScale(1, 1);
        if (pause > 0) {
            strcpy(str1, "Yes");
        } else {
            strcpy(str1, "Resume");
        }
        gfx_PrintStringXY(str1, 160 - gfx_GetStringWidth(str1) / 2, 117);
        if (pause > 0) {
            strcpy(str1, "No");
        } else {
            strcpy(str1, "Quit");
        }
        gfx_PrintStringXY(str1, 160 - gfx_GetStringWidth(str1) / 2, 133);
        if ((kb_Data[7] & kb_Down) && (keypress)) {
            ++y;
            keypress = false;
        }
        if ((kb_Data[7] & kb_Up) && (keypress)) {
            --y;
            keypress = false;
        }
        if (y > 1) {
            if (menuwrapping == true) {
                y = 0;
            } else {
                y = 1;
            }
        }
        if (y < 0) {
            if (menuwrapping == true) {
                y = 1;
            } else {
                y = 0;
            }
        }
        if (!kb_AnyKey()) keypress = true;
        gfx_SetColor(20);
        gfx_FillCircle(98, y * 16 + 120, 6);
        gfx_FillCircle(222, y * 16 + 120, 6);
        gfx_FillRectangle(98, y * 16 + 114, 124, 13);
        //outside of box
        gfx_SetTextBGColor(0);
        gfx_SetTextFGColor(255);
        gfx_SetTextTransparentColor(0);
        if (y == 0) {
            if (pause > 0) {
                strcpy(str1, "Yes");
            } else {
                strcpy(str1, "Resume");
            }
        } else {
            if (pause > 0) {
                strcpy(str1, "No");
            } else {
                strcpy(str1, "Quit");
            }
        }
        gfx_PrintStringXY(str1, 160 - gfx_GetStringWidth(str1) / 2, y * 16 + 117);
        gfx_SwapDraw();
    }
    while((kb_Data[6] & kb_Enter) || (kb_Data[1] & kb_2nd)) {
        kb_Scan();
        gfx_SetColor(255);
        gfx_FillCircle(98, y * 16 + 120, 6);
        gfx_FillCircle(222, y * 16 + 120, 6);
        gfx_FillRectangle(98, y * 16 + 114, 124, 13);
        BlaText();
        gfx_PrintStringXY(str1, 160 - gfx_GetStringWidth(str1) / 2, y * 16 + 117);
        gfx_SwapDraw();
    }
    if (y == 0) {
        return true;
    } else {
        return false;
    }
}


/* Put all your code here */
int main(void) {
    short keypressed;
    bool keypress;
    ti_var_t slota;
    //things for the appvar
    bool menuwrapping;
    bool showfps;
    bool hardmode;
    bool starfieldoff;
    int highscore;
    uint8_t currship;
    int asteroidsdestroyed;
    uint16_t asteroidsdestroyedtemp;
    uint16_t gamesplayed;
    int shotsfired;
    uint16_t shotsfiredtemp;
    uint8_t playershieldcolor;
    uint8_t playerbulletcolor;
    uint16_t plx;
    float plangle;
    uint8_t i;
    uint8_t j;
    short playerhealth;
    uint16_t numasteroids;
    uint16_t frametime;
    uint8_t loopsplayeroff;
    char str1[30];
    int count;
    uint8_t numbullets;
    uint16_t bulletcountdown;
    uint8_t bulletsfired;
    bool canfire;
    uint16_t shieldcountdown;
    typedef struct {
        uint16_t x;
        uint8_t y;
    } fstar_t;
    fstar_t fstar[20];
    uint8_t dist;
    typedef struct {
        uint16_t x;
        uint8_t y;
        bool movedlast;
    } bstar_t;
    bstar_t bstar[20];
    typedef struct {
        uint8_t size;
        uint8_t y;
        uint16_t x;
        uint8_t speed;
        uint8_t type;
    } asteroid_t;
    asteroid_t asteroid[20];
    gfx_sprite_t *spr[14][3];
    char menutext[4][20] = {"Select Ship", "Statistics", "Settings", "About"};
    char menutextb[7][20] = {"Menu Wrapping", "Show FPS", "Hard Mode", "Starfield Off", "Reset Statistics", "Shield Color", "Bullet Color"};
    char menutextc[35][40] = {" Introduction", "Thanks for downloading and playing", "HailStorm, which aims to be a fun space", "shooter with many features. With smooth", "and consistent gameplay, many available", "spaceships and quality graphics, it", "it is a fun game to play in math class", "or anywhere else.", " Gameplay", "The gameplay is very simple: just dodge", "the asteroids, shooting them or", "activating your shield when necessary.", "With that said, here are the controls:", "[<] and [>]: move left or right", "[2nd]: fire", "[alpha]: activate shield", "[y=]: pause", "[clear]: save and quit", " Credits", "Thanks to everyone who helped out with", "this project.", "Graphics: matkeller19", "Feedback: tr1p1ea, malagas, Jeffitus,", "beckadamtheinventor, tr1wrk", "Help: MateoConLechuga, Runer112", " Conclusion", "Thanks again for downloading. If you", "want to check out some of my other", "programs or submit feature requests", "or sprites for this one, please", "visit www.cemetech.net and search for", "HailStorm CE."};
    char menutextd[13][23] = {"open HailStorm", "score 1000", "destroy 100 asteroids", "score 5000", "destroy 200 asteroids", "fire 500 shots", "score 7500", "own my earlier games", "score 10000", "destroy 1000 asteroids", "play 100 games", "score 12500", "score 15000"};
    typedef struct {
        uint16_t x;
        uint8_t y;
        float xinc;
        float yinc;
    } bullet_t;
    bullet_t bullet[20];
    bool unlockedshipstemp[10];
    //pointers
    fstar_t *f;
    bstar_t *t;
    bullet_t *b;
    asteroid_t *s;
    //spaceships!
    typedef struct {
        gfx_sprite_t *spr;
        bool unlocked;
    } spaceship_t;
    spaceship_t spaceship[13];
    gfx_sprite_t *currspaceship;
    spaceship[0].spr = spaceshipa;
    spaceship[1].spr = spaceshipb;
    spaceship[2].spr = spaceshipc;
    spaceship[3].spr = spaceshipd;
    spaceship[4].spr = spaceshipe;
    spaceship[5].spr = spaceshipf;
    spaceship[6].spr = spaceshipg;
    spaceship[7].spr = spaceshiph;
    spaceship[8].spr = spaceshipi;
    spaceship[9].spr = spaceshipj;
    spaceship[10].spr = spaceshipk;
    spaceship[11].spr = spaceshipl;
    spaceship[12].spr = spaceshipm;
//store some asteroids in the array
    gfx_Begin();
    gfx_SetDraw(1);
    gfx_SetTransparentColor(255);
    Loading(1);
    delay(100);
    for (i = 2; i < 15; i++) {
        gfx_FillScreen(255);
        gfx_RotatedScaledTransparentSprite_NoClip(asteroida, 0, 0, 0, i * 8.5);
        spr[i - 2][0] = gfx_MallocSprite(i * 4, i * 4);
        gfx_GetSprite(spr[i - 2][0], 0, 0);
    }
    Loading(2);
    delay(100);
    for (i = 2; i < 15; i++) {
        gfx_FillScreen(255);
        gfx_RotatedScaledTransparentSprite_NoClip(asteroidb, 0, 0, 0, i * 8.5);
        spr[i - 2][1] = gfx_MallocSprite(i * 4, i * 4);
        gfx_GetSprite(spr[i - 2][1], 0, 0);
    }
    Loading(3);
    delay(100);
    for (i = 2; i < 15; i++) {
        gfx_FillScreen(255);
        gfx_RotatedScaledTransparentSprite_NoClip(asteroidc, 0, 0, 0, i * 8.5);
        spr[i - 2][2] = gfx_MallocSprite(i * 4, i * 4);
        gfx_GetSprite(spr[i - 2][2], 0, 0);
    }
    Loading(4);
    delay(100);
//highscore stuff
    ti_CloseAll();
    if (!(slota = ti_Open("HSTRMDAT", "r"))) {
        highscore = 0;
        currship = 0;
        gamesplayed = 0;
        asteroidsdestroyed = 0;
        shotsfired = 0;
        playershieldcolor = 20;
        playerbulletcolor = 231;
        menuwrapping = true;
        showfps = false;
        hardmode = false;
        starfieldoff = false;
    } else {
        ti_Read(&highscore, 3, 1, slota);
        ti_Read(&currship, 1, 1, slota);
        ti_Read(&gamesplayed, 2, 1, slota);
        ti_Read(&asteroidsdestroyed, 3, 1, slota);
        ti_Read(&shotsfired, 3, 1, slota);
        ti_Read(&playershieldcolor, 1, 1, slota);
        ti_Read(&playerbulletcolor, 1, 1, slota);
        ti_Read(&menuwrapping, 1, 1, slota);
        ti_Read(&showfps, 1, 1, slota);
        ti_Read(&hardmode, 1, 1, slota);
        ti_Read(&starfieldoff, 1, 1, slota);
    }
//ship unlocks
    spaceship[0].unlocked = true;
    spaceship[1].unlocked = highscore >= 1000;
    spaceship[2].unlocked = asteroidsdestroyed >= 100;
    spaceship[3].unlocked = highscore >= 5000;
    spaceship[4].unlocked = asteroidsdestroyed >= 200;
    spaceship[5].unlocked = shotsfired >= 500;
    spaceship[6].unlocked = highscore >= 7500;
    //check to see if the user has my other programs
    spaceship[7].unlocked = ((ti_Open("ACRCNDAT", "r")) && (ti_Open("WAVERDAT", "r")) && (ti_Open("DR0VEDAT", "r")));
    spaceship[8].unlocked = highscore >= 10000;
    spaceship[9].unlocked = asteroidsdestroyed >= 1000;
    spaceship[10].unlocked = gamesplayed >= 100;
    spaceship[11].unlocked = highscore >= 12500;
    spaceship[12].unlocked = highscore >= 15000;
//main menu
    WhiText();
    for (i = 0; i < 20; ++i) {
        f = &fstar[i];
        t = &bstar[i];
        f->x = randInt(0, 320);
        f->y = randInt(0, 240);
        t->x = randInt(0, 320);
        t->y = randInt(0, 240);
    }
    currspaceship = spaceship[currship].spr;
MAINMENU:
    //mainmenu start
    keypressed = 0;
    keypress = false;
    while (kb_AnyKey()) kb_Scan();
    while ((keypressed == 0) || (keypress == true)) {
        kb_Scan();
        //update star positions
        gfx_ZeroScreen();
        gfx_SetColor(255);
        for (i = 0; i < 20; ++i) {
            f = &fstar[i];
            t = &bstar[i];
            f->y++;
            if (t->movedlast) {
                t->movedlast = false;
            } else {
                t->movedlast = true;
                t->y++;
            }
            //graphics
            gfx_FillRectangle(f->x, f->y, 2, 2);
            gfx_SetPixel(t->x, t->y);
        }
        //draw everything
        //left
        if (kb_Data[1] & kb_Yequ) {
            gfx_SetColor(255);
            keypress = true;
            keypressed = 1;
        } else {
            gfx_SetColor(20);
        }
        gfx_Circle(50, 195, 30);
        gfx_Circle(50, 195, 31);
        gfx_FillCircle(50, 195, 6);
        gfx_FillCircle(70, 195, 6);
        gfx_FillCircle(30, 195, 6);
        gfx_SetTextFGColor(20);
        //logo, cool
        gfx_ScaledTransparentSprite_NoClip(logo, 85, 5, 3, 2);
        gfx_SetColor(20);
        gfx_HorizLine(50, 30, 220);
        gfx_SetTextScale(1, 1);
        //right
        if (kb_Data[1] & kb_Graph) {
            gfx_SetColor(255);
            keypress = true;
            keypressed = 3;
        } else {
            gfx_SetColor(20);
        }
        gfx_Circle(270, 195, 30);
        gfx_Circle(270, 195, 31);
        gfx_Line(249, 174, 289, 216);
        gfx_Line(250, 174, 290, 216);
        gfx_Line(251, 174, 291, 216);
        gfx_Line(289, 174, 249, 216);
        gfx_Line(290, 174, 250, 216);
        gfx_Line(291, 174, 251, 216);
        //center
        if (kb_Data[1] & kb_Zoom) {
            gfx_SetColor(255);
            keypress = true;
            keypressed = 2;
        } else {
            gfx_SetColor(20);
        }
        gfx_Circle(160, 190, 37);
        gfx_Circle(160, 190, 36);
        gfx_Circle(160, 190, 35);
        gfx_FillTriangle(186, 190, 160 + 26 * cos(2.09), 190 + 26 * sin(2.09), 160 + 26 * cos(4.19), 190 + 26 * sin(4.19));
        gfx_SetColor(0);
        gfx_FillTriangle(180, 190, 160 + 20 * cos(2.09), 190 + 20 * sin(2.09), 160 + 20 * cos(4.19), 190 + 20 * sin(4.19));
        //end
        if (!kb_AnyKey()) keypress = false;
        gfx_SwapDraw();
    }
    if (keypressed == 3) goto QUIT;
    if (keypressed == 1) {
        plx = 0;
        keypress = true;
        j = currship;
        playerhealth = 0;
        loopsplayeroff = 0;
        WhiText();
        while (!(kb_Data[1] & kb_Zoom)) {
            gfx_ZeroScreen();
            gfx_SetTextScale(1, 1);
            //NOTE: bottom toolbar code is embedded here
            kb_Scan();
            if (kb_Data[1] & kb_Yequ) {
                if ((keypress) && (plx > 0)) {
                    plx--;
                    keypress = false;
                }
                gfx_SetColor(255);
                gfx_SetTextFGColor(255);
            } else {
                gfx_SetColor(20);
                gfx_SetTextFGColor(20);
            }
            if (plx > 0) {
                gfx_FillTriangle(5, 225, 15 + 10 * cos(1.05), 225 + 10 * sin(1.05), 15 + 10 * cos(5.24), 225 + 10 * sin(5.24));
                gfx_SetColor(0);
                gfx_FillTriangle(9, 225, 15 + 6 * cos(1.05), 225 + 6 * sin(1.05), 15 + 6 * cos(5.24), 225 + 6 * sin(5.24));
                gfx_PrintStringXY(menutext[plx - 1], 23, 222);
            }
            if ((kb_Data[1] & kb_Graph)) {
                if ((keypress) && (plx < 3)) {
                    plx++;
                    keypress = false;
                }
                gfx_SetColor(255);
                gfx_SetTextFGColor(255);
            } else {
                gfx_SetColor(20);
                gfx_SetTextFGColor(20);
            }
            if (plx < 3) {
                gfx_FillTriangle(315, 225, 305 + 10 * cos(2.09), 225 + 10 * sin(2.09), 305 + 10 * cos(4.19), 225 + 10 * sin(4.19));
                gfx_SetColor(0);
                gfx_FillTriangle(311, 225, 305 + 6 * cos(2.09), 225 + 6 * sin(2.09), 305 + 6 * cos(4.19), 225 + 6 * sin(4.19));
                gfx_PrintStringXY(menutext[plx + 1], 297 - gfx_GetStringWidth(menutext[plx + 1]), 222);
            }
            //stars
            gfx_SetColor(255);
            for (i = 0; i < 20; ++i) {
                f = &fstar[i];
                t = &bstar[i];
                f->y++;
                if (t->movedlast) {
                    t->movedlast = false;
                } else {
                    t->movedlast = true;
                    t->y++;
                }
                //graphics
                gfx_FillRectangle(f->x, f->y, 2, 2);
                gfx_SetPixel(t->x, t->y);
            }
            i = 1;
            gfx_SetColor(20);
            gfx_FillCircle(12 + i * 106.66, 240, 12);
            gfx_FillCircle(95 + i * 106.66, 240, 12);
            gfx_FillRectangle(12 + i * 106.66, 227, 82, 2);
            gfx_SetColor(0);
            gfx_FillCircle(12 + i * 106.66, 240, 10);
            gfx_FillCircle(95 + i * 106.66, 240, 10);
            gfx_FillRectangle(12 + i * 106.66, 229, 81, 11);
            gfx_SetTextFGColor(20);
            strcpy(str1, "Main Menu");
            gfx_PrintStringXY(str1, 160 - gfx_GetStringWidth(str1) / 2, 231);
            if (kb_Data[1] & kb_Zoom) goto MAINMENU;
            gfx_SetColor(20);
            //do some making of the button that returns to the main menu
            gfx_SetTextFGColor(20);
            if (!kb_AnyKey()) keypress = true;
            //draw everything
            //title text and stuff
            gfx_SetTextScale(2, 2);
            gfx_PrintStringXY(menutext[plx], 160 - gfx_GetStringWidth(menutext[plx]) / 2, 10);
            gfx_HorizLine(15, 30, 290);
            gfx_HorizLine(30, 215, 260);

            //menu specific stuff
            if (plx == 0) {
                if (kb_Data[7] & kb_Left) {
                    if ((keypress) && (j > 0)) {
                        j--;
                        keypress = false;
                    }
                    gfx_SetColor(255);
                } else {
                    gfx_SetColor(20);
                }
                if (j > 0) {
                    gfx_FillTriangle(64, 120, 90 + 26 * cos(1.05), 120 + 26 * sin(1.05), 90 + 26 * cos(5.24), 120 + 26 * sin(5.24));
                    gfx_SetColor(0);
                    gfx_FillTriangle(70, 120, 90 + 20 * cos(1.05), 120 + 20 * sin(1.05), 90 + 20 * cos(5.24), 120 + 20 * sin(5.24));
                }
                if (kb_Data[7] & kb_Right) {
                    if ((keypress) && (j < 12)) {
                        j++;
                        keypress = false;
                    }
                    gfx_SetColor(255);
                } else {
                    gfx_SetColor(20);
                }
                if (j < 12) {
                    gfx_FillTriangle(256, 120, 230 + 26 * cos(2.09), 120 + 26 * sin(2.09), 230 + 26 * cos(4.19), 120 + 26 * sin(4.19));
                    gfx_SetColor(0);
                    gfx_FillTriangle(250, 120, 230 + 20 * cos(2.09), 120 + 20 * sin(2.09), 230 + 20 * cos(4.19), 120 + 20 * sin(4.19));
                }
                gfx_ScaledTransparentSprite_NoClip(spaceship[j].spr, 130, 90, 2, 2);
                sprintf(str1, "%d / 13", j + 1);
                gfx_SetTextScale(1, 1);
                gfx_SetTextFGColor(255);
                gfx_PrintStringXY(str1, 160 - gfx_GetStringWidth(str1) / 2, 40);
                strcpy(str1, "Unlocked : ");
                if (spaceship[j].unlocked) {
                    strcat(str1, "Yes");
                } else {
                    strcat(str1, "No");
                }
                gfx_PrintStringXY(str1, 160 - gfx_GetStringWidth(str1) / 2, 55);
                strcpy(str1, "To unlock : ");
                strcat(str1, menutextd[j]);
                gfx_PrintStringXY(str1, 160 - gfx_GetStringWidth(str1) / 2, 70);
                if (j == currship) {
                    WhiText();
                    strcpy(str1, "SELECTED");
                    gfx_SetColor(7);
                } else {
                    gfx_SetColor(255);
                    strcpy(str1, "2nd/enter");
                }
                if (spaceship[j].unlocked) {
                    gfx_FillCircle(120, 164, 5);
                    gfx_FillCircle(200, 164, 5);
                    gfx_FillRectangle(120, 159, 80, 11);
                    BlaText();
                    gfx_PrintStringXY(str1, 160 - gfx_GetStringWidth(str1) / 2, 161);
                }
                WhiText();
                //if some condition is fulfilled
                if ((kb_Data[6] & kb_Enter) || (kb_Data[1] & kb_2nd)) {
                    if (spaceship[j].unlocked) currship = j;
                }
            }
            if (plx == 1) {
                gfx_SetTextScale(1, 1);
                gfx_SetTextFGColor(255);
                gfx_PrintStringXY("Highscore :", 40, 40);
                sprintf(str1, "%d", highscore);
                gfx_PrintStringXY(str1, 280 - gfx_GetStringWidth(str1), 40);
                gfx_PrintStringXY("Games played :", 40, 55);
                sprintf(str1, "%d", gamesplayed);
                gfx_PrintStringXY(str1, 280 - gfx_GetStringWidth(str1), 55);
                gfx_PrintStringXY("Asteroids destroyed :", 40, 70);
                sprintf(str1, "%d", asteroidsdestroyed);
                gfx_PrintStringXY(str1, 280 - gfx_GetStringWidth(str1), 70);
                gfx_PrintStringXY("Shots fired :", 40, 85);
                sprintf(str1, "%d", shotsfired);
                gfx_PrintStringXY(str1, 280 - gfx_GetStringWidth(str1), 85);
                sprintf(str1, "%d%s", asteroidsdestroyed * 100 / (shotsfired + (shotsfired == 0)), "%");
                gfx_PrintStringXY("Accuracy :", 40, 100);

                gfx_PrintStringXY(str1, 280 - gfx_GetStringWidth(str1), 100);
            }
            if (plx == 2) {
                if (kb_Data[7] & kb_Up) {
                    if (keypress) {
                        playerhealth--;
                        keypress = false;
                    }
                }
                if (kb_Data[7] & kb_Down) {
                    if (keypress) {
                        playerhealth++;
                        keypress = false;
                    }
                }
                if (playerhealth > 6) {
                    if (menuwrapping) {
                        playerhealth = 0;
                    } else {
                        playerhealth = 6;
                    }
                }
                if (playerhealth < 0) {
                    if (menuwrapping) {
                        playerhealth = 6;
                    } else {
                        playerhealth = 0;
                    }
                }
                if (kb_Data[7] & kb_Right) {
                    if (playerhealth == 0) menuwrapping = true;
                    if (playerhealth == 1) showfps = true;
                    if (playerhealth == 2) hardmode = true;
                    if (playerhealth == 3) starfieldoff = true;
                    if ((playerhealth == 5) && (playershieldcolor < 255)) playershieldcolor++;
                    if ((playerhealth == 6) && (playerbulletcolor < 255)) playerbulletcolor++;
                }
                if (kb_Data[7] & kb_Left) {
                    if (playerhealth == 0) menuwrapping = false;
                    if (playerhealth == 1) showfps = false;
                    if (playerhealth == 2) hardmode = false;
                    if (playerhealth == 3) starfieldoff = false;
                    if ((playerhealth == 5) && (playershieldcolor > 1)) playershieldcolor--;
                    if ((playerhealth == 6) && (playerbulletcolor > 1)) playerbulletcolor--;
                }
                gfx_SetTextFGColor(255);
                gfx_SetTextScale(1, 1);
                for (i = 0; i < 7; i++) {
                    gfx_PrintStringXY(menutextb[i], 40, 40 + i * 15);
                }
                //the box
                if ((kb_Data[6] & kb_Enter) || (kb_Data[1] & kb_2nd)) {
                    if ((playerhealth == 4) && (keypress)) {
                        if (Confirm(1, menuwrapping, "Really?")) {
                            asteroidsdestroyed = 0;
                            highscore = 0;
                            currship = 0;
                            gamesplayed = 0;
                            asteroidsdestroyed = 0;
                            shotsfired = 0;
                            for (i = 1; i < 13; i++) spaceship[i].unlocked = false;
                        }
                    }
                    if ((playerhealth == 0) && (keypress)) {
                        menuwrapping = !menuwrapping;
                        keypress = false;
                    }
                    if ((playerhealth == 1) && (keypress)) {
                        showfps = !showfps;
                        keypress = false;
                    }
                    if ((playerhealth == 2) && (keypress)) {
                        hardmode = !hardmode;
                        keypress = false;
                    }
                    if ((playerhealth == 3) && (keypress)) {
                        starfieldoff = !starfieldoff;
                        keypress = false;
                    }
                    gfx_SetColor(20);
                    WhiText();
                } else {
                    gfx_SetColor(255);
                    BlaText();
                }
                gfx_FillCircle(40, 44 + playerhealth * 15, 6);
                gfx_FillRectangle(40, 38 + playerhealth * 15, 110, 13);
                gfx_FillCircle(150, 44 + playerhealth * 15, 6);
                gfx_PrintStringXY(menutextb[playerhealth], 40, 40 + playerhealth * 15);
                //colors
                gfx_SetColor(playershieldcolor);
                gfx_FillCircle(240, 119, 6);
                gfx_FillRectangle(240, 113, 40, 13);
                gfx_FillCircle(280, 119, 6);
                gfx_SetColor(playerbulletcolor);
                gfx_FillCircle(240, 134, 6);
                gfx_FillRectangle(240, 128, 40, 13);
                gfx_FillCircle(280, 134, 6);
                gfx_SetColor(255);
                for (i = 0; i < 4; i++) {
                    if (i == 0) {
                        canfire = menuwrapping;
                    } else if (i == 1) {
                        canfire = showfps;
                    } else if (i == 2) {
                        canfire = hardmode;
                    } else if (i == 3) {
                        canfire = starfieldoff;
                    }
                    gfx_SetColor(224);
                    if (canfire) gfx_SetColor(7);
                    gfx_FillCircle(240, 44 + i * 15, 6);
                    gfx_FillRectangle(240, 38 + i * 15, 40, 13);
                    gfx_FillCircle(280, 44 + i * 15, 6);
                    gfx_SetColor(255);
                    if (canfire) {
                        gfx_FillCircle(280, 44 + i * 15, 4);
                    } else {
                        gfx_FillCircle(240, 44 + i * 15, 4);
                    }
                }
                for (i = 2; i < 4; i++) {
                    if (i == 2) {
                        count = playershieldcolor;
                    } else {
                        count = playerbulletcolor;
                    }
                    if ((count > 1) && (count < 255)) sprintf(str1, "< %d >", count);
                    if (count == 1) sprintf(str1, "%d >", count);
                    if (count == 255) sprintf(str1, "< %d", count);
                    gfx_FillCircle(243, 89 + i * 15, 4);
                    gfx_FillRectangle(243, 85 + i * 15, 34, 9);
                    gfx_FillCircle(277, 89 + i * 15, 4);
                    gfx_PrintStringXY(str1, 260 - gfx_GetStringWidth(str1) / 2, 86 + i * 15);
                }
                WhiText();
            }
            if (plx == 3) {
                gfx_SetColor(20);
                if (kb_Data[7] & kb_Up) {
                    if ((keypress) && (loopsplayeroff > 0)) {
                        loopsplayeroff--;
                        keypress = false;
                    }
                    gfx_SetColor(255);
                }
                if (kb_Data[7] & kb_Down) {
                    if ((keypress) && (loopsplayeroff < 22)) {
                        loopsplayeroff++;
                        keypress = false;
                    }
                    gfx_SetColor(255);
                }
                //the scrolling box thing
                gfx_FillRectangle(17, 37 + loopsplayeroff * 7, 5, 17);
                gfx_FillCircle(19, 37 + loopsplayeroff * 7, 2);
                gfx_FillCircle(19, 54 + loopsplayeroff * 7, 2);
                WhiText();
                gfx_SetTextScale(1, 1);
                //bulletcountdown is used for text y position because why not
                bulletcountdown = 0;
                for (i = 0; i < 10; ++i) {
                    if (" "[0] == menutextc[i + loopsplayeroff][0]) {
                        gfx_SetTextScale(2, 2);
                        bulletcountdown++;
                        gfx_PrintStringXY(menutextc[i + loopsplayeroff], 160 - gfx_GetStringWidth(menutextc[i + loopsplayeroff]) / 2, 44 + (bulletcountdown - 1) * 15);
                        bulletcountdown++;
                    } else {
                        gfx_SetTextScale(1, 1);
                        gfx_PrintStringXY(menutextc[i + loopsplayeroff], 40, 40 + bulletcountdown * 15);
                        bulletcountdown++;
                    }
                    if (bulletcountdown > 10) break;
                }
            }
            gfx_SwapDraw();
        }
        goto MAINMENU;
    }
START:
    numasteroids = 0;
    for (i = 0; i < 20; ++i) {
        asteroid_t *s = &asteroid[i];
        s->x = randInt(320, 640);
        s->y = randInt(0, 240);
        s->speed = randInt(2, 7);
        s->size = randInt(2, 15);
        s->type = randInt(0, 2);
    }
    count = 0;
    playerhealth = 100;
    numbullets = 0;
    plx = 160;
    plangle = 1.75;
    bulletsfired = 0;
    bulletcountdown = 0;
    shieldcountdown = 0;
    canfire = true;
    keypress = true;
    gamesplayed++;
    shotsfiredtemp = 0;
    asteroidsdestroyedtemp = 0;
    ti_CloseAll();
    if (slota = ti_Open("HSTRMTMP", "r")) {
        if (Confirm(3, menuwrapping, "Continue?")) {
            ti_Read(&asteroidsdestroyedtemp, 2, 1, slota);
            ti_Read(&shotsfiredtemp, 2, 1, slota);
            ti_Read(&plx, 2, 1, slota);
            ti_Read(&plangle, 4, 1, slota);
            ti_Read(&playerhealth, 1, 1, slota);
            ti_Read(&numasteroids, 2, 1, slota);
            ti_Read(&loopsplayeroff, 1, 1, slota);
            ti_Read(&count, 3, 1, slota);
            ti_Read(&numbullets, 1, 1, slota);
            ti_Read(&bulletcountdown, 2, 1, slota);
            ti_Read(&bulletsfired, 1, 1, slota);
            ti_Read(&shieldcountdown, 2, 1, slota);
            ti_Read(&asteroid, 120, 1, slota);
            ti_Read(&bullet, 220, 1, slota);
            ti_Read(&hardmode, 1, 1, slota);
        }
        ti_Delete("HSTRMTMP");
    }
    //just in case
    currspaceship = spaceship[currship].spr;
    WhiText();
    while (playerhealth > 0) {
        timer_Control = TIMER1_ENABLE | TIMER1_32K | TIMER1_UP;
        numasteroids = count / 150;
        if (numasteroids > 20) numasteroids = 20;
        ++count;
        kb_Scan();
        if ((kb_Data[7] & kb_Left) || (kb_Data[7] & kb_Right)) {
            if (kb_Data[7] & kb_Left) {
                if (plangle < 2.55) plangle += 0.1;
            }
            if (kb_Data[7] & kb_Right) {
                if (plangle > 0.55) plangle -= 0.1;
            }
        } else {
            if (plangle < 1.55) {
                plangle += 0.1;
            }
            if (plangle > 1.65) {
                plangle -= 0.1;
            }
        }
        if (kb_AnyKey()) {
            if ((kb_Data[2] & kb_Alpha) && (shieldcountdown == 0)) shieldcountdown = 1000;
            //bullet stuff
            if (kb_Data[1] & kb_2nd) {
                if ((canfire) && (keypress)) {
                    b = &bullet[numbullets++];
                    b->x = plx + 15 * cos(plangle + 0.8);
                    b->y = 180 + 15 * sin(plangle + 0.8);
                    b->xinc = 10 * cos(plangle);
                    b->yinc = 10 * sin(plangle);
                    b = &bullet[numbullets++];
                    b->x = plx + 15 * cos(plangle - 0.8);
                    b->y = 180 + 15 * sin(plangle - 0.8);
                    b->xinc = 10 * cos(plangle);
                    b->yinc = 10 * sin(plangle);
                    if (bulletsfired < 5) bulletsfired++;
                    if ((bulletcountdown == 0) && (bulletsfired == 5)) bulletcountdown = 200;
                    keypress = false;
                    shotsfired += 2;
                    shotsfiredtemp += 2;
                }
            } else {
                keypress = true;
            }
            if (kb_Data[6] & kb_Clear) {
                //do some game saving
                if (Confirm(1, false, "Save & quit?")) {
                    ti_CloseAll();
                    slota = ti_Open("HSTRMTMP", "w+");
                    Loading(0);
                    gfx_Blit(0);
                    WhiText();
                    strcpy(str1, "Saving your game...");
                    gfx_PrintStringXY(str1, 160 - gfx_GetStringWidth(str1) / 2, 180);
                    gfx_SwapDraw();
                    delay(200);
                    ti_Write(&asteroidsdestroyedtemp, 2, 1, slota);
                    ti_Write(&shotsfiredtemp, 2, 1, slota);
                    ti_Write(&plx, 2, 1, slota);
                    ti_Write(&plangle, 4, 1, slota);
                    ti_Write(&playerhealth, 1, 1, slota);
                    ti_Write(&numasteroids, 2, 1, slota);
                    ti_Write(&loopsplayeroff, 1, 1, slota);
                    ti_Write(&count, 3, 1, slota);
                    ti_Write(&numbullets, 1, 1, slota);
                    ti_Write(&bulletcountdown, 2, 1, slota);
                    ti_Write(&bulletsfired, 1, 1, slota);
                    ti_Write(&shieldcountdown, 2, 1, slota);
                    ti_Write(&asteroid, 120, 1, slota);
                    ti_Write(&bullet, 220, 1, slota);
                    ti_Write(&hardmode, 1, 1, slota);
                    ti_SetArchiveStatus(true, slota);
                    Loading(4);
                    delay(100);
                    goto QUIT;
                    while (kb_AnyKey()) kb_Scan();
                }
            }
        }
        //plx update
        plx = plx + 6 * cos(plangle);
        if (bulletcountdown == 1) bulletsfired = 0;
        if (bulletcountdown > 0) bulletcountdown--;
        if (bulletsfired > 4) {
            canfire = false;
        } else {
            canfire = true;
        }
        if (shieldcountdown > 0) shieldcountdown--;
//update asteroids
        for (i = 0; i < numasteroids; ++i) {
            s = &asteroid[i];
            if (s->type > 6) {
                if (s->speed > 0) s->speed--;
                ++s->type;
            }
            s->y += s->speed;
            if (s->y > 240) {
                s->x = randInt(0, 320);
                s->size = randInt(2, 15);
                s->speed = randInt(2, 7);
                s->type = randInt(0, 2);
                continue;
            }
            if (s->type > 9) {
                s->x = randInt(0, 320);
                s->size = randInt(2, 15);
                s->y = 0;
                s->speed = randInt(2, 7);
                s->type = randInt(0, 2);
                continue;
            }
            if ((abs(180 - s->y) <= s->size) && (abs(plx - s->x) <= s->size + 10)) {
                if (s->type < 3) {
                    s->type = 7;
                    if (!(shieldcountdown > 800)) {
                        playerhealth -= s->size * s->speed / 6;
                    }
                    continue;
                } else {
                    if (s->type == 3) {
                        if (playerhealth < 75) {
                            playerhealth += 25;
                        } else {
                            playerhealth = 100;
                        }
                        s->x = randInt(0, 320);
                        s->y = 0;
                        s->size = randInt(2, 15);
                        s->speed = randInt(2, 7);
                        s->type = randInt(0, 2);
                        continue;
                    }
                    if (s->type == 4) {
                        //continue existing shield
                        if (shieldcountdown > 800) {
                            shieldcountdown = 1000;
                        } else {
                            shieldcountdown = 0;
                        }
                        s->x = randInt(0, 320);
                        s->y = 0;
                        s->size = randInt(2, 15);
                        s->speed = randInt(2, 7);
                        s->type = randInt(0, 2);
                        continue;
                    }
                    if (s->type == 5) {
                        bulletcountdown = 1;
                        s->x = randInt(0, 320);
                        s->y = 0;
                        s->size = randInt(2, 15);
                        s->speed = randInt(2, 7);
                        s->type = randInt(0, 2);
                        continue;
                    }
                    if (s->type == 6) {
                        s->x = randInt(0, 320);
                        s->y = 0;
                        s->size = randInt(2, 15);
                        s->speed = randInt(2, 7);
                        s->type = randInt(0, 2);
                        //j = distance from ship position to 0,0
                        //keypressed = distance from ship position to 320, 0
                        dist = sqrt((plx * plx) + (165 * 165));
                        keypressed = sqrt(((320 - plx) * (320 - plx)) + (165 * 165));
                        if (dist > keypressed) keypressed = dist;
                        for (j = 0; j < keypressed / 4; j++) {
                            gfx_SetColor(playerbulletcolor);
                            gfx_Circle(plx, 180, j * 4);
                            gfx_SetColor(0);
                            gfx_Circle(plx, 180, (j - 1) * 4);
                            gfx_SwapDraw();
                            //cool graphics
                        }
                        for (dist = 0; dist < numasteroids; ++dist) {
                            s = &asteroid[dist];
                            if (s->type < 3) s->type = 7;
                        }
                        break;
                    }
                }
            }
        }
        //update bullets
        for (i = 0; i < numbullets; ++i) {
            b = &bullet[i];
            b->x += b->xinc;
            b->y -= b->yinc;
            if ((b->x > 320) || (b->x < 0) || (b->y > 240)) {
                bullet[i] = bullet[--numbullets];
                continue;
            }
            for (j = 0; j < numasteroids; j++) {
                s = &asteroid[j];
                if ((abs(b->x - s->x) <= s->size + 2)) {
                    if ((abs(b->y - s->y) <= s->size + 2) && (s->y > 0)) {
                        if (s->type < 3) {
                            s->type = 7;
                            if ((randInt(0, 5) == 5) && (!hardmode)) {
                                s->type = randInt(3, 6);
                                s->size = 7;
                                s->speed = 4;
                            }
                            bullet[i] = bullet[--numbullets];
                            asteroidsdestroyed++;
                            asteroidsdestroyedtemp++;
                            break;
                        }
                    }
                }
            }
        }
//graphics start here
        gfx_ZeroScreen();
        //highscore line thing
        if (abs(highscore - count) < 54) {
            gfx_SetColor(randInt(0, 255));
            gfx_FillRectangle(0, 165 - (highscore - count) * 3, 120, 2);
            gfx_FillRectangle(200, 165 - (highscore - count) * 3, 120, 2);
            gfx_SetTextFGColor(randInt(0, 255));
            strcpy(str1, "Highscore!");
            if (161 - (highscore - count) < 180) gfx_PrintStringXY(str1, 160 - gfx_GetStringWidth(str1) / 2, 161 - (highscore - count) * 3);
        }
        if (!starfieldoff) {
            gfx_SetColor(255);
            for (i = 0; i < 20; ++i) {
                f = &fstar[i];
                t = &bstar[i];
                f->y++;
                if (t->movedlast) {
                    t->movedlast = false;
                } else {
                    t->movedlast = true;
                    t->y++;
                }
                //graphics
                gfx_FillRectangle(f->x, f->y, 2, 2);
                gfx_SetPixel(t->x, t->y);
            }
        }
        if (plx < 15 || plx > 305) {
            loopsplayeroff++;
            if (loopsplayeroff > 60) playerhealth--;
            gfx_SetTextFGColor(224);
            gfx_SetTextScale(2, 2);
            if (plx < 15) gfx_PrintStringXY("!", 8, 172);
            if (plx > 305) gfx_PrintStringXY("!", 312, 172);
            gfx_SetTextFGColor(255);
            gfx_SetTextScale(1, 1);
        } else {
            loopsplayeroff = 0;
            gfx_RotatedTransparentSprite_NoClip(currspaceship, plx - 15, 165, -plangle * 41 + 64);
            if (shieldcountdown > 800) {
                gfx_SetColor(playershieldcolor);
                if (shieldcountdown < 850) {
                    if (randInt(0, 5) > 3) gfx_Circle(plx, 180, 18);
                } else {
                    gfx_Circle(plx, 180, 18);
                }
            }
        }
        for (i = 0; i < numasteroids; ++i) {
            s = &asteroid[i];
            if (s->y < 235) {
                if (s->type < 3) {
                    gfx_TransparentSprite(spr[s->type][s->size - 2], s->x - s->size, s->y - s->size);
                } else {
                    if (s->type == 3) gfx_TransparentSprite(powerupa, s->x - s->size, s->y - s->size);
                    if (s->type == 4) gfx_TransparentSprite(powerupb, s->x - s->size, s->y - s->size);
                    if (s->type == 5) gfx_TransparentSprite(powerupc, s->x - s->size, s->y - s->size);
                    if (s->type == 6) gfx_TransparentSprite(powerupd, s->x - s->size, s->y - s->size);
                    if (s->type > 6) {
                        gfx_SetColor(74);
                        for (j = 0; j < s->type - 1; j++) {
                            gfx_FillRectangle(randInt(s->x - s->size, s->x + s->size), randInt(s->y - s->size, s->y + s->size), 15 - s->type, 15 - s->type);
                        }
                    }
                }
            }
        }
        gfx_SetColor(0);
        gfx_FillRectangle(0, 228, 320, 12);
        if (playerhealth > 70) gfx_SetColor(7);
        if (playerhealth < 80) gfx_SetColor(231);
        if (playerhealth < 40) gfx_SetColor(224);
        gfx_FillRectangle(160 - playerhealth, 232, playerhealth * 2, 6);
        if ((bulletcountdown > 0) && (bulletsfired > 4)) {
            gfx_SetColor(224);
            gfx_FillRectangle(2, 232, bulletcountdown / 3.6, 6);
        } else if ((bulletcountdown == 0) && (bulletsfired == 0)) {
            gfx_SetColor(7);
            gfx_FillRectangle(2, 232, 52, 6);
        } else if ((bulletcountdown == 0) && (bulletsfired < 5)) {
            gfx_SetColor(20);
            gfx_FillRectangle(2, 232, (5 - bulletsfired) * 10, 6);
        }
        if (shieldcountdown >= 800) {
            gfx_SetColor(20);
            gfx_FillRectangle(318 - (shieldcountdown - 800) / 3.8, 232, (shieldcountdown - 800) / 3.8, 6);
        }
        if ((shieldcountdown < 800) && (shieldcountdown > 0)) {
            gfx_SetColor(224);
            gfx_FillRectangle(318 - shieldcountdown / 15.2, 232, shieldcountdown / 15.2, 6);
        }
        if (shieldcountdown == 0) {
            gfx_SetColor(7);
            gfx_FillRectangle(266, 232, 52, 6);
        }
        //bottom toolbar
        gfx_SetTextFGColor(20);
        gfx_PrintStringXY("HEALTH", 136, 218);
        gfx_PrintStringXY("SHIELD", 267, 218);
        gfx_PrintStringXY("BULLET", 5, 218);
        gfx_SetColor(20);
        gfx_FillRectangle(0, 228, 320, 2);
        gfx_FillRectangle(56, 228, 2, 12);
        gfx_FillRectangle(262, 228, 2, 12);
        //for the top
        gfx_FillRectangle(0, 12, 320, 2);
        gfx_SetColor(0);
        gfx_FillRectangle(0, 0, 320, 12);
        gfx_SetColor(playerbulletcolor);
        for (i = 0; i < numbullets; ++i) {
            bullet_t *b = &bullet[i];
            gfx_Line(b->x, b->y, b->x + b->xinc / 2, b->y - b->yinc / 2);
        }
        sprintf(str1, "SCORE : %d", count);
        gfx_PrintStringXY(str1, 5, 2);
        sprintf(str1, "ASTEROIDS : %d", asteroidsdestroyedtemp);
        gfx_PrintStringXY(str1, 315 - gfx_GetStringWidth(str1), 2);
        if (showfps) {
            sprintf(str1, "FPS : %d", 32768 / timer_1_Counter);
            gfx_PrintStringXY(str1, 160 - gfx_GetStringWidth(str1) / 2, 2);
        }
        if (kb_Data[1] & kb_Yequ) {
            gfx_ZeroScreen();
            gfx_Blit(1);
            if (!(Confirm(0, menuwrapping, "Paused."))) playerhealth = 0;
        }
        gfx_SwapDraw();
        //update frametime and stuff
        frametime = timer_1_Counter;
        timer_1_Counter = 0;
    }
//update the unlocked ships
    for (i = 0; i < 13; i++) {
        unlockedshipstemp[i] = false;
    }
//shieldcountdown will be used to refer to the number of ships that were unlocked this game- edit nevermind that's not a good idea
    if ((count >= 1000) && (!(spaceship[1].unlocked))) {
        spaceship[1].unlocked = true;
        unlockedshipstemp[1] = true;
    }
//less
    if ((asteroidsdestroyed >= 100)  && (!(spaceship[2].unlocked))) {
        spaceship[2].unlocked = true;
        unlockedshipstemp[2] = true;
    }
    if ((count >= 5000) && (!(spaceship[3].unlocked))) {
        spaceship[3].unlocked = true;
        unlockedshipstemp[3] = true;
    }
    if ((asteroidsdestroyed >= 200) && (!(spaceship[4].unlocked))) {
        spaceship[4].unlocked = true;
        unlockedshipstemp[4] = true;
    }
    if ((shotsfired >= 500) && (!(spaceship[5].unlocked))) {
        spaceship[5].unlocked = true;
        unlockedshipstemp[5] = true;
    }
    if ((count >= 7500) && (!(spaceship[6].unlocked))) {
        spaceship[6].unlocked = true;
        unlockedshipstemp[6] = true;
    }
    if ((count >= 10000) && (!(spaceship[8].unlocked))) {
        spaceship[8].unlocked = true;
        unlockedshipstemp[8] = true;
    }
    if ((asteroidsdestroyed >= 1000) && (!(spaceship[9].unlocked))) {
        spaceship[9].unlocked = true;
        unlockedshipstemp[9] = true;
    }
    if ((gamesplayed >= 100) && (!(spaceship[10].unlocked))) {
        spaceship[10].unlocked = true;
        unlockedshipstemp[10] = true;
    }
    if ((count >= 12500) && (!(spaceship[11].unlocked))) {
        spaceship[11].unlocked = true;
        unlockedshipstemp[11] = true;
    }
    if ((count >= 15000) && (!(spaceship[12].unlocked))) {
        spaceship[12].unlocked = true;
        unlockedshipstemp[12] = true;
    }
//add more as needed
    while (kb_AnyKey()) kb_Scan();
    while (!(kb_Data[1] & kb_Zoom)) {
        kb_Scan();
        if (kb_Data[1] & kb_Yequ) {
            while (kb_Data[1] & kb_Yequ) kb_Scan();
            goto START;
        }
        if (kb_Data[1] & kb_Graph) if (Confirm(1, menuwrapping, "Quit?")) goto QUIT;
        //draw everything
        gfx_ZeroScreen();
        gfx_SetColor(255);
        for (i = 0; i < 20; ++i) {
            f = &fstar[i];
            t = &bstar[i];
            f->y++;
            if (t->movedlast) {
                t->movedlast = false;
            } else {
                t->movedlast = true;
                t->y++;
            }
            //graphics
            gfx_FillRectangle(f->x, f->y, 2, 2);
            gfx_SetPixel(t->x, t->y);
        }
        gfx_SetTextScale(2, 2);
        gfx_SetTextFGColor(20);
        gfx_PrintStringXY("Finished.", 160 - gfx_GetStringWidth("Finished") / 2, 10);
        gfx_SetColor(20);
        gfx_HorizLine(15, 30, 290);
        //bottom toolbar here
        for (i = 0; i < 3; i++) {
            gfx_SetColor(20);
            gfx_FillCircle(12 + i * 106.66, 240, 12);
            gfx_FillCircle(95 + i * 106.66, 240, 12);
            gfx_FillRectangle(12 + i * 106.66, 227, 82, 2);
            gfx_SetColor(0);
            gfx_FillCircle(12 + i * 106.66, 240, 10);
            gfx_FillCircle(95 + i * 106.66, 240, 10);
            gfx_FillRectangle(12 + i * 106.66, 229, 81, 11);
        }
        if (count >= highscore) {
            gfx_SetTextFGColor(randInt(0, 255));
            strcpy(str1, "New highscore!");
            gfx_PrintStringXY(str1, 160 - gfx_GetStringWidth(str1) / 2, 180);
            highscore = count;
        }
        gfx_SetTextScale(1, 1);
        gfx_SetTextFGColor(20);
        gfx_PrintStringXY("Main Menu", 160 - gfx_GetStringWidth("Main Menu") / 2, 231);
        gfx_PrintStringXY("Restart", 53.333 - gfx_GetStringWidth("Restart") / 2, 231);
        gfx_PrintStringXY("Quit", 266.66 - gfx_GetStringWidth("Quit") / 2, 231);
        WhiText();
        gfx_PrintStringXY("Score :", 40, 40);
        gfx_PrintStringXY("Highscore :", 40, 55);
        gfx_PrintStringXY("Shots fired :", 40, 70);
        gfx_PrintStringXY("Asteroids destroyed :", 40, 85);
        gfx_PrintStringXY("Accuracy :", 40, 100);
        sprintf(str1, "%d", count);
        gfx_PrintStringXY(str1, 280 - gfx_GetStringWidth(str1), 40);
        sprintf(str1, "%d", highscore);
        gfx_PrintStringXY(str1, 280 - gfx_GetStringWidth(str1), 55);
        sprintf(str1, "%d", shotsfiredtemp);
        gfx_PrintStringXY(str1, 280 - gfx_GetStringWidth(str1), 70);
        sprintf(str1, "%d", asteroidsdestroyedtemp);
        gfx_PrintStringXY(str1, 280 - gfx_GetStringWidth(str1), 85);
        sprintf(str1, "%d%s", asteroidsdestroyedtemp * 100 / (shotsfiredtemp + (shotsfiredtemp == 0)), "%");
        gfx_PrintStringXY(str1, 280 - gfx_GetStringWidth(str1), 100);
        //ships unlocked
        j = 0;
        for (i = 1; i < 13; i++) {
            if (unlockedshipstemp[i]) {
                gfx_RotatedScaledTransparentSprite_NoClip(spaceship[i].spr, 265 - j * 20, 112, 0, 32);
                j++;
            }
        }
        if (j > 0) {
            sprintf(str1, "Unlocked (%d) :", j);
            gfx_PrintStringXY(str1, 40, 115);
        }
        gfx_SwapDraw();
    }
    goto MAINMENU;
//save highscore to appvar
QUIT:
    gfx_End();
    ti_CloseAll();
    slota = ti_Open("HSTRMDAT", "w+");
    ti_Write(&highscore, 3, 1, slota);
    ti_Write(&currship, 1, 1, slota);
    ti_Write(&gamesplayed, 2, 1, slota);
    ti_Write(&asteroidsdestroyed, 3, 1, slota);
    ti_Write(&shotsfired, 3, 1, slota);
    ti_Write(&playershieldcolor, 1, 1, slota);
    ti_Write(&playerbulletcolor, 1, 1, slota);
    ti_Write(&menuwrapping, 1, 1, slota);
    ti_Write(&showfps, 1, 1, slota);
    ti_Write(&hardmode, 1, 1, slota);
    ti_Write(&starfieldoff, 1, 1, slota);
    ti_SetArchiveStatus(true, slota);
}



