#include <stdio.h>
#include <string.h>
#include <exec/types.h> 
#include <exec/io.h> 
#include <devices/serial.h>
#include <exec/libraries.h>
#include <proto/exec.h>
#include <stdbool.h>
#include <stdlib.h>
#include <proto/dos.h>
#include <dos/dosextens.h>
#include <intuition/intuition.h>
#include <graphics/gfxbase.h>
#include <proto/intuition.h>
#include <proto/graphics.h>

//Telefonboken
#define MAX_SNABBNR 100 
#define MAX_NAMN 40 
#define MAX_NUMMER 30 
#define FILNAMN "PROGDIR:snabbnr.dat" 

struct Snabbnummer { //Structen för poster
    char namn[MAX_NAMN]; 
    char nummer[MAX_NUMMER]; 
}; 

struct Snabbnummer snabbnr[MAX_SNABBNR]; //Listan över nr

struct IntuitionBase *IntuitionBase;
struct GfxBase *GfxBase;

/*
Afån Serial CLI v0.1

Ett litet och medvetet primitivt CLI-program för Amigan. Inga konstigheter, inga ramverk och absolut ingen Python ? 
bara C, `serial.device` och en gammal hederlig input-loop. 

Programmet öppnar Amigans serieport via `serial.device`, väntar på att användaren skriver ett kommando och 
trycker Enter. Raden skickas sedan direkt vidare till serieporten.


Just nu är det bara en dum terminal. Och det är precis meningen.


Röret som ska snacka från amigan med serialbridge i fånen

Koden e rörig o hemsk men nu funkar det viktigaste en backup innan renskrivning bara :) hehe

*/

void skrivSerie(struct IOExtSer *io, char *text); //Deklarera funktion innan main såatteee
void kontrolleraFil(void); //Kolla så snabbnrfil finns
void lasLista(void);  //Läs in telefonlista 
void mnuHuvud();
void skrivText(char *text);
void mnuHuvud(void);
int las_val(void);
void tryckEnter(void);
void tomSkarm(void);

char *mnuStarta();
char *mnuSms();
char *mnuRing();
char *las_text(void);
int las_val(void);
struct Window *afonWindow;
struct RastPort *afonRP;

int textY = 20;
bool kor = true;
int meny(int aktiv);

int main(void)
{
    char *input;
    char rad[300];
    char sendbuf[300];



    IntuitionBase = (struct IntuitionBase *)OpenLibrary(
        "intuition.library", 37);

    GfxBase = (struct GfxBase *)OpenLibrary(
        "graphics.library", 37);
if (IntuitionBase == NULL || GfxBase == NULL)
{
    if (GfxBase != NULL)
        CloseLibrary((struct Library *)GfxBase);

    if (IntuitionBase != NULL)
        CloseLibrary((struct Library *)IntuitionBase);

    return 1;
}
struct Screen *wbScreen;

wbScreen = IntuitionBase->FirstScreen;

afonWindow = OpenWindowTags(NULL,
    WA_CustomScreen, wbScreen,
    WA_Left, 0,
    WA_Top, 0,
    WA_Width, wbScreen->Width,
    WA_Height, wbScreen->Height,
    WA_Title, (ULONG)"AFÅNSEND",
    WA_CloseGadget, TRUE,
    WA_DragBar, TRUE,
    WA_DepthGadget, TRUE,
    WA_IDCMP, IDCMP_CLOSEWINDOW | IDCMP_VANILLAKEY,
    TAG_END
);
ActivateWindow(afonWindow);

if (afonWindow == NULL)
{
    CloseLibrary((struct Library *)GfxBase);
    CloseLibrary((struct Library *)IntuitionBase);
    return 1;
}


afonRP = afonWindow->RPort;
        //slut test
SetAPen(afonRP, 1);
SetBPen(afonRP, 0);

    struct IOExtSer io; //structur för kommunikation med serial.device
    LONG error; //variabel för o spara felkod opendevice
    memset(&io, 0, sizeof(io)); //Rensa io

    error = OpenDevice( "serial.device", 0, (struct IORequest *)&io, 0 );//Öppna serieport
if (error != 0)
{
    skrivText("Kunde inte oppna serieporten!");

    CloseWindow(afonWindow);
    CloseLibrary((struct Library *)GfxBase);
    CloseLibrary((struct Library *)IntuitionBase);

    return 1;
} 

    char *inputText; //textpekare från funktion
    int aktivMeny=1;
    skrivText("AFÅN SERIAL CLI v0.1");
    skrivText("Serieport oppnad.");
    skrivText("Skriv kommando:");
    kontrolleraFil();  //Kolla så snabbnrfil finns
    lasLista();  //Läs in telefonlista 
        // Protokoll: "kommando" "Telefonnr" "Text" 0(flaggor)
        // SKICKASMS
        // RING
        //STARTAMODEM
        //$ printf '"STARTAMODEM" "435345" "LINUX" 0\r\n' > /run/amiga-com
           mnuHuvud();
     
    while (kor)
    {


     

         switch (las_val()) {

        case 1:
            skrivSerie(&io,mnuStarta());
            tryckEnter();
            tomSkarm();
            mnuHuvud();
            
            break;

        case 2:
            
            skrivSerie(&io,mnuRing());
            tryckEnter();
            tomSkarm();
            mnuHuvud();
            break;

        case 3:
            skrivSerie(&io,mnuSms());
            tryckEnter();
            tomSkarm();
            mnuHuvud();
            break;
     
        case 4:
  
            break;
            
     
     
            case 9:
                skrivText("9 Börjar stänga ner anslutning.....");
                kor=false;
            break;
    }
         }

CloseDevice((struct IORequest *)&io);
skrivText("AFONSEND SLUTAR NU");
CloseWindow(afonWindow);
CloseLibrary((struct Library *)GfxBase);
CloseLibrary((struct Library *)IntuitionBase);


    return 0;
                                                                                                            }


