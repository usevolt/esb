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
Sheet 8 9
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L 1K R801
U 1 1 59EDA346
P 5400 3850
AR Path="/59EDA240/59EDA346" Ref="R801"  Part="1" 
AR Path="/59EDB7AA/59EDA346" Ref="R901"  Part="1" 
F 0 "R901" H 5450 4034 45  0000 C CNN
F 1 "1K" H 5450 3950 45  0000 C CNN
F 2 "yageo:0603" H 5430 4000 20  0001 C CNN
F 3 "" H 5369 3889 60  0001 C CNN
F 4 "" H 5769 4289 60  0001 C CNN "1st Source Part Number"
F 5 "Digikey" H 5669 4189 60  0001 C CNN "1st Source"
F 6 "RC0603FR-071KL" H 5569 4089 60  0001 C CNN "Manufacturer Part Number"
F 7 "YAGEO" H 5469 3989 60  0001 C CNN "Manufacturer"
	1    5400 3850
	1    0    0    -1  
$EndComp
Text HLabel 6000 3850 2    60   Input ~ 0
VIN
Wire Wire Line
	6000 3850 5600 3850
Text HLabel 5000 3850 0    60   Output ~ 0
VOUT
Wire Wire Line
	5000 3850 5300 3850
Connection ~ 5150 3850
Wire Wire Line
	5150 3750 5150 3850
Wire Wire Line
	5150 3300 5150 3450
$Comp
L +3V3 #PWR801
U 1 1 59EDA7C2
P 5150 3300
AR Path="/59EDA240/59EDA7C2" Ref="#PWR801"  Part="1" 
AR Path="/59EDB7AA/59EDA7C2" Ref="#PWR901"  Part="1" 
F 0 "#PWR901" H 5150 3150 50  0001 C CNN
F 1 "+3V3" H 5165 3473 50  0000 C CNN
F 2 "" H 5150 3300 50  0000 C CNN
F 3 "" H 5150 3300 50  0000 C CNN
	1    5150 3300
	1    0    0    -1  
$EndComp
$Comp
L DIODE_RS07J-GS18 D801
U 1 1 59EDA742
P 5150 3550
AR Path="/59EDA240/59EDA742" Ref="D801"  Part="1" 
AR Path="/59EDB7AA/59EDA742" Ref="D901"  Part="1" 
F 0 "D901" V 5100 3452 45  0000 R CNN
F 1 "DIODE_RS07J-GS18" H 5170 3490 45  0001 L BNN
F 2 "usevolt:DO219AB" H 5180 3700 20  0001 C CNN
F 3 "http://www.vishay.com/docs/85742/rs07b.pdf" H 5170 3570 60  0001 C CNN
F 4 "1864882" H 5770 4170 60  0001 C CNN "2nd Source Part Number"
F 5 "Digi-Key" H 5470 3870 60  0001 C CNN "1st Source"
F 6 "Farnell" H 5670 4070 60  0001 C CNN "2nd Source"
F 7 "RS07J-GS18" H 5370 3770 60  0001 C CNN "Manufacturer Part Number"
F 8 "RS07J-GS18-ND" H 5570 3970 60  0001 C CNN "1st Source Part Number"
F 9 "Vishay Semiconductor Diodes Division" H 5270 3670 60  0001 C CNN "Manufacturer"
	1    5150 3550
	0    -1   -1   0   
$EndComp
$EndSCHEMATC
