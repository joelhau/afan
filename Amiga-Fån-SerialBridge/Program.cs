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
 *
 * ---------------------------------------------------------------
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
sudo printf 'HEJ FRAN LINUX\r' > /run/amiga-com


 *Class EaglePty Skapar Pty
 *Class EagleLog skriver till logfil
 *Class EagleVolte hanterar röstsamtal hanterar 4g modeulen
 */




    static async Task Main()
    {
      //var pty = new EaglePty(); //Skapar pty objekt

      VälkomstText();
      EagleVoLTE eagle = new EagleVoLTE(true); //true=emulerad
      eagle.Start();

   
 
 
      var pty = new EaglePty(); //Skapar pty objekt
      var logger = new EagleLogg(); //skapar logger objekt
      //pty.DataReceived += logger.LogData; //on event från pty spara i loggobj.
     
      //pty.DataReceived += (data, antal) =>  //skapar en minifunktion, neat
        //Console.WriteLine("Från com-port: " + System.Text.Encoding.ASCII.GetString(data, 0, antal));
      //pty.DataReceived += data => KommandoParser("Från com-port: " + data);
    
    
    
        pty.DataReceived += (data, antal) =>
        MottagData(data, antal, logger);
      await pty.Start(); //Startar pty alltså pipe mot amigan
   
   
        
     



    }

static void MottagData(byte[] data, int antal, EagleLogg logger) //behövs för datan kommer bitvis
{
    string sparadMottagenData = "";
    sparadMottagenData += System.Text.Encoding.ASCII.GetString(data, 0, antal);

    while (sparadMottagenData.Contains("\r\n"))
    {
        int slut = sparadMottagenData.IndexOf("\r\n");

        string rad = sparadMottagenData.Substring(0, slut);

        sparadMottagenData =
            sparadMottagenData.Substring(slut + 2);

        Console.WriteLine("Från com-port: " + rad);

        logger.LogData(data, antal);

        KommandoParser(rad);
    }
}

static void KommandoParser(string text)
{
    Console.WriteLine("Kommando från com-port:");

    try
    {
        // Protokoll: "kommando" "Telefonnr" "Text" 0(flaggor)
        string[] delar = text.Split('"');

        string anrop = delar[0].Trim();
        string nummer = delar[1];
        string meddelande = delar[3];
        int flaggor = int.Parse(delar[4].Trim());

        Console.WriteLine("Anrop: {0}", anrop);
        Console.WriteLine("Telefonnr: {0}", nummer);
        Console.WriteLine("Text: {0}", meddelande);
        Console.WriteLine("Flaggor: {0}", flaggor);
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