int las_val(void)
{
    char text[16];
    char *resultat;

    resultat = las_text();

    if (resultat == NULL)
        return 9;

    strcpy(text, resultat);

    return atoi(text);
}
void mnuHuvud()
{
    skrivText("Huvudmeny");
    skrivText("");
    skrivText("1. Aktivera 4G-Modul");
    skrivText("2. Ring nummer");
    skrivText("3. SMSa nummer");
    skrivText("x. SMSa snabbnr");
    skrivText("x. Redigera snabbnr");
    skrivText("x. Läs SMS");
    skrivText("9. Avbryt");
}
char *mnuRing(){
        char *inputText; //textpekare från funktion
        static char kommando[256];

            skrivText("Slå in nummer att ringa");
            inputText = las_text();
            skrivText("Ringer.....");
        sprintf(kommando, "\"RING\" \"%s\" \"LINUX\" 0\r\n", inputText);
    return kommando;
}
char *mnuSms(){
     static char kommando[256];

                char *inputNr; //textpekare från funktion
                char *inputText; //textpekare från funktion

            skrivText("Slå in nummer att SMSa");
            inputNr = las_text();
            skrivText("Skriv text");
            inputText = las_text();
            skrivText("Skickar");
            sprintf(kommando, "\"SKICKASMS\" \"%s\" \"%s\" 0\r\n",
            inputNr, inputText);
            return kommando;
        }
char *mnuStarta(){
     static char kommando[256];

                char *inputNr; //textpekare från funktion
                char *inputText; //textpekare från funktion

            sprintf(kommando, "\"STARTAMODEM\" \"0\" \"LINUX\" 0\r\n");
            return kommando;
        }

     //$ printf '"STARTAMODEM" "435345" "LINUX" 0\r\n' > /run/amiga-com
   


void tryckEnter(void)
{
    skrivText("Tryck Enter for att fortsatta...");
    las_text();
}

void skrivSerie(struct IOExtSer *io, char *text) { 
 
    char rad[300];
    sprintf(rad, "SKICKAR: %s", text);
    skrivText(rad);
    io->IOSer.io_Data = (APTR)text; //lägg in texten
    io->IOSer.io_Length = strlen(text); //Längden på texten
    io->IOSer.io_Command = CMD_WRITE;
    SendIO((struct IORequest *)io); //Skicka
}

void tomLista(void) { //Skapa tom telefonlistna
    int i; for (i = 0; i < MAX_SNABBNR; i++) { 
        snabbnr[i].namn[0] = '\0'; snabbnr[i].nummer[0] = '\0'; 
    } 
}

