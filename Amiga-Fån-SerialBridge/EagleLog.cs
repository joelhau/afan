using System.IO;

class EagleLogg
{
    private const string LogFile = "/var/log/amiga-com.log";//Sökväg

    private FileStream log;

    public EagleLogg()
    {
        log = new FileStream(
            LogFile,
            FileMode.Append,
            FileAccess.Write,
            FileShare.Read);//Skapa en filestream
            Console.WriteLine($"Logg: {LogFile}");

    }

    public async void LogData(byte[] buffer, int count)
    {
        
        await log.WriteAsync(
            buffer.AsMemory(0, count)); // Skriv till loggfilen

        await log.FlushAsync();//Vänta in filestreeamen
    }

    public void Close()
    {
        log.Close();
    }
}