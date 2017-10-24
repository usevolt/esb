EESchema Schematic File Version 2
LIBS:esb-rescue
LIBS:kemet
LIBS:usevolt
LIBS:wurth_elektronik_LED
LIBS:power
LIBS:elt21300_kicad
LIBS:yageo
LIBS:esb-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 9
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 7450 3450 2    60   Input ~ 0
SENSOR1
Text HLabel 5850 3350 2    60   Output ~ 0
VOUT1
Text HLabel 7450 3850 2    60   Input ~ 0
SENSOR2
Text HLabel 5850 3750 2    60   Output ~ 0
VOUT2
$Comp
L +3V3 #PWR029
U 1 1 59EDDEDF
P 4050 3350
F 0 "#PWR029" H 4050 3200 50  0001 C CNN
F 1 "+3V3" H 4065 3523 50  0000 C CNN
F 2 "" H 4050 3350 50  0000 C CNN
F 3 "" H 4050 3350 50  0000 C CNN
	1    4050 3350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR030
U 1 1 59EDDEF8
P 4050 4000
F 0 "#PWR030" H 4050 3750 50  0001 C CNN
F 1 "GND" H 4055 3827 50  0000 C CNN
F 2 "" H 4050 4000 50  0000 C CNN
F 3 "" H 4050 4000 50  0000 C CNN
	1    4050 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 3800 4050 4000
Wire Wire Line
	4050 3850 4300 3850
$Comp
L 100nF C701
U 1 1 59EDDF13
P 4050 3700
F 0 "C701" V 4142 3627 45  0000 R CNN
F 1 "100nF" V 4058 3627 45  0000 R CNN
F 2 "kemet:0603" H 4080 3850 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2062872.pdf?_ga=1.250530118.1948984246.1460023966" H 3954 3709 60  0001 C CNN
F 4 "1288255" H 4354 4109 60  0001 C CNN "1st Source Part Number"
F 5 "C0603C104K5RACTU" H 4154 3909 60  0001 C CNN "Manufacturer Part Number"
F 6 "Farnell" H 4254 4009 60  0001 C CNN "1st Source"
F 7 "KEMET" H 4054 3809 60  0001 C CNN "Manufacturer"
	1    4050 3700
	0    -1   -1   0   
$EndComp
$Comp
L OPAMP_MCP6002 U701
U 1 1 59EDF729
P 4550 3550
F 0 "U701" H 4675 3997 60  0000 C CNN
F 1 "OPAMP_MCP6002" H 4675 3891 60  0000 C CNN
F 2 "usevolt:SOIC8" H 4550 4300 60  0001 C CNN
F 3 "http://www.farnell.com/datasheets/524878.pdf?_ga=2.16653489.687633841.1508741885-1862443086.1504075057" H 4550 4300 60  0001 C CNN
F 4 "1292246" H 4950 4700 60  0001 C CNN "1st Source Part Number"
F 5 "Digikey" H 5050 4800 60  0001 C CNN "2nd Source"
F 6 "Farnell" H 4850 4600 60  0001 C CNN "1st Source"
F 7 "MCP6002-I/SN" H 4750 4500 60  0001 C CNN "Manufacturer Part Number"
F 8 "MCP6002-I/SN-ND" H 5150 4900 60  0001 C CNN "2nd Source Part Number"
F 9 "MICROCHIP" H 4650 4400 60  0001 C CNN "Manufacturer"
	1    4550 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 3350 4050 3500
Wire Wire Line
	4300 3450 4050 3450
Connection ~ 4050 3450
Connection ~ 4050 3850
Wire Wire Line
	5050 3350 5850 3350
Wire Wire Line
	5050 3750 5850 3750
$Comp
L +3V3 #PWR031
U 1 1 59EDF8B8
P 7000 3000
F 0 "#PWR031" H 7000 2850 50  0001 C CNN
F 1 "+3V3" H 7015 3173 50  0000 C CNN
F 2 "" H 7000 3000 50  0000 C CNN
F 3 "" H 7000 3000 50  0000 C CNN
	1    7000 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 3450 6650 3450
