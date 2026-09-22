using System.Runtime.InteropServices;
using Microsoft.Win32.SafeHandles;

class Program
{

/*
 *  █████╗ ███╗   ███╗██╗ ██████╗  █████╗
 * ██╔══██╗████╗ ████║██║██╔════╝ ██╔══██╗
 * ███████║██╔████╔██║██║██║  ███╗███████║
 * ██╔══██║██║╚██╔╝██║██║██║   ██║██╔══██║
 * ██║  ██║██║ ╚═╝ ██║██║╚██████╔╝██║  ██║
 * ╚═╝  ╚═╝╚═╝     ╚═╝╚═╝ ╚═════╝ ╚═╝  ╚═╝
 *
 *       <<>>  A M I G A FÅN S E R I A L  <<>>
 *                 L I N U X  //  P T Y
 *
 *             AFÅN TEAM EAGLE 2026
 *
 * ---------------------------------------------------------------
 *
 *  AMIGA<<>>Linux Serial Streamer v0.1
 *
 *  Ett litet Linux-hjälpprogram som:
 *
 *    [1] skapar ett virtuellt PTY-par
 *    [2] hämtar PTY-slavens Linux-sökväg
 *    [3] skapar en fast symlink för Amiga-emulatorn
 *    [4] lyssnar på data från PTY:n
 *    [5] loggar mottagna bytes
 *
 * ---------------------------------------------------------------
 *
 *  VERSION HISTORY
 *
 *  0.1  2026-09-05
 *       Skapar en virtuell PTY-port i Linux, Länkar slaven till
 *       en fast symlink och loggar inkommande seriell data.
 *        En enkel parser för att att bryta ner anrop över comporten
 *        Protokoll: "kommando" "Telefonnr" "Text" 0(flaggor)
 *        En class EagleVolte för att kommunicera med modemet    
 *   0.2 2026-09-21
 *      Eaglepty fixat rättighet till port för alla användare chmod(ptr, 0x1B6chmod0666
 *       EaglePty avaktiverade lite debugdata 
 *      Program kortade lite hur den hanterar event från eaglepty
 *      Fixade lite fulheter med emulering av modem, jävla rödvin
 *      Implemenerade parser RING och SMS
 *   0.3 2026-0922
 *      Kopplade modemet till parser
 *      Modememulering kan avbrytas via terminal
 *      Nu e det ju ganska färdigt för o provköra mkt enkelt men ändock
 *

 
 
 ---------------------------------------------------------------
 *
 *  Copyright (C) 2026 AFÅN Team Eagle
 *
 *  This program is free software: you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, either version 3
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see:
 *  https://www.gnu.org/licenses/
 *
 * ---------------------------------------------------------------
 *
Fellogg
 echo -e "AT+CPIN?\r" | sudo tee /dev/ttyUSB0
AT+CPIN?

udo chmod 666 /dev/pts/2
sudo printf 'HEJ FRAN LINUX\r\n' > /run/amiga-com
echo "HEJ FRÅN AMIGAN" > SER:

 *Class EaglePty Skapar Pty
 *Class EagleLog skriver till logfil
 *Class EagleVolte hanterar röstsamtal hanterar 4g modeulen
 */


    static string sparadMottagenData = ""; //static för o spara mottagen data i

    static async Task Main()
    {
    Console.CancelKeyPress += (sender, e) =>//ctrl+c hanterare
    {
        e.Cancel = true;
        Environment.Exit(0);
    };
      VälkomstText();
      EagleVoLTE eagle = new EagleVoLTE(true); //true=emulerad
      //eagle.Start();

   
 
 
      var pty = new EaglePty(); //Skapar pty objekt
      var logger = new EagleLogg(); //skapar logger objekt

        pty.DataReceived += (data, antal) => //event från pty
        { 
            sparadMottagenData +=  System.Text.Encoding.ASCII.GetString(data, 0, antal); //Lägg till i buffert
            while (sparadMottagenData.Contains("\r\n")) //loopa sålänge radbryt finns i buffert
    {           int radslut = sparadMottagenData.IndexOf("\r\n"); //hitta radbryt
                string rad = sparadMottagenData.Substring(0, radslut); //Spara allt innan radbryt i ny sträng
                sparadMottagenData = sparadMottagenData.Substring(radslut + 2); //Behåll text efter radbryt
                Console.WriteLine("Textrad: " + rad);//Skriv ut den utformaterade raden
                KommandoParser(rad, eagle); //anrop kommandoparser
    }
};

      await pty.Start(); //Startar pty alltså pipe mot amigan
   
   
        
     



    }


static void KommandoParser(string text, EagleVoLTE eagle)
{
    Console.WriteLine("Kommando från com-port:");

    try
    {
        // Protokoll: "kommando" "Telefonnr" "Text" 0(flaggor)
        // SKICKASMS
        // RING
string[] delar = text.Split('"');

string anrop = delar[1];
string nummer = delar[3];
string meddelande = delar[5];
int flaggor = int.Parse(delar[6].Trim());

        Console.WriteLine("Anrop: {0}", anrop);
        Console.WriteLine("Telefonnr: {0}", nummer);
        Console.WriteLine("Text: {0}", meddelande);
        Console.WriteLine("Flaggor: {0}", flaggor);

        if (anrop == "SKICKASMS")
        {
            Console.WriteLine("STARTA FUNKTION:eagle.SkickaSMS(nummer, meddelande");
            eagle.SkickaSMS(nummer, meddelande);
        }
        if (anrop == "RING")
        {
            Console.WriteLine("STARTA FUNKTION:eagle.Ring(nummer)");
            eagle.Ring(nummer);
        }
        if (anrop == "STARTAMODEM")
        {
            Console.WriteLine("STARTA FUNKTION:STARTAMODEM");
            eagle.AnslutRiktigtModem();
        }


    }
    catch
    {
        Console.WriteLine("Fel: Felaktigt kommandoformat.");
    }
}


    static void VälkomstText(){
          Console.WriteLine("");
          Console.WriteLine("  █████╗ ███╗   ███╗██╗ ██████╗  █████╗");
          Console.WriteLine(" ██╔══██╗████╗ ████║██║██╔════╝ ██╔══██╗");
          Console.WriteLine(" ███████║██╔████╔██║██║██║  ███╗███████║");
          Console.WriteLine(" ██╔══██║██║╚██╔╝██║██║██║   ██║██╔══██║");
          Console.WriteLine(" ██║  ██║██║ ╚═╝ ██║██║╚██████╔╝██║  ██║");
          Console.WriteLine(" ╚═╝  ╚═╝╚═╝     ╚═╝╚═╝ ╚═════╝ ╚═╝  ╚═╝");
          Console.WriteLine("");
          Console.WriteLine("       <<>>  A M I G A   S E R I A L  <<>>");
          Console.WriteLine("                 L I N U X  //  P T Y");
          Console.WriteLine("");
          Console.WriteLine("             AFÅN TEAM EAGLE 2026");

    }


        
}