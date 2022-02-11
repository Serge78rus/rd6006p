# rd6006p

Command line utility for RD6006P power source

## Dependency

libmodbus

libmodbus-dev (development only)

## Building and installation

```bash
make
sudo make install 
```
Last command install application binary file to /usr/local/bin. 

## Usage

```bash
Usage: rd6006p options

options:

-h, --help                       show this help screen and exit
-V, --version                    show version information and exit
    --verbose          optional  show verbose information
                                 (default: false)
-d, --device=NAME      required  serial communication device
-b, --baudrate=VALUE   optional  communication baudrate
                                 (default: 115200)
-s, --slave=VALUE      optional  slave address
                                 (default: 1)
-v, --voltage=VALUE    optional  set output voltage value
                                 (default: no set)
-c, --current=VALUE    optional  set output current value
                                 (default: no set)
-o, --output=STATE     optional  set output state
                                 valid STATE: 0, off, 1, on
                                 (default: no set)
-C, --cycle=VALUE      optional  display status cycle in seconds
                                 (default: 1.0)
-n, --number=VALUE     optional  display status cycles count
                                 (default: 0 - no limit)
-S, --seconds          optional  show how many seconds have passed since the start
                                 (default: false)
-t, --time             optional  show status time
                                 (default: false)
-D, --date=FORMAT      optional  format specifier for date part of status time,
                                 for example "%d.%m.%Y" produce "DD.MM.YYYY" string
                                 (use "man strftime" for more information)
                                 (default: empty string - not show date)
-i, --info             optional  show device information
                                 (default: false)
```
                                 