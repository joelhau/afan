```text
╔══════════════════════════════════════════════════════════════════╗
║                                                                  ║
║       █████╗ ███╗   ███╗██╗ ██████╗  █████╗                      ║
║      ██╔══██╗████╗ ████║██║██╔════╝ ██╔══██╗                     ║
║      ███████║██╔████╔██║██║██║  ███╗███████║                     ║
║      ██╔══██║██║╚██╔╝██║██║██║   ██║██╔══██║                     ║
║      ██║  ██║██║ ╚═╝ ██║██║╚██████╔╝██║  ██║                     ║
║      ╚═╝  ╚═╝╚═╝     ╚═╝╚═╝ ╚═════╝ ╚═╝  ╚═╝                     ║
║                                                                  ║
║          <<>> A M I G A   S E R I A L <<>>                       ║
║                L I N U X  /  P T Y                               ║
║                                                                  ║
║                  AFÅN TEAM EAGLE 2026                            ║
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝


             AFån Serial Communication / ASC
             ─────────────────────────────────

                 "THE EAGLE HAS LANDED."


Vadhän detta?
─────────────

Eagle är Afåns lilla Linux-kompis.

Amigan skickar text över comporten.
Linux tar emot o processar.

Tanken är att Amigan ska kunna prata med Linux
och använda saker som 4G, SMS och andra tjänster.


──────────────────────────────────────────────────────────────────

THE MAGIC PORT
──────────────

Amigan använder alltid:

    /run/amiga-com

Den pekar automatiskt på rätt virtuell seriell port.


Kontrollera:

    ls -l /run/amiga-com


Du ska se något i stil med:

    /run/amiga-com -> /dev/pts/6


──────────────────────────────────────────────────────────────────


SÄND ETT KOMMANDO
─────────────────

Formatet är:

    KOMMANDO "PARAMETER" "TEXT" FLAGGOR


Exempel:

    SKICKASMS "+46701234567" "Hej från Afån!" 0


Det betyder:

    SKICKASMS
    └─ nummer: +46701234567
    └─ text:   Hej från Afån!
    └─ flaggor: 0


──────────────────────────────────────────────────────────────────


SHELL-TEST
──────────

Vill du testa utan Amigan?

    printf 'SKICKASMS "+46701234567" "E du död?!" 0\r\n' \
    | sudo tee /run/amiga-com > /dev/null


Om allt fungerar dyker kommandot upp i ASC.


Exempel:

    Från com-port:
    SKICKASMS "+46701234567" "E du död?!" 0


──────────────────────────────────────────────────────────────────


ÖRNENS PROTOKOLL
────────────────

Kommandon skickas som vanlig text.

Inga JSON-monster.
Inga molntjänster.
Inga 47 bibliotek.

Bara:

    KOMMANDO "DATA" "DATA" FLAGGOR, SLAPPT O DÅLIGT


                         /\_/\
                        ( o.o )
                         > ^ <
                       THE EAGLE


──────────────────────────────────────────────────────────────────


TROUBLESHOOTING
───────────────

Om /run/amiga-com inte finns:

    sudo rm /run/amiga-com

Starta sedan Eagle igen.


Om du får:

    Åtkomst nekas

använd:

    printf 'DITT KOMMANDO' | sudo tee /run/amiga-com > /dev/null


──────────────────────────────────────────────────────────────────


STATUS O MÅL MED 1.0
────────────────────

    [OK]  PTY
    [OK]  SERIAL LINK
    [OK]  LOGGER
    [SNART]  AMIGA
	[SNART]  PROTOKOLL SKICKA ENKELT SMS MAX 160 TECKEN
	[SNART]  PROTOKOLL TANKA NER SMS
	[SNART]  PROTOKOLL TÖM SMS
	[SNART]  PROTOKOLL RING NR
	[SNART]  PROTOKOLL SVARA
	
	
	
	

╔══════════════════════════════════════════════════════════════════╗
║                                                                  ║
║                 AFÅN TEAM EAGLE 2026                             ║
║                                                                  ║
║          AMIGA <<>> LINUX <<>> THE OUTSIDE WORLD                 ║
║                                                                  ║
║                  "SEND THE MESSAGE."                             ║
║                                                                  ║
╚══════════════════════════════════════════════════════════════════╝
```




Starta emulering av 4gmodem med nullport
sudo apt install socat
socat -d -d pty,raw,echo=0 pty,raw,echo=0

programmet måste ha root

prova comport i shell
$ printf 'SKICKASMS "+46701234567" "E du död?!" 0\r\n' | sudo tee /run/amiga-com > /dev/null

buggar länkar inte om, fixar idag skapa länk fallit bort ur konstruktorn

AT+CMGS="+46767738698"
AT+CMGS="+46763122024"

