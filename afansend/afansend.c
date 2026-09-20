#include <stdio.h>
#include <string.h>
#include <exec/types.h> 
#include <exec/io.h> 
#include <devices/serial.h>
#include <exec/libraries.h>
#include <proto/exec.h>


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


*/

void skrivSerie(struct IOExtSer *io, char *text); //Deklarera funktion innan main såatteee
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


    char input[256];

    printf("AFÅN SERIAL CLI v0.1\n");
    printf("Serieport oppnad.\n");
    printf("Skriv kommando:\n");

    while (1)
    {
            printf("> ");
            if (fgets(input, sizeof(input), stdin) == NULL)
            break;

            input[strcspn(input, "\r\n")] = '\0';//Tabort enter

            if (strlen(input) == 0)
            continue;

            
            printf("SKICKAR: [%s]\n", input);
            skrivSerie(&io, input);//skriver till serieport
    }
    CloseDevice((struct IORequest *)&io); //stäng serieporten
    return 0;
                                                                                                            }

void skrivSerie(struct IOExtSer *io, char *text) { 
    io->IOSer.io_Data = (APTR)text; //lägg in texten
    io->IOSer.io_Length = strlen(text); //Längden på texten
    io->IOSer.io_Command = CMD_WRITE;
    DoIO((struct IORequest *)io); //Skicka
}
