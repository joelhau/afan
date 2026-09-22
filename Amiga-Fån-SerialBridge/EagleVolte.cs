using System.IO.Ports;

class EagleVoLTE
{
    private SerialPort Modem;
    private bool emulated;
    public EagleVoLTE(bool emulated)
    {
        if (emulated){
                Modem = null; //Skapar modemet
                this.emulated = true;
        } else {
        Modem = new SerialPort(GetModemPort(), GetModemBaud()); //Skapar modemet
        this.emulated = false;
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

        if (!emulated) //Ha va snitsigt skicka bara till modem om det inte e emulerat moahahahha
        {
            Modem.Write(command + "\r");
        }

        
    }
    
    public void AnslutRiktigtModem() //Funktion för att lämna emulering
    {
    if (!emulated)
        return;

    emulated = false;

    Modem = new SerialPort(GetModemPort(), GetModemBaud());
    Modem.Open();
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