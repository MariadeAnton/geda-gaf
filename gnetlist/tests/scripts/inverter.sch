v 20130414 2
C 0 0 0 0 0 title-B.sym
C 12600 5800 1 0 0 transformer-3.sym
{
T 13225 7350 5 10 1 1 0 0 1
refdes=T?
T 12600 7400 5 10 0 0 0 0 1
device=TRANSFORMER
}
C 14600 9175 1 0 0 12V-plus-1.sym
C 925 10050 1 0 0 12V-plus-1.sym
C 14200 4100 1 0 0 receptacle-110V-15A-1.sym
{
T 14350 6400 5 10 0 0 0 0 1
symversion=1.0
T 15600 5200 5 10 0 0 0 0 1
device=darlington, NPN
T 15600 4800 5 10 0 0 0 0 1
footprint=TO92
T 14675 5445 5 10 1 1 0 0 1
refdes=REC?
}
N 14100 7100 14225 7100 4
C 13300 8275 1 0 0 fuse-1.sym
{
T 13500 8675 5 10 0 0 0 0 1
device=FUSE
T 13600 8675 5 10 1 1 180 0 1
refdes=F?
T 13500 8875 5 10 0 0 0 0 1
symversion=0.1
T 13725 8550 5 10 1 1 0 0 1
value=50A
}
C 11400 4125 1 0 0 IRF1010N-1.sym
{
T 12750 4250 5 10 0 1 0 0 1
device=IRF1010N
T 12000 4625 5 10 0 0 0 0 1
footprint=TO-220AB
T 12100 4525 5 10 1 1 0 0 1
refdes=Q?
}
C 11400 8475 1 0 0 IRF1010N-1.sym
{
T 10975 8375 5 10 0 1 0 0 1
device=IRF1010N
T 12000 8975 5 10 0 0 0 0 1
footprint=TO-220AB
T 12050 8975 5 10 1 1 0 0 1
refdes=Q?
}
N 11800 9575 12600 9575 4
N 12600 9575 12600 7100 4
N 11800 3625 12600 3625 4
N 12600 3625 12600 5900 4
N 11900 9275 11900 9575 4
N 11900 8475 11900 4925 4
N 12600 6500 12300 6500 4
N 12300 6500 12300 8375 4
N 13300 8375 12300 8375 4
N 14800 9175 14800 8375 4
N 14800 8375 14200 8375 4
N 11900 6500 9800 6500 4
C 10200 8575 1 0 0 resistor-1.sym
{
T 10500 8975 5 10 0 0 0 0 1
device=RESISTOR
T 10250 8850 5 10 1 1 0 0 1
refdes=R?
T 10725 8850 5 10 1 1 0 0 1
value=100
}
C 8700 8575 1 0 0 resistor-1.sym
{
T 9000 8975 5 10 0 0 0 0 1
device=RESISTOR
T 8900 8875 5 10 1 1 0 0 1
refdes=R?
T 8900 8375 5 10 1 1 0 0 1
value=3.3k
}
C 8450 8775 1 180 0 resistor-1.sym
{
T 8150 8375 5 10 0 0 180 0 1
device=RESISTOR
T 8075 8975 5 10 1 1 180 0 1
refdes=R?
T 7825 8400 5 10 1 1 0 0 1
value=1k
}
C 10200 4225 1 0 0 resistor-1.sym
{
T 10500 4625 5 10 0 0 0 0 1
device=RESISTOR
T 10275 4500 5 10 1 1 0 0 1
refdes=R?
T 10750 4500 5 10 1 1 0 0 1
value=100
}
C 8700 4225 1 0 0 resistor-1.sym
{
T 9000 4625 5 10 0 0 0 0 1
device=RESISTOR
T 8700 4525 5 10 1 1 0 0 1
refdes=R?
T 9250 4525 5 10 1 1 0 0 1
value=3.3k
}
C 8425 4425 1 180 0 resistor-1.sym
{
T 8125 4025 5 10 0 0 180 0 1
device=RESISTOR
T 8075 4650 5 10 1 1 180 0 1
refdes=R?
T 7825 4050 5 10 1 1 0 0 1
value=1k
}
C 10000 6775 1 90 0 zener-1.sym
{
T 9400 7175 5 10 0 0 90 0 1
device=ZENER_DIODE
T 9700 7625 5 10 1 1 180 0 1
refdes=Z?
}
C 9600 6250 1 270 0 zener-1.sym
{
T 10200 5850 5 10 0 0 270 0 1
device=ZENER_DIODE
T 9250 5800 5 10 1 1 0 0 1
refdes=Z?
}
N 9800 6775 9800 6250 4
N 11100 8675 11400 8675 4
N 11100 4325 11400 4325 4
N 9600 8675 10200 8675 4
N 9600 4325 10200 4325 4
N 9800 7675 9800 8675 4
N 9800 5350 9800 4325 4
C 10900 9375 1 0 0 zener-1.sym
{
T 11300 9975 5 10 0 0 0 0 1
device=ZENER_DIODE
T 11100 9875 5 10 1 1 180 0 1
refdes=Z?
T 11600 9725 5 10 1 1 0 0 1
value=36V
}
N 10900 9575 10800 9575 4
N 9600 9575 9900 9575 4
N 9800 9575 9800 8675 4
C 10900 3425 1 0 0 zener-1.sym
{
T 11300 4025 5 10 0 0 0 0 1
device=ZENER_DIODE
T 11100 3925 5 10 1 1 180 0 1
refdes=Z?
T 11200 3200 5 10 1 1 0 0 1
value=36V
}
N 10900 3625 10800 3625 4
N 9600 3625 9900 3625 4
N 11900 4125 11900 3625 4
N 9800 3625 9800 4325 4
N 8450 8675 8700 8675 4
N 8425 4325 8700 4325 4
N 8700 3625 8550 3625 4
N 8550 3625 8550 4325 4
N 8700 9575 8575 9575 4
N 8575 9575 8575 8675 4
C 6100 8175 1 0 0 4001-1.sym
{
T 6700 9075 5 10 0 0 0 0 1
device=4001
T 6550 8600 5 10 1 1 0 0 1
refdes=U?D
T 6700 10475 5 10 0 0 0 0 1
footprint=DIP14
T 6100 8175 5 10 0 0 0 0 1
slot=4
}
C 6100 3825 1 0 0 4001-1.sym
{
T 6700 4725 5 10 0 0 0 0 1
device=4001
T 6550 4250 5 10 1 1 0 0 1
refdes=U?C
T 6700 6125 5 10 0 0 0 0 1
footprint=DIP14
T 6100 3825 5 10 0 0 0 0 1
slot=3
}
C 6100 5175 1 0 0 4001-1.sym
{
T 6700 6075 5 10 0 0 0 0 1
device=4001
T 6550 5625 5 10 1 1 0 0 1
refdes=U?B
T 6700 7475 5 10 0 0 0 0 1
footprint=DIP14
T 6100 5175 5 10 0 0 0 0 1
slot=2
}
C 6100 6875 1 0 0 4001-1.sym
{
T 6700 7775 5 10 0 0 0 0 1
device=4001
T 6575 7300 5 10 1 1 0 0 1
refdes=U?A
T 6700 9175 5 10 0 0 0 0 1
footprint=DIP14
T 6100 6875 5 10 0 0 0 0 1
slot=1
}
N 7500 6750 7500 7375 4
N 7400 7375 7500 7375 4
C 14200 2500 1 0 0 receptacle-110V-15A-1.sym
{
T 14350 4800 5 10 0 0 0 0 1
symversion=1.0
T 15600 3600 5 10 0 0 0 0 1
device=darlington, NPN
T 15600 3200 5 10 0 0 0 0 1
footprint=TO92
T 15450 2670 5 10 1 1 0 0 1
refdes=REC?
}
N 5725 4125 5725 5000 4
N 5725 4525 6100 4525 4
N 6100 4125 5725 4125 4
N 5725 5475 6100 5475 4
N 6100 5875 6000 5875 4
N 5725 7175 6100 7175 4
N 5725 5475 5725 7175 4
C 4450 8175 1 90 0 resistor-1.sym
{
T 4050 8475 5 10 0 0 90 0 1
device=RESISTOR
T 4750 8875 5 10 1 1 180 0 1
refdes=R?
T 4550 8475 5 10 1 1 0 0 1
value=22k
}
C 5375 5100 1 180 0 resistor-1.sym
{
T 5075 4700 5 10 0 0 180 0 1
device=RESISTOR
T 4850 4850 5 10 1 1 180 0 1
refdes=R?
T 5000 4700 5 10 1 1 0 0 1
value=22k
}
C 4450 3875 1 90 0 resistor-1.sym
{
T 4050 4175 5 10 0 0 90 0 1
device=RESISTOR
T 4175 4575 5 10 1 1 180 0 1
refdes=R?
T 3900 4200 5 10 1 1 0 0 1
value=22k
}
N 4350 8175 4350 4775 4
N 4550 7775 4350 7775 4
N 7500 6750 6000 6750 4
N 6000 5875 6000 8875 4
N 5550 7575 6100 7575 4
C 4550 7175 1 0 0 lf353-1.sym
{
T 5225 7775 5 10 0 0 0 0 1
device=LF353
T 5250 8525 5 10 0 0 0 0 1
footprint=DIP8
T 5150 7975 5 10 1 1 0 0 1
refdes=U?A
}
C 2750 6975 1 0 0 lf353-1.sym
{
T 3425 7575 5 10 0 0 0 0 1
device=LF353
T 3450 8325 5 10 0 0 0 0 1
footprint=DIP8
T 2950 7875 5 10 1 1 0 0 1
refdes=U?B
T 2750 6975 5 10 0 0 0 0 1
slot=2
}
C 1025 8675 1 270 0 resistor-1.sym
{
T 1425 8375 5 10 0 0 270 0 1
device=RESISTOR
T 925 8475 5 10 1 1 180 0 1
refdes=R?
T 625 8075 5 10 1 1 0 0 1
value=22k
}
C 2650 6075 1 90 0 resistor-1.sym
{
T 2250 6375 5 10 0 0 90 0 1
device=RESISTOR
T 2350 6675 5 10 1 1 180 0 1
refdes=R?
T 2050 6275 5 10 1 1 0 0 1
value=22k
}
N 4550 7375 3750 7375 4
C 2800 8625 1 0 0 capacitor-1.sym
{
T 3000 9325 5 10 0 0 0 0 1
device=CAPACITOR
T 3125 9150 5 10 1 1 180 0 1
refdes=C?
T 3000 9525 5 10 0 0 0 0 1
symversion=0.1
T 2775 8625 5 10 1 1 0 0 1
value=.1uF
}
C 1225 8975 1 90 0 resistor-1.sym
{
T 825 9275 5 10 0 0 90 0 1
device=RESISTOR
T 925 9675 5 10 1 1 180 0 1
refdes=R?
T 625 9275 5 10 1 1 0 0 1
value=39k
}
C 1400 8725 1 0 0 resistor-1.sym
{
T 1700 9125 5 10 0 0 0 0 1
device=RESISTOR
T 1700 9025 5 10 1 1 0 0 1
refdes=R?
T 1700 8525 5 10 1 1 0 0 1
value=47k
}
N 3900 7375 3900 8825 4
N 3900 8825 3700 8825 4
N 2800 8825 2300 8825 4
N 2550 6975 2550 8825 4
N 2750 7575 1125 7575 4
C 1025 6975 1 270 0 resistor-1.sym
{
T 1425 6675 5 10 0 0 270 0 1
device=RESISTOR
T 925 6775 5 10 1 1 180 0 1
refdes=R?
T 625 6375 5 10 1 1 0 0 1
value=22k
}
N 1125 6975 1125 7775 4
N 1125 8975 1125 8675 4
N 1400 8825 1125 8825 4
N 2750 7175 2550 7175 4
N 2550 6075 2550 5925 4
C 4475 5325 1 0 0 resistor-1.sym
{
T 4775 5725 5 10 0 0 0 0 1
device=RESISTOR
T 4875 5775 5 10 1 1 180 0 1
refdes=R?
T 4950 5650 5 10 1 1 0 0 1
value=1k
}
N 3150 5425 4475 5425 4
N 4475 5000 4350 5000 4
N 1125 9875 1125 10050 4
C 1025 3100 1 0 0 gnd-1.sym
N 7400 8675 7550 8675 4
N 6000 8875 6100 8875 4
N 6100 8475 6000 8475 4
N 7400 4325 7525 4325 4
N 5375 5000 7500 5000 4
N 7500 5000 7500 5675 4
N 7400 5675 7500 5675 4
N 9800 6500 5725 6500 4
N 2550 3750 2550 4925 4
N 5500 5000 5500 5425 4
N 5375 5425 5500 5425 4
N 11850 8000 12600 8000 4
N 11825 5125 12600 5125 4
N 10850 5125 10850 8000 4
N 5725 8000 9900 8000 4
C 8800 6775 1 90 0 zener-1.sym
{
T 8200 7175 5 10 0 0 90 0 1
device=ZENER_DIODE
T 8525 7625 5 10 1 1 180 0 1
refdes=Z?
T 8525 6600 5 10 1 1 90 0 1
value=13V
}
N 8600 8000 8600 7675 4
N 8600 6250 8600 6775 4
N 4350 9075 4350 9325 4
N 4350 9325 5725 9325 4
N 5050 9325 5050 7975 4
N 8000 7675 8000 8000 4
N 5725 9325 5725 8000 4
N 8000 6775 8000 6500 4
N 9200 7650 9200 8000 4
N 9200 6750 9200 6500 4
N 9675 5125 10925 5125 4
C 9900 7900 1 0 0 resistor-1.sym
{
T 10200 8300 5 10 0 0 0 0 1
device=RESISTOR
T 10200 8200 5 10 1 1 0 0 1
refdes=R?
T 10200 7700 5 10 1 1 0 0 1
value=2.7k
}
N 10800 8000 10950 8000 4
N 1125 3400 1125 6075 4
N 4350 3875 4350 3750 4
N 4350 3750 1125 3750 4
N 5725 6500 3600 6500 4
N 3600 6500 3600 3750 4
N 5050 7175 5050 6500 4
C 8700 5350 1 90 0 led-2.sym
{
T 8000 5525 5 10 1 1 0 0 1
refdes=LED?
T 8750 5700 5 10 0 0 0 0 1
device=LED
}
C 9675 5225 1 180 0 resistor-1.sym
{
T 9225 4975 5 10 0 0 0 5 1
device=RESISTOR
T 9225 5275 5 10 1 1 0 3 1
refdes=R?
T 9225 4975 5 10 1 1 0 5 1
value=330
}
N 8775 5125 8600 5125 4
N 8600 5125 8600 5350 4
N 15400 3200 15400 7100 4
N 14800 4100 14800 3875 4
N 14800 3875 13900 3875 4
N 13900 3875 13900 2375 4
N 13900 2375 14800 2375 4
N 14800 2500 14800 2375 4
C 3150 4925 1 0 1 2N3904_npn_40V_1W_SOT-223-1.sym
{
T 3150 6775 5 10 0 0 0 6 1
symversion=1.0
T 2250 5625 5 10 1 1 0 6 1
device=2N3904
T 2250 5425 5 10 1 1 0 6 1
refdes=Q?
T 2250 5225 5 10 0 1 0 6 1
value=40V, 1W
T 2250 5025 5 10 0 0 0 6 1
footprint=SOT223
}
C 8700 9175 1 0 0 1N4148-1.sym
{
T 9000 9975 5 10 0 0 0 0 1
footprint=D300-op
T 9100 9875 5 10 1 1 0 4 1
refdes=D?
T 9000 10575 5 10 0 0 0 0 1
device=1N4148
T 9200 9275 5 10 1 1 0 4 1
value=1N4148
}
C 9900 9175 1 0 0 1N4148-1.sym
{
T 10200 9975 5 10 0 0 0 0 1
footprint=D300-op
T 10300 9875 5 10 1 1 0 4 1
refdes=D?
T 10200 10575 5 10 0 0 0 0 1
device=1N4148
T 10400 9275 5 10 1 1 0 4 1
value=1N4148
}
C 8200 6775 1 90 0 capacitor-1.sym
{
T 7500 6975 5 10 0 0 90 0 1
device=CAPACITOR
T 7925 7000 5 10 1 1 180 0 1
refdes=C?
T 7300 6975 5 10 0 0 90 0 1
symversion=0.1
T 8100 7375 5 10 0 0 90 0 1
footprint=unknown
}
C 9400 6750 1 90 0 capacitor-1.sym
{
T 8700 6950 5 10 0 0 90 0 1
device=CAPACITOR
T 9075 7000 5 10 1 1 180 0 1
refdes=C?
T 8500 6950 5 10 0 0 90 0 1
symversion=0.1
T 9300 7350 5 10 0 0 90 0 1
footprint=unknown
}
C 14225 6800 1 0 0 circuit-breaker_1p-1.sym
{
T 14225 7700 5 10 0 0 0 0 1
device=ZESTYK
T 14525 6825 5 10 1 1 180 8 1
value=XX
T 14925 7375 5 10 1 1 180 6 1
refdes=CB?
}
C 10925 4725 1 0 0 1N4148-1.sym
{
T 11225 5525 5 10 0 0 0 0 1
footprint=D300-op
T 11325 5425 5 10 1 1 0 4 1
refdes=D?
T 11225 6125 5 10 0 0 0 0 1
device=1N4148
T 11350 5625 5 10 1 1 0 4 1
value=1N4148
}
C 9900 3225 1 0 0 1N4148-1.sym
{
T 10200 4025 5 10 0 0 0 0 1
footprint=D300-op
T 10300 3925 5 10 1 1 0 4 1
refdes=D?
T 10200 4625 5 10 0 0 0 0 1
device=1N4148
T 10400 3325 5 10 1 1 0 4 1
value=1N4148
}
C 8700 3225 1 0 0 1N4148-1.sym
{
T 9000 4025 5 10 0 0 0 0 1
footprint=D300-op
T 9100 3925 5 10 1 1 0 4 1
refdes=D?
T 9000 4625 5 10 0 0 0 0 1
device=1N4148
T 9200 3325 5 10 1 1 0 4 1
value=1N4148
}
C 10950 7600 1 0 0 1N4148-1.sym
{
T 11250 8400 5 10 0 0 0 0 1
footprint=D300-op
T 11350 8300 5 10 1 1 0 4 1
refdes=D?
T 11250 9000 5 10 0 0 0 0 1
device=1N4148
T 11450 7700 5 10 1 1 0 4 1
value=1N4148
}
N 14100 5900 14200 5900 4
N 14200 3200 14200 5900 4
N 15025 7100 15400 7100 4