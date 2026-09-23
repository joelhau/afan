#include <stdio.h>
#include <string.h>
#include <exec/types.h> 
#include <exec/io.h> 
#include <devices/serial.h>
#include <exec/libraries.h>
#include <proto/exec.h>
#include <stdbool.h>
#include <stdlib.h>


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


/*
Afån Serial CLI v0.1

Ett litet och medvetet primitivt CLI-program för Amigan. Inga konstigheter, inga ramverk och absolut ingen Python — 
bara C, `serial.device` och en gammal hederlig input-loop. 

Programmet öppnar Amigans serieport via `serial.device`, väntar på att användaren skriver ett kommando och 
trycker Enter. Raden skickas sedan direkt vidare till serieporten.


Just nu är det bara en dum terminal. Och det är precis meningen.


Röret som ska snacka från amigan med serialbridge i fånen

0.1 2026-09-20
    Bara ett experiment att lyckas skicka något från emulatorn till SerialBridge. Det funka
0.2
    Bytte till SendIO för o inte låsa upp programmet
    Skickar med radbrytningar
0.3 2026-09-30
    Nu börjar telefonen faktiskt likna en telefon.
    Menysystemet är i detta skede medvetet enkelt och ganska rörigt, ber om ursäkt för det.
    Input, menyer och funktioner ligger fortfarande tätt ihop och
    kodstrukturen är ungefär "få skiten att fungera först".
    v0.3 handlar alltså mer om funktion än om snygg kod.
    När telefonfunktionerna är testade ska menydelen skrivas om
    med renare struktur, gemensamma UI-funktioner och mindre
    hoppande mellan funktioner.
    Det ser ut som skit eftersom det just nu ska fungera,
    inte för att det ska vara vackert.
    0 kr i lön.
    100 % entusiasm.
    Den skickar kommando för sms å att ringa till serialbridge i Linux

*/

void skrivSerie(struct IOExtSer *io, char *text); //Deklarera funktion innan main såatteee
void kontrolleraFil(void); //Kolla så snabbnrfil finns
void lasLista(void);  //Läs in telefonlista 
void mnuHuvud();
char *mnuSms();
char *mnuRing();
char *las_text(void);
int las_val(void);



int meny(int aktiv);

int main(void)
{
    struct IOExtSer io; //structur för kommunikation med serial.device
    LONG error; //variabel för o spara felkod opendevice
    memset(&io, 0, sizeof(io)); //Rensa io

    error = OpenDevice( "serial.device", 0, (struct IORequest *)&io, 0 );//Öppna serieport

if (error != 0) { //Kolla felkod
    printf("Kunde inte oppna serieporten!\n"); 
    return 1; 
}

    char *inputText; //textpekare från funktion
    int aktivMeny=1;
    char input[256];
    char sendbuf[300];
    bool kor=true;
    printf("AFÅN SERIAL CLI v0.1\n");
    printf("Serieport oppnad.\n");
    printf("Skriv kommando:\n");
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
            mnuHuvud();
            
            break;

        case 2:
            
            skrivSerie(&io,mnuRing());
            break;

        case 3:
            skrivSerie(&io,mnuSms());
            break;
     
        case 4:
            printf("> ");
            if (fgets(input, sizeof(input), stdin) == NULL)
            break;

            input[strcspn(input, "\r\n")] = '\0';//Tabort enter

            if (strlen(input) == 0)
            continue;

            
            printf("SKICKAR: [%s]\n", input);
            //
//
            sprintf(sendbuf, "%s\r\n", input);
            skrivSerie(&io, sendbuf);
        break;
     
     
     
            case 9:
         printf("9 FUNKAR!\n");
            kor=false;
            break;
    }
         }

    CloseDevice((struct IORequest *)&io); //stäng serieporten
    return 0;
                                                                                                            }

char *las_text(void)
{
    static char text[161];

    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0';

    return text;
}

int las_val(void)
{
    char text[16];

    fgets(text, sizeof(text), stdin);

    return atoi(text);
}

void mnuHuvud(){
            printf("Huvudmeny\n\n");
            printf("1.Huvudmeny\n");
            printf("2.Ring nummer\n");
            printf("3.SMSa nummer\n");
            printf("x.SMSa snabbnr\n");
            printf("x.Redigera snabbnr\n");
            printf("x.Laes SMS\n");
            printf("9.Avbryt\n");
}
char *mnuRing(){
        char *inputText; //textpekare från funktion
        static char kommando[256];

            printf("Sla in nummer att ringa\n");
            inputText = las_text();
            printf("Ringer.....\n");
              //$ printf '"STARTAMODEM" "435345" "LINUX" 0\r\n' > /run/amiga-com
        //return "'RING'" + inputText "LINUX" 0\r\n'
        sprintf(kommando, "\"RING\" \"%s\" \"LINUX\" 0\r\n", inputText);

    return kommando;
}
char *mnuSms(){
     static char kommando[256];

                char *inputNr; //textpekare från funktion
                char *inputText; //textpekare från funktion

            printf("Sla in nummer att SMSa\n");
            inputNr = las_text();
            printf("Skriv text\n");
            inputText = las_text();
            printf("Skickar\n");
            sprintf(kommando, "\"SKICKASMS\" \"%s\" \"%s\" 0\r\n",
            inputNr, inputText);
            return kommando;
        }


void skrivSerie(struct IOExtSer *io, char *text) { 
    printf("SKICKAR: [%s]\n", text);
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
        printf("Kunde inte skapa snabbnummer-filen!\n"); 
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
        if (fil == NULL) { printf("Kunde inte oppna snabbnummer-filen!\n"); return; } 
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
                printf("Kunde inte spara snabbnummer!\n"); 
                return; 
        } 
        for (i = 0; i < MAX_SNABBNR; i++) { //Loopa o spara 
            fprintf( fil, "%s|%s\n", snabbnr[i].namn, snabbnr[i].nummer ); 
        } 
        fclose(fil); 
    }