v 20151204 2
C 52500 58200 0 0 0 title-A4.sym
{
T 57400 59000 9 10 1 1 0 0 1
sheet-title=LIGHTNING DETECTOR
T 61000 58300 9 12 1 1 0 0 1
sheet-drawnby=techman@dingoblue.net.au
T 61000 58600 9 10 1 0 0 0 1
0.1.5
T 57200 58600 9 10 1 1 0 0 1
sheet-filename=lightning.sch
T 56600 59400 8 10 1 1 0 0 1
sheet-drawncomment=PRELIMINARY ENGINEERING
T 57400 58300 9 10 1 1 0 0 1
sheet-number=1
T 58900 58300 9 10 1 1 0 0 1
sheet-pagetot=1
}
C 53300 60300 1 90 0 inductor-1.sym
{
T 53000 60800 5 10 1 1 0 6 1
refdes=L1
T 53000 60600 5 10 1 1 0 6 1
value=10mH
T 52750 60400 5 10 0 1 0 0 1
symversion=0.2
T 52800 60800 5 10 0 1 90 0 1
footprint=AXIAL_LAY-800
}
C 53300 63100 1 90 0 inductor-1.sym
{
T 53300 63800 5 10 1 1 0 0 1
refdes=L2
T 53300 63600 5 10 1 1 0 0 1
value=10mH
T 53300 63200 5 10 0 1 0 0 1
symversion=0.2
T 53000 62700 5 10 0 1 90 0 1
footprint=AXIAL_LAY-800
}
C 58300 62100 1 270 0 capacitor-2.sym
{
T 58300 61800 5 10 1 1 0 6 1
refdes=C4
T 58300 61400 5 10 1 1 0 6 1
value=100uF
T 58600 61100 5 10 0 1 0 0 1
symversion=0.1
T 58300 61200 5 10 0 1 0 6 1
footprint=ACY-600P
}
C 55100 62400 1 90 0 resistor-1.sym
{
T 54800 62900 5 10 1 1 0 6 1
refdes=R1
T 54800 62700 5 10 1 1 0 6 1
value=180k
T 54800 63100 5 10 0 1 0 6 1
device=RESISTOR
T 54800 62500 5 10 0 1 0 6 1
footprint=AXIAL_LAY-500
}
C 55700 64800 1 270 0 resistor-1.sym
{
T 55600 64400 5 10 1 1 0 6 1
refdes=R2
T 55600 64200 5 10 1 1 0 6 1
value=3.9k
T 55600 64600 5 10 0 1 0 6 1
device=RESISTOR
T 55600 64000 5 10 0 1 0 6 1
footprint=AXIAL_LAY-500
}
C 62100 64950 1 90 0 resistor-1.sym
{
T 62200 65500 5 10 1 1 0 0 1
refdes=R7
T 62200 65300 5 10 1 1 0 0 1
value=47R
T 62200 65700 5 10 0 1 0 0 1
device=RESISTOR
T 62200 65100 5 10 0 1 0 0 1
footprint=AXIAL_LAY-500
}
C 57200 61600 1 90 0 resistor-1.sym
{
T 56900 62100 5 10 1 1 0 6 1
refdes=R3
T 56900 61900 5 10 1 1 0 6 1
value=22k
T 56900 62300 5 10 0 1 0 6 1
device=RESISTOR
T 56900 61700 5 10 0 1 0 6 1
footprint=R-025
}
C 58700 62400 1 0 0 resistor-1.sym
{
T 58900 62900 5 10 1 1 0 0 1
refdes=R5
T 58900 62700 5 10 1 1 0 0 1
value=2.2K
T 58900 63100 5 10 0 1 0 0 1
device=RESISTOR
T 58600 63300 5 10 0 1 0 0 1
footprint=AXIAL_LAY-500
}
C 60800 60000 1 90 0 resistor-1.sym
{
T 60500 60500 5 10 1 1 0 6 1
refdes=R6
T 60500 60300 5 10 1 1 0 6 1
value=2.7k
T 60500 60700 5 10 0 1 0 6 1
device=RESISTOR
T 60500 60100 5 10 0 1 0 6 1
footprint=AXIAL_LAY-500
}
C 57200 63500 1 90 0 resistor-variable-1.sym
{
T 57300 64000 5 10 1 1 0 0 1
refdes=R4
T 57300 63800 5 10 1 1 0 0 1
value=20k
T 56300 64900 5 10 0 1 0 0 1
footprint=POT__BOURNS_3386P
T 56300 65100 5 10 0 1 0 0 1
description=POT,20K OHM,THUMBWHEEL,CERM,ST
}
N 53200 63100 53200 61200 4
N 53200 60300 53200 60000 4
N 53700 61200 53700 61600 4
N 53700 60300 53700 60000 4
N 53200 61600 53900 61600 4
N 54800 61600 55100 61600 4
N 55800 63900 55800 62100 4
N 53200 64700 53200 64000 4
N 62500 60000 53200 60000 4
N 55800 61100 55800 60000 4
N 55800 60000 60700 60000 4
N 55000 63500 55000 63300 4
N 55000 62400 55000 61600 4
N 62200 60500 62200 60000 4
N 62500 66000 55800 66000 4
N 55800 66000 55800 64800 4
N 62000 65850 62000 66000 4
N 62000 63000 62000 64950 4
N 62000 63000 62400 63000 4
N 55000 63500 55800 63500 4
N 61500 61000 57100 61000 4
N 57100 61000 57100 61600 4
N 60700 60900 60700 61000 4
N 60700 62000 60700 61000 4
N 60700 63000 60700 64800 4
N 55800 63000 56000 63000 4
N 57100 62500 57100 63500 4
N 56400 64000 56400 64800 4
N 56400 64800 62000 64800 4
N 58700 62500 58000 62500 4
N 62400 62000 62200 62000 4
N 62200 62000 62200 61500 4
N 59800 62500 59800 62100 4
N 59800 61200 59800 61000 4
N 58000 63650 58500 63650 4
N 58500 63650 58500 62100 4
N 58500 61200 58500 61000 4
C 61100 64100 1 270 0 capacitor-2.sym
{
T 61550 63700 5 10 1 1 0 0 1
refdes=C6
T 61550 63500 5 10 1 1 0 0 1
value=1uF
T 61400 63100 5 10 0 1 0 0 1
symversion=0.1
T 61050 63200 5 10 0 1 90 0 1
footprint=ACY-600P
}
N 61300 64100 61300 66000 4
N 61300 63200 61300 60000 4
N 59600 62500 60000 62500 4
N 57100 63000 57300 63000 4
N 57100 64400 57100 64800 4
N 58000 64700 58000 64800 4
N 58000 63500 58000 63800 4
C 57800 64700 1 270 0 diode-1.sym
{
T 58900 64500 5 10 0 1 0 0 1
symversion=0.5
T 58300 64300 5 10 1 1 0 0 1
refdes=D1
T 58300 64100 5 10 1 1 0 0 1
value=1N914
T 58300 64500 5 10 0 1 0 0 1
device=DIODE
T 58300 63900 5 10 0 1 0 0 1
footprint=AXIAL_LAY-500
}
C 55100 61100 1 0 0 2N4401.sym
{
T 56100 61300 5 10 1 1 0 0 1
refdes=Q1
T 56100 61100 5 10 1 1 0 0 1
value=2N4401
}
C 57300 63500 1 180 1 2N4403.sym
{
T 57300 62500 5 10 1 1 0 0 1
refdes=Q2
T 57300 62300 5 10 1 1 0 0 1
value=2N4403
}
C 60000 62000 1 0 0 2N4401.sym
{
T 60000 63300 5 10 1 1 0 0 1
refdes=Q3
T 59800 63000 5 10 1 1 0 0 1
value=2N4401
}
C 61500 60500 1 0 0 2N4401.sym
{
T 62600 61100 5 10 1 1 0 0 1
refdes=Q4
T 62500 60900 5 10 1 1 0 0 1
value=2N4401
}
N 56400 64000 56600 64000 4
N 56900 63000 57100 63000 4
C 53500 61200 1 270 0 capacitor-1.sym
{
T 54000 61000 5 10 0 1 0 0 1
device=CAPACITOR
T 54000 60800 5 10 1 1 0 0 1
refdes=C1
T 54000 60200 5 10 0 1 0 0 1
symversion=0.1
T 54000 60400 5 10 0 1 0 0 1
footprint=ACY-600
T 54000 60600 5 10 1 1 0 0 1
value=680pf
}
C 53900 61400 1 0 0 capacitor-1.sym
{
T 53900 62000 5 10 0 1 0 0 1
device=CAPACITOR
T 54000 61800 5 10 1 1 0 0 1
refdes=C2
T 54500 61800 5 10 0 1 0 0 1
symversion=0.1
T 53900 62200 5 10 0 1 0 0 1
footprint=ACY-600
T 54200 61200 5 10 1 1 0 0 1
value=0.01uF
}
C 56000 62800 1 0 0 capacitor-1.sym
{
T 56100 63400 5 10 0 1 0 0 1
device=CAPACITOR
T 56100 63200 5 10 1 1 0 0 1
refdes=C3
T 56600 63200 5 10 0 1 0 0 1
symversion=0.1
T 56100 63600 5 10 0 1 0 0 1
footprint=ACY-600
T 56200 62600 5 10 1 1 0 0 1
value=0.01uF
}
C 59600 62100 1 270 0 capacitor-1.sym
{
T 60100 61900 5 10 0 1 0 0 1
device=CAPACITOR
T 60100 61700 5 10 1 1 0 0 1
refdes=C5
T 60100 61100 5 10 0 1 0 0 1
symversion=0.1
T 60100 61300 5 10 0 1 0 0 1
footprint=ACY-600
T 60100 61500 5 10 1 1 0 0 1
value=5nF
}
C 62400 62900 1 0 0 out-1.sym
{
T 62500 63300 5 10 0 0 0 0 1
symversion=0.4
T 62850 62850 5 10 0 1 0 6 1
footprint=PAD150
T 63100 63000 5 8 1 1 0 0 1
refdes=lamp(1)
T 63000 63150 5 10 0 1 0 0 1
device=OUTPUT
T 63100 62850 5 10 0 2 0 0 1
nobom=1
}
C 62400 61900 1 0 0 out-1.sym
{
T 63100 62000 5 8 1 1 0 0 1
refdes=lamp(2)
T 62450 62300 5 10 0 0 0 0 1
symversion=0.4
T 62850 61850 5 10 0 1 0 6 1
footprint=PAD150
T 63000 62150 5 10 0 1 0 0 1
device=OUTPUT
T 63100 61850 5 10 0 2 0 0 1
nobom=1
}
C 63100 66100 1 180 0 in-1.sym
{
T 63050 65750 5 10 0 1 0 0 1
symversion=0.4
T 63300 65700 5 10 0 1 0 0 1
footprint=PAD150
T 63300 66000 5 10 1 1 0 0 1
refdes=bat(+3v)
T 62500 65750 5 10 0 1 0 0 1
device=INPUT
T 63300 65850 5 10 0 2 0 0 1
nobom=1
}
C 63100 60100 1 180 0 in-1.sym
{
T 63700 60300 5 10 0 0 180 0 1
symversion=0.4
T 63200 59650 5 10 0 1 0 0 1
footprint=PAD150
T 63200 59950 5 10 1 1 0 0 1
refdes=bat(0v)
T 63000 59850 5 10 0 1 180 0 1
device=INPUT
T 63200 59800 5 10 0 2 0 0 1
nobom=1
}
C 53100 65300 1 270 0 in-1.sym
{
T 53350 64550 5 10 0 1 0 0 1
symversion=0.4
T 53400 65300 5 10 0 1 0 0 1
footprint=PAD150
T 53100 65500 5 10 1 1 0 0 1
refdes=A1
T 53350 65200 5 10 0 1 270 0 1
device=INPUT
T 53400 65500 5 10 0 2 0 0 1
nobom=1
}