Wire Wire Line
	5050 3850 6650 3850
$Comp
L 10K R705
U 1 1 59EDFACF
P 6750 3450
F 0 "R705" H 6800 3634 45  0000 C CNN
F 1 "10K" H 6800 3550 45  0000 C CNN
F 2 "yageo:0603" H 6780 3600 20  0001 C CNN
F 3 "" H 6719 3489 60  0001 C CNN
F 4 "" H 7119 3889 60  0001 C CNN "1st Source Part Number"
F 5 "Digikey" H 7019 3789 60  0001 C CNN "1st Source"
F 6 "RC0603FR-0710KL" H 6919 3689 60  0001 C CNN "Manufacturer Part Number"
F 7 "YAGEO" H 6819 3589 60  0001 C CNN "Manufacturer"
	1    6750 3450
	1    0    0    -1  
$EndComp
$Comp
L 10K R706
U 1 1 59EDFB0E
P 6750 3850
F 0 "R706" H 6800 4034 45  0000 C CNN
F 1 "10K" H 6800 3950 45  0000 C CNN
F 2 "yageo:0603" H 6780 4000 20  0001 C CNN
F 3 "" H 6719 3889 60  0001 C CNN
F 4 "" H 7119 4289 60  0001 C CNN "1st Source Part Number"
F 5 "Digikey" H 7019 4189 60  0001 C CNN "1st Source"
F 6 "RC0603FR-0710KL" H 6919 4089 60  0001 C CNN "Manufacturer Part Number"
F 7 "YAGEO" H 6819 3989 60  0001 C CNN "Manufacturer"
	1    6750 3850
	1    0    0    -1  
$EndComp
$Comp
L 10K R703
U 1 1 59EE04D0
P 5500 3550
F 0 "R703" H 5550 3734 45  0000 C CNN
F 1 "10K" H 5550 3650 45  0000 C CNN
F 2 "yageo:0603" H 5530 3700 20  0001 C CNN
F 3 "" H 5469 3589 60  0001 C CNN
F 4 "" H 5869 3989 60  0001 C CNN "1st Source Part Number"
F 5 "Digikey" H 5769 3889 60  0001 C CNN "1st Source"
F 6 "RC0603FR-0710KL" H 5669 3789 60  0001 C CNN "Manufacturer Part Number"
F 7 "YAGEO" H 5569 3689 60  0001 C CNN "Manufacturer"
	1    5500 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 3350 5750 3550
Wire Wire Line
	5750 3550 5700 3550
Connection ~ 5750 3350
Wire Wire Line
	5050 3550 5400 3550
$Comp
L 1K R701
U 1 1 59EE059A
P 5300 3650
F 0 "R701" H 5350 3834 45  0000 C CNN
F 1 "1K" H 5350 3750 45  0000 C CNN
F 2 "yageo:0603" H 5330 3800 20  0001 C CNN
F 3 "" H 5269 3689 60  0001 C CNN
F 4 "" H 5669 4089 60  0001 C CNN "1st Source Part Number"
F 5 "Digikey" H 5569 3989 60  0001 C CNN "1st Source"
F 6 "RC0603FR-071KL" H 5469 3889 60  0001 C CNN "Manufacturer Part Number"
F 7 "YAGEO" H 5369 3789 60  0001 C CNN "Manufacturer"
	1    5300 3650
	1    0    0    -1  
$EndComp
Connection ~ 5150 3550
Wire Wire Line
	5200 3650 5150 3650
Wire Wire Line
	5150 3650 5150 3550
