v 20151031 2
C 48300 42000 0 0 0 title-A2.sym
C 54700 52400 1 0 0 in-1.sym
{
T 54700 53550 5 8 0 0 0 0 1
symversion=0.3
T 55300 52500 5 10 0 0 0 0 1
footprint=none
T 54700 52700 5 10 1 1 0 0 1
refdes=usb_power
T 54700 52700 5 10 0 0 0 0 1
device=INPUT
}
C 54700 51400 1 0 0 in-1.sym
{
T 54700 52550 5 8 0 0 0 0 1
symversion=0.3
T 55300 51500 5 10 0 0 0 0 1
footprint=none
T 54800 51700 5 10 1 1 0 0 1
refdes=jtag_power
T 54700 51700 5 10 0 0 0 0 1
device=INPUT
}
C 60900 51100 1 0 0 MAX882-gt.sym
{
T 61200 54050 5 10 0 0 0 0 1
symversion=0.1
T 62400 53300 5 10 1 1 0 0 1
refdes=U30
T 61700 52300 5 10 0 1 0 0 1
device=MAX882
T 61800 51900 5 10 0 1 0 0 1
footprint=SO8
}
C 65300 52100 1 0 0 3.3V-plus-1.sym
C 63400 48800 1 0 0 gnd-1.sym
C 63300 51000 1 270 0 capacitor-2.sym
{
T 63700 50700 5 10 1 1 0 0 1
refdes=C32
T 63700 50500 5 10 1 1 0 0 1
value=2.2u
T 63800 50300 5 10 0 1 0 0 1
footprint=SMD_POLAR-120-60
T 63800 51100 5 10 0 1 0 0 1
device=POLARIZED_CAPACITOR
T 63800 50900 5 10 0 0 0 0 1
symversion=0.1
}
C 57300 51000 1 270 0 capacitor-2.sym
{
T 57800 50600 5 10 1 1 0 0 1
refdes=C30
T 57800 50400 5 10 1 1 0 0 1
value=2.2u
T 57300 51000 5 10 0 1 0 0 1
footprint=SMD_POLAR-120-60
T 57500 51700 5 10 0 0 0 0 1
device=POLARIZED_CAPACITOR
T 57500 51900 5 10 0 0 0 0 1
symversion=0.1
}
N 63000 51900 65500 51900 2
N 65500 51900 65500 52100 2
N 63500 51900 63500 51000 2
N 58500 51000 58500 52500 4
N 57500 49500 63500 49500 4
N 63500 49100 63500 50100 4
N 58500 49500 58500 50100 4
N 61500 51100 61500 49500 4
N 62000 51100 62000 49500 4
C 55800 52400 1 0 0 resistor-2.sym
{
T 56000 52700 5 10 1 1 0 0 1
refdes=R30
T 56200 52200 5 10 1 1 0 0 1
value=0
T 56500 52700 5 10 0 1 0 0 1
footprint=SM0805
T 56200 52750 5 10 0 0 0 0 1
device=RESISTOR
}
C 55800 51400 1 0 0 resistor-2.sym
{
T 56000 51700 5 10 1 1 0 0 1
refdes=R31
T 56200 51200 5 10 1 1 0 0 1
value=0
T 56400 51700 5 10 0 1 0 0 1
footprint=SM0805
T 56200 51750 5 10 0 0 0 0 1
device=RESISTOR
}
N 60900 52500 56700 52500 4
N 60900 52900 59500 52900 4
N 55800 51500 55300 51500 4
N 55800 52500 55300 52500 4
C 58300 51000 1 270 0 capacitor-1.sym
{
T 58800 50600 5 10 1 1 0 0 1
refdes=C31
T 58800 50400 5 10 1 1 0 0 1
value=100n
T 58300 51000 5 10 0 1 0 0 1
footprint=SM1206
T 58500 51700 5 10 0 0 0 0 1
device=CAPACITOR
T 58500 51900 5 10 0 0 0 0 1
symversion=0.1
}
N 57500 49500 57500 50100 4
N 57500 51000 57500 52500 4
N 57000 52500 57000 51500 4
N 57000 51500 56700 51500 4
T 65700 42900 9 20 1 0 0 0 1
gTAG - Power Supply Unit
T 64600 42400 9 10 1 0 0 0 1
gTAG-psu.sch
T 64600 42100 9 10 1 0 0 0 1
4
T 66100 42100 9 10 1 0 0 0 1
5
T 68600 42400 9 10 1 0 0 0 1
$Revision$
T 68600 42100 9 10 1 0 0 0 1
Stefan Petersen (spe@stacken.kth.se)
N 62300 51100 62300 49500 4
C 48600 42200 1 0 0 copyleft.sym
{
T 50900 45400 5 10 0 0 0 0 1
device=none
}
C 59600 50100 1 90 0 resistor-2.sym
{
T 59700 50600 5 10 1 1 0 0 1
refdes=R33
T 59700 50400 5 10 1 1 0 0 1
value=39k
T 59700 50200 5 10 0 1 0 0 1
footprint=SM0805
T 59700 50800 5 10 0 1 0 0 1
device=RESISTOR
}
C 59600 51400 1 90 0 resistor-2.sym
{
T 59700 51900 5 10 1 1 0 0 1
refdes=R32
T 59700 51700 5 10 1 1 0 0 1
value=100k
T 59700 51800 5 10 0 1 0 0 1
footprint=SM0805
T 60000 51750 5 10 0 0 0 0 1
device=RESISTOR
}
N 60900 51800 60500 51800 4
N 60500 51800 60500 51200 4
N 60500 51200 59500 51200 4
N 59500 51000 59500 51400 4
N 59500 50100 59500 49500 4
N 59500 52300 59500 52500 4
N 59500 52500 59500 52900 4
C 63600 53000 1 90 0 resistor-2.sym
{
T 63700 53600 5 10 1 1 0 0 1
refdes=R34
T 63700 53400 5 10 1 1 0 0 1
value=10k
T 63700 53200 5 10 0 1 0 0 1
footprint=SM0805
T 63700 53750 5 10 0 1 0 0 1
device=RESISTOR
}
C 65500 52900 1 0 0 out-1.sym
{
T 66100 53450 5 10 0 0 0 0 1
symversion=0.4
T 66100 53000 5 10 0 0 0 0 1
footprint=none
T 65500 53200 5 8 1 1 0 0 1
refdes=pon_reset#
T 65500 53200 5 10 0 0 0 0 1
device=OUTPUT
}
C 63300 54200 1 0 0 3.3V-plus-1.sym
N 63000 52500 64500 52500 4
N 63500 53000 63500 52500 4
N 63500 53900 63500 54200 4
T 58500 49000 9 10 1 0 0 0 1
"low-bat" voltage is calculated
T 58500 48700 9 10 1 0 0 0 1
according to the following formula:
T 58500 48400 9 10 1 0 0 0 1
Vlo = Vlbi*((R32+R33)/R33), where Vlbi is 1.2V
T 58500 48100 9 10 1 0 0 0 1
Here: Vlo = 1.2*139/39 = 4.3V
N 64500 53000 64500 52500 4
N 64500 53000 65500 53000 4