void kontrolleraFil(void) { //Kolla så fil finns
    FILE *fil; int i; fil = fopen(FILNAMN, "r"); 
    if (fil != NULL) {  //Filen finns ABORT!! 
        fclose(fil); 
        return; 
    } 
    fil = fopen(FILNAMN, "w"); 
    if (fil == NULL) { //Fel med o skapa fil
        skrivText("Kunde inte skapa snabbnummer-filen!"); 
        return; 
    } 
    for (i = 0; i < MAX_SNABBNR; i++) { //Skapar filen
        fprintf(fil, "\n"); 
    } 
    fclose(fil); } 
    
    void lasLista(void) { //Läs in telefonlista 
        FILE *fil; char rad[100]; 
        int plats = 0; char *separator; 
        tomLista(); 
        fil = fopen(FILNAMN, "r"); 
        if (fil == NULL) { skrivText("Kunde inte oppna snabbnummer-filen!"); return; } 
        while (fgets(rad, sizeof(rad), fil) != NULL && plats < MAX_SNABBNR) { 
            rad[strcspn(rad, "\r\n")] = '\0'; 
            separator = strchr(rad, '|');
            if (separator != NULL) { 
                *separator = '\0'; 
                strncpy( snabbnr[plats].namn, rad, MAX_NAMN - 1 ); 
                snabbnr[plats].namn[MAX_NAMN - 1] = '\0'; 
                strncpy( snabbnr[plats].nummer, separator + 1, MAX_NUMMER - 1 ); 
                snabbnr[plats].nummer[MAX_NUMMER - 1] = '\0'; 
            } plats++; 
        } fclose(fil); 
    } 
    
    int laggTillSnabbnr(char *namn, char *nummer) { //* * Lägger till ett nytt snabbnummer. * * Returnerar: * 0 = lyckades * 1 = listan är full */ 
        int i; 
        for (i = 0; i < MAX_SNABBNR; i++) { //Leta ledig plats 
            if (snabbnr[i].namn[0] == '\0') { 
                strncpy( snabbnr[i].namn, namn, MAX_NAMN - 1 ); 
                snabbnr[i].namn[MAX_NAMN - 1] = '\0'; 
                strncpy( snabbnr[i].nummer, nummer, MAX_NUMMER - 1 ); 
                snabbnr[i].nummer[MAX_NUMMER - 1] = '\0'; 
                return 0; 
            } 
        } 
       return 1; //Ingel ledig plats 
    } 
    
    void sparaLista(void) { 
        FILE *fil; int i; 
        fil = fopen(FILNAMN, "w"); //w=skriv om filen
        if (fil == NULL) { 
                skrivText("Kunde inte spara snabbnummer!"); 
                return; 
        } 
        for (i = 0; i < MAX_SNABBNR; i++) { //Loopa o spara 
            fprintf( fil, "%s|%s\n", snabbnr[i].namn, snabbnr[i].nummer ); 
        } 
        fclose(fil); 
    }
void skrivText(char *text)
{
    Move(afonRP, 8, textY);
    Text(afonRP, text, strlen(text));

    textY += 10;
}

void tomSkarm(void)
{
    SetAPen(afonRP, 0);

    RectFill(afonRP,
             5, 12,
             afonWindow->Width - 10,
             afonWindow->Height - 10);

    SetAPen(afonRP, 1);
    SetBPen(afonRP, 0);
    SetDrMd(afonRP, JAM2);

    textY = 20;
}

char *las_text(void)
{
    static char text[161];
    int pos = 0;
    int startX = 8;
    int startY = textY;

    struct IntuiMessage *msg;

    text[0] = '\0';

    while (kor)
    {
        WaitPort(afonWindow->UserPort);

        while ((msg = (struct IntuiMessage *)GetMsg(afonWindow->UserPort)))
        {
            if (msg->Class == IDCMP_CLOSEWINDOW)
            {
                kor = false;
                ReplyMsg((struct Message *)msg);
                return NULL;
            }

            if (msg->Class == IDCMP_VANILLAKEY)
            {
                if (msg->Code == 13)
                {
                    text[pos] = '\0';

                    textY += 10;

                    ReplyMsg((struct Message *)msg);
                    return text;
                }

                if (msg->Code == 8)
                {
                    if (pos > 0)
                    {
                        pos--;
                        text[pos] = '\0';

                        /* Rita om hela inmatningsraden */
                        SetAPen(afonRP, 0);
                        SetDrMd(afonRP, JAM2);

                        Move(afonRP, startX, startY);
                        Text(afonRP, text, strlen(text));
                    }
                }
                else if (msg->Code >= 32 && msg->Code <= 126)
                {
                    if (pos < 160)
                    {
                        text[pos] = msg->Code;
                        pos++;
                        text[pos] = '\0';

                        /* Visa det vi skriver */
                        SetAPen(afonRP, 1);
                        SetBPen(afonRP, 0);
                        SetDrMd(afonRP, JAM2);

                        Move(afonRP, startX, startY);
                        Text(afonRP, text, strlen(text));
                    }
                }
            }

            ReplyMsg((struct Message *)msg);
        }
    }

    return NULL;
}