$Comp
L 10K R704
U 1 1 59EE0699
P 5500 3950
F 0 "R704" H 5550 4134 45  0000 C CNN
F 1 "10K" H 5550 4050 45  0000 C CNN
F 2 "yageo:0603" H 5530 4100 20  0001 C CNN
F 3 "" H 5469 3989 60  0001 C CNN
F 4 "" H 5869 4389 60  0001 C CNN "1st Source Part Number"
F 5 "Digikey" H 5769 4289 60  0001 C CNN "1st Source"
F 6 "RC0603FR-0710KL" H 5669 4189 60  0001 C CNN "Manufacturer Part Number"
F 7 "YAGEO" H 5569 4089 60  0001 C CNN "Manufacturer"
	1    5500 3950
	1    0    0    -1  
$EndComp
$Comp
L 1K R702
U 1 1 59EE06D8
P 5300 4050
F 0 "R702" H 5350 4234 45  0000 C CNN
F 1 "1K" H 5350 4150 45  0000 C CNN
F 2 "yageo:0603" H 5330 4200 20  0001 C CNN
F 3 "" H 5269 4089 60  0001 C CNN
F 4 "" H 5669 4489 60  0001 C CNN "1st Source Part Number"
F 5 "Digikey" H 5569 4389 60  0001 C CNN "1st Source"
F 6 "RC0603FR-071KL" H 5469 4289 60  0001 C CNN "Manufacturer Part Number"
F 7 "YAGEO" H 5369 4189 60  0001 C CNN "Manufacturer"
	1    5300 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 3950 5750 3950
Wire Wire Line
	5750 3950 5750 3750
Connection ~ 5750 3750
Wire Wire Line
	5050 3950 5400 3950
Wire Wire Line
	5200 4050 5150 4050
Wire Wire Line
	5150 4050 5150 3950
Connection ~ 5150 3950
$Comp
L GND #PWR032
U 1 1 59EE07A1
P 6250 4100
F 0 "#PWR032" H 6250 3850 50  0001 C CNN
F 1 "GND" H 6255 3927 50  0000 C CNN
F 2 "" H 6250 4100 50  0000 C CNN
F 3 "" H 6250 4100 50  0000 C CNN
	1    6250 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 3650 6250 4100
Wire Wire Line
	6250 4050 5500 4050
Wire Wire Line
	6250 3650 5500 3650
Connection ~ 6250 4050
Wire Wire Line
	6950 3450 7450 3450
Wire Wire Line
	6950 3850 7450 3850
$Comp
L 1K5 R708
U 1 1 59EE17CE
P 7100 3700
F 0 "R708" H 7150 3884 45  0000 C CNN
F 1 "1K5" H 7150 3800 45  0000 C CNN
F 2 "yageo:0603" H 7130 3850 20  0001 C CNN
F 3 "" H 7069 3739 60  0001 C CNN
F 4 "" H 7469 4139 60  0001 C CNN "1st Source Part Number"
F 5 "Digikey" H 7369 4039 60  0001 C CNN "1st Source"
F 6 "RC0603FR-071K5L" H 7269 3939 60  0001 C CNN "Manufacturer Part Number"
F 7 "YAGEO" H 7169 3839 60  0001 C CNN "Manufacturer"
	1    7100 3700
	1    0    0    -1  
$EndComp
$Comp
L 1K5 R707
U 1 1 59EE1849
P 7100 3300
F 0 "R707" H 7150 3484 45  0000 C CNN
F 1 "1K5" H 7150 3400 45  0000 C CNN
F 2 "yageo:0603" H 7130 3450 20  0001 C CNN
F 3 "" H 7069 3339 60  0001 C CNN
F 4 "" H 7469 3739 60  0001 C CNN "1st Source Part Number"
F 5 "Digikey" H 7369 3639 60  0001 C CNN "1st Source"
F 6 "RC0603FR-071K5L" H 7269 3539 60  0001 C CNN "Manufacturer Part Number"
F 7 "YAGEO" H 7169 3439 60  0001 C CNN "Manufacturer"
	1    7100 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 3300 7300 3450
Connection ~ 7300 3450
Wire Wire Line
	7300 3700 7300 3850
Connection ~ 7300 3850
Wire Wire Line
	7000 3000 7000 3700
Connection ~ 7000 3300
Wire Wire Line
	7000 3100 7000 3100
$EndSCHEMATC
