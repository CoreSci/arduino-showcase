# Raspberry Pi notes for the System Controller

Setup notes and screenshots from connecting the Raspberry Pi to the Arduino controller.

| File | Contents |
|---|---|
| `serial-interface-notes.txt` | Reading and testing the Pi's serial port (`/dev/ttyS0`, `dmesg \| grep tty`) |
| `apache-cgi-config.txt`, `apache-enable-cgi.jpg` | Enabling CGI scripts in Apache, for a small web control page |
| `gpio-terminal.png` | Driving GPIO pins from the shell with `gpio mode` / `gpio write` and small shell scripts |
| `gpio-readall.jpg`, `pinout-command.jpg` | Pin maps from `gpio readall` and `pinout` (Pi 3 Model B) |
| `hosts-file.jpg` | Default `/etc/hosts` on the Pi |

For the full OS, database and web-stack setup (with placeholders instead of real credentials), see `automation-workflow/raspberry-pi-setup/`.
