using System.IO.Ports;

class EagleVoLTE
{
    private SerialPort Modem;

    public EagleVoLTE(bool emulated)
    {
        if (emulated){
                Modem = new SerialPort("/dev/pts/1", GetModemBaud()); //Skapar modemet  
        } else {
        Modem = new SerialPort(GetModemPort(), GetModemBaud()); //Skapar modemet
        }
    }

    public void Start()
    {
        // Öppna modemets AT-port
        Modem.Open();
    }
    private string GetModemPort(){ //Funktion som returnerar port till modemet
        return "/dev/sim7600-at";
    }

    private int GetModemBaud(){ //Funktion som returnerar baudraten
        return 115200;
    }

        private void Send(string command){ //Skickar till modem o lägger in ett litet enter

        Modem.Write(command + "\r");
    }
    
    public void Ring(string nummer)
    {
        Send("ATDT" + nummer + ";"); //Skicka ring o nr
    }

    public void LäggPå()
    {
        Send("ATH");// Skicka Lägg på
    }

    public void Svara()
    {
         Send("ATA");// Skicka Svara
    }

    public void DödaModem()
    {
        Modem.Close(); // Stäng modemport
    }

    public void SkickaSMS(string nummer, string text)
    {
    Send("AT+CMGF=1"); // Ställ in textläge
    Thread.Sleep(200); //Vänta in modem
    Send("AT+CMGS=\"" + nummer + "\""); //SMS Läge
    Thread.Sleep(200); //Vänta in modem
    Modem.Write(text); //Skicka Texten
    Modem.Write("\x1A");// Ctrl+Z = avsluta textinmatning å skicka SMS
    Thread.Sleep(3000); //Vänta in modem
    }
}