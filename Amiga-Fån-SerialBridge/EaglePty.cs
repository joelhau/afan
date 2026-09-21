using System.Runtime.InteropServices;
using Microsoft.Win32.SafeHandles;


class EaglePty
{
    //Linux mumbojumbo
    const int O_RDWR = 2; // Öppna port för både läsning och skrivning.
    const int O_NOCTTY = 0x100; // Behåll terminal och använd PTY bara för kommuniakation

    //libc anrop
    [DllImport("libc", SetLastError = true)] //Hämta linux funktion
    static extern int posix_openpt(int flags); //libc för o skapa PTY Master
    
    [DllImport("libc", SetLastError = true)] //skapa slave
    static extern int grantpt(int fd); //skapa slave

    [DllImport("libc", SetLastError = true)] //Hämta linux funktion
    static extern int unlockpt(int fd); //Gör slavände öppningsbar

    [DllImport("libc")] //Hämta linux funktion
    static extern IntPtr ptsname(int fd); // Hämtar namnet på slave-sidan. Exempel:  /dev/pts/2

    [DllImport("libc", SetLastError = true)] //Hämta linux funktion
    static extern int symlink(string target, string linkpath); // Skapar symbolisk länk.

    [DllImport("libc", SetLastError = true)]
    static extern int open(string pathname, int flags); // Öppnar PTY-slaven och håller den öppen för att förhindra EIO när Amiberry stänger porten.

    //[DllImport("libc", SetLastError = true)]
    //static extern int chmod(string pathname, uint mode); // Ändrar rättigheter på PTY-slaven

    [DllImport("libc", SetLastError = true)]
    static extern int chmod(IntPtr pathname, uint mode); // Ändrar rättigheter på PTY-slaven

    //lite egna variabler
    public const string link = "/run/amiga-com"; // Fast adress som länkar PTY som Amiberry alltid använder. PTY kan ändras vid omstart
    private int master; //Håller linux FD referens till masterport
    private string slave; //sökvägen till slaven
    private int slaveFd; //håller linux FD referens och hindrar slaven från att stängas
    public event Action<byte[], int>? DataReceived; //Eventhanterare när pty tagit emot data

    public EaglePty(){
        master = createPtyMaster();//Ger PTY mastern sin FD referens
        slave = createPtySlave(master);//sökväg till slav
        slaveFd = tvingaSlavenOpen(); //Förhindrar att slaven går o dör
        skapaSymLink();//skapa länken hur kunde ja glömma den?
    }

    private int createPtyMaster(){
            // Skapa PTY master FD linux referens, Master e linux ände av den virtuella kabeln
            int master = posix_openpt(O_RDWR | O_NOCTTY);
            if (master < 0)
            {
                throw new Exception(
                    $"Kunde inte skapa PTY. " +
                    $"Linux error: {Marshal.GetLastPInvokeError()}");
            }
            
            return master;
    }

    private string createPtySlave(int master){       
        //Skapar PTY Slaven
        if (grantpt(master) != 0) throw new Exception("grantpt() misslyckades."); //// Förbered PTY-slaven. me hjälp av master FD
        if (unlockpt(master) != 0) throw new Exception("unlockpt() misslyckades."); // Tillåt att PTY-slaven öppnas.
        IntPtr ptr = ptsname(master); //fråga linux vilken slave som hör till master
        if (ptr == IntPtr.Zero) throw new Exception("Kunde inte hitta PTY slave."); //Hitta ingen
        Console.WriteLine($"PTY slave: {Marshal.PtrToStringAnsi(ptr)}");
        // Ger användaren läs- och skrivrättighet via /run/amiga-com        
        if (chmod(ptr, 0x1B6) != 0)
        throw new Exception($"Kunde inte ändra PTY-rättigheter. Linux error: {Marshal.GetLastPInvokeError()}");//Ge alla användare skrivrättighet till port

        
        return Marshal.PtrToStringAnsi(ptr)!; //Konvertera till string, inte null!    
    }

    private int tvingaSlavenOpen(){
        int slaveFd = open(slave,O_RDWR | O_NOCTTY);//Tvingar slave att hålla öppet
        if (slaveFd < 0) //Felhanterare
        {
            throw new Exception(
                $"Kunde inte öppna PTY slave. " +
                $"Linux error: {Marshal.GetLastPInvokeError()}");
        }
        Console.WriteLine($"Slave hålls öppen. FD: {slaveFd}");
        return slaveFd;

    }

    private void skapaSymLink(){
        // Skapar länken så våran amiga kan hitta porten oavsett nr
        if (File.Exists(link))
        {
            File.Delete(link);
        }


        if (symlink(slave, link) != 0)
        {
            throw new Exception(
                $"Kunde inte skapa symlink. " +
                $"Linux error: {Marshal.GetLastPInvokeError()}");
        }

    }

    public async Task Start(){
        // Buffer för inkommande data.
        byte[] buffer = new byte[256];


        //en mkt cool funktion för o starta PTY
        Console.WriteLine($"Fast port: {link}");
        Console.WriteLine($"Länk: {link} -> {slave}");
        Console.WriteLine();
        Console.WriteLine("PTY:n är aktiv.");
        Console.WriteLine("Väntar på data...");
        Console.WriteLine("Avsluta med Ctrl+C.");

     
        using var masterHandle =
            new SafeFileHandle(
                (IntPtr)master,
                ownsHandle: true);//skapa objekt av masters fd nr

        using var stream =
            new FileStream(
                masterHandle,
                FileAccess.ReadWrite); //skapa en ström av det




        while (true) //Läs från pty port loop
        {
            try
            {
                
                int count = await stream.ReadAsync(buffer);// Vänta på bytes från Amiberry/Amiga.

                if (count == 0) // 0 bytes betyder att streamen e död
                {
                    Console.WriteLine(
                        "PTY master stängdes.");
                    break;
                }
                DataReceived?.Invoke(buffer, count);//Anropa alla som lyssnar på event. Logger T.ex. Mhmm
            }
            catch (IOException ex)
            {
                Console.WriteLine(
                    $"I/O-fel: {ex.Message}");//Visa IO fel innan den dör
            }
        }


      

    }

    public void close(){
        close(slaveFd); //döda
    [DllImport("libc", SetLastError = true)] //döda
    static extern int close(int fd); //döda
    }



}