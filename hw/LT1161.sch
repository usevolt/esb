EESchema Schematic File Version 2
LIBS:msb-rescue
LIBS:kemet
LIBS:multicomp
LIBS:usevolt
LIBS:wurth_elektronik_LED
LIBS:power
LIBS:elt21300_kicad
LIBS:msb-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 16
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 4650 2750 950  350 
U 58BE8E4D
F0 "sheet58BE8E45" 60
F1 "currentSense.sch" 60
F2 "SENSE+" I R 5600 2850 60 
F3 "SENSE-" I R 5600 3000 60 
F4 "VOUT" O L 4650 2850 60 
$EndSheet
$Comp
L GND #PWR501
U 1 1 58BE8E62
P 2100 4700
AR Path="/58BE52C9/58BE8E62" Ref="#PWR501"  Part="1" 
AR Path="/58C7B0A8/58BE8E62" Ref="#PWR1001"  Part="1" 
F 0 "#PWR1001" H 2100 4450 50  0001 C CNN
F 1 "GND" H 2105 4527 50  0000 C CNN
F 2 "" H 2100 4700 50  0000 C CNN
F 3 "" H 2100 4700 50  0000 C CNN
	1    2100 4700
	1    0    0    -1  
$EndComp
$Comp
L MOSFET_DRIVER_LT1161 U501
U 1 1 58BE8E74
P 2750 4000
AR Path="/58BE52C9/58BE8E74" Ref="U501"  Part="1" 
AR Path="/58C7B0A8/58BE8E74" Ref="U1001"  Part="1" 
F 0 "U1001" H 2750 4797 60  0000 C CNN
F 1 "MOSFET_DRIVER_LT1161" H 2750 4691 60  0000 C CNN
F 2 "usevolt:SO20" H 2800 4875 60  0001 C CNN
F 3 "http://www.linear.com/docs/3607" H 2850 4725 60  0001 C CNN
F 4 "LINEAR TECHNOLOGY" H 2775 4950 60  0001 C CNN "Manufacturer"
F 5 "LT1161" H 2800 5050 60  0001 C CNN "Manufacturer Part Number"
F 6 "Digikey" H 2800 5150 60  0001 C CNN "1st Source"
F 7 "LT1161ISW#TRPBFCT-ND" H 2900 4800 60  0001 C CNN "1st Source Part Number"
F 8 "Farnell" H 2750 4000 60  0001 C CNN "2nd Source"
F 9 "2115820" H 2750 4000 60  0001 C CNN "2nd Source Part Number"
	1    2750 4000
	1    0    0    -1  
$EndComp
$Sheet
S 4650 3250 950  350 
U 58BE8E7A
F0 "sheet58BE8E46" 60
F1 "currentSense.sch" 60
F2 "SENSE+" I R 5600 3350 60 
F3 "SENSE-" I R 5600 3500 60 
F4 "VOUT" O L 4650 3350 60 
$EndSheet
$Sheet
S 4500 4150 950  350 
U 58BE8E88
F0 "sheet58BE8E47" 60
F1 "currentSense.sch" 60
F2 "SENSE+" I R 5450 4250 60 
F3 "SENSE-" I R 5450 4400 60 
F4 "VOUT" O L 4500 4250 60 
$EndSheet
$Sheet
S 4500 4700 950  350 
U 58BE8E96
F0 "sheet58BE8E48" 60
F1 "currentSense.sch" 60
F2 "SENSE+" I R 5450 4800 60 
F3 "SENSE-" I R 5450 4950 60 
F4 "VOUT" O L 4500 4800 60 
$EndSheet
$Comp
L 100nF C?
U 1 1 58BE8EAF
P 3550 3450
AR Path="/58BE8EAF" Ref="C?"  Part="1" 
AR Path="/58BE52C9/58BE8EAF" Ref="C501"  Part="1" 
AR Path="/58C7B0A8/58BE8EAF" Ref="C1001"  Part="1" 
F 0 "C1001" H 3600 3649 45  0000 C CNN
F 1 "100nF" H 3600 3565 45  0000 C CNN
F 2 "kemet:0603" H 3580 3600 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2062872.pdf?_ga=1.250530118.1948984246.1460023966" H 3454 3459 60  0001 C CNN
F 4 "1288255" H 3854 3859 60  0001 C CNN "1st Source Part Number"
F 5 "C0603C104K5RACTU" H 3654 3659 60  0001 C CNN "Manufacturer Part Number"
F 6 "Farnell" H 3754 3759 60  0001 C CNN "1st Source"
F 7 "KEMET" H 3554 3559 60  0001 C CNN "Manufacturer"
	1    3550 3450
	1    0    0    -1  
$EndComp
$Comp
L 100nF C?
U 1 1 58BE8EBA
P 3550 4600
AR Path="/58BE8EBA" Ref="C?"  Part="1" 
AR Path="/58BE52C9/58BE8EBA" Ref="C502"  Part="1" 
AR Path="/58C7B0A8/58BE8EBA" Ref="C1002"  Part="1" 
F 0 "C1002" H 3600 4799 45  0000 C CNN
F 1 "100nF" H 3600 4715 45  0000 C CNN
F 2 "kemet:0603" H 3580 4750 20  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2062872.pdf?_ga=1.250530118.1948984246.1460023966" H 3454 4609 60  0001 C CNN
F 4 "1288255" H 3854 5009 60  0001 C CNN "1st Source Part Number"
F 5 "C0603C104K5RACTU" H 3654 4809 60  0001 C CNN "Manufacturer Part Number"
F 6 "Farnell" H 3754 4909 60  0001 C CNN "1st Source"
F 7 "KEMET" H 3554 4709 60  0001 C CNN "Manufacturer"
	1    3550 4600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR502
U 1 1 58BE8EC1
P 3750 4700
AR Path="/58BE52C9/58BE8EC1" Ref="#PWR502"  Part="1" 
AR Path="/58C7B0A8/58BE8EC1" Ref="#PWR1002"  Part="1" 
F 0 "#PWR1002" H 3750 4450 50  0001 C CNN
F 1 "GND" H 3755 4527 50  0000 C CNN
F 2 "" H 3750 4700 50  0000 C CNN
F 3 "" H 3750 4700 50  0000 C CNN
	1    3750 4700
	1    0    0    -1  
$EndComp
Text HLabel 7450 4100 2    60   Output ~ 0
OUT1
Text HLabel 7450 4400 2    60   Output ~ 0
OUT2
Text HLabel 7450 5050 2    60   Output ~ 0
OUT3
Text HLabel 7450 5350 2    60   Output ~ 0
OUT4
Text HLabel 2000 3700 0    60   Input ~ 0
IN1
Text HLabel 1900 3950 0    60   Input ~ 0
IN2
Text HLabel 1800 4200 0    60   Input ~ 0
IN3
Text HLabel 1700 4450 0    60   Input ~ 0
IN4
Text HLabel 2400 2850 0    60   Output ~ 0
SENSE1
Text HLabel 2300 2950 0    60   Output ~ 0
SENSE2
Text HLabel 2200 3050 0    60   Output ~ 0
SENSE3
Text HLabel 2100 3150 0    60   Output ~ 0
SENSE4
Wire Wire Line
	2400 2850 4650 2850
Wire Wire Line
	5800 4500 6200 4500
Wire Wire Line
	5600 3000 6100 3000
Wire Wire Line
	6100 3000 6100 4100
Wire Wire Line
	6100 4100 6200 4100
Wire Wire Line
	2000 3700 2300 3700
Connection ~ 2100 4600
Wire Wire Line
	2100 3450 2300 3450
Wire Wire Line
	2100 4600 2300 4600
Wire Wire Line
	2100 3450 2100 4700
Connection ~ 3400 4350
Wire Wire Line
	3200 4600 3450 4600
Connection ~ 3400 4100
Wire Wire Line
	3400 4350 3200 4350
Connection ~ 3400 3850
Wire Wire Line
	3400 4100 3200 4100
Connection ~ 3400 3600
Wire Wire Line
	3400 3850 3200 3850
Connection ~ 3400 3450
Wire Wire Line
	3400 3600 3200 3600
Wire Wire Line
	3200 3450 3450 3450
Wire Wire Line
	3400 2050 3400 4600
Wire Wire Line
	6000 4400 6200 4400
Wire Wire Line
	6000 3500 6000 4400
Wire Wire Line
	6000 3500 5600 3500
Wire Wire Line
	5750 3350 5600 3350
Wire Wire Line
	2300 2950 4250 2950
Wire Wire Line
	4250 2950 4250 3350
Wire Wire Line
	4250 3350 4650 3350
Wire Wire Line
	1900 3950 2300 3950
Wire Wire Line
	6050 5150 3950 5150
Wire Wire Line
	3950 5150 3950 4200
Wire Wire Line
	3950 4200 3200 4200
Wire Wire Line
	3200 4450 3850 4450
Wire Wire Line
	3850 4450 3850 5450
Wire Wire Line
	3850 5450 6050 5450
Wire Wire Line
	3200 3700 5900 3700
Wire Wire Line
	5900 3700 5900 4200
Wire Wire Line
	5900 4200 6200 4200
Wire Wire Line
	5800 4500 5800 3950
Wire Wire Line
	5800 3950 3200 3950
Wire Wire Line
	5600 4800 5450 4800
Wire Wire Line
	5450 4400 5700 4400
Wire Wire Line
	5700 4400 5700 5050
Wire Wire Line
	5700 5050 6050 5050
Wire Wire Line
	5450 4950 5600 4950
Wire Wire Line
	5600 4950 5600 5350
Wire Wire Line
	5600 5350 6050 5350
Wire Wire Line
	1800 4200 2300 4200
Wire Wire Line
	2200 3050 4150 3050
Wire Wire Line
	4150 3050 4150 4250
Wire Wire Line
	4150 4250 4500 4250
Wire Wire Line
	4500 4800 4050 4800
Wire Wire Line
	4050 4800 4050 3150
Wire Wire Line
	4050 3150 2100 3150
Wire Wire Line
	1700 4450 2300 4450
Connection ~ 3400 4600
Wire Wire Line
	3750 3450 3750 4700
Connection ~ 3750 4600
Wire Wire Line
	6650 5350 7450 5350
Wire Wire Line
	7450 5050 6650 5050
Wire Wire Line
	6800 4400 7450 4400
Wire Wire Line
	7450 4100 6800 4100
Wire Wire Line
	5600 3800 5600 4800
Wire Wire Line
	5600 4250 5450 4250
Connection ~ 5750 2850
Wire Wire Line
	5750 2850 5600 2850
Wire Wire Line
	5750 2050 5750 3350
Text HLabel 5950 2050 2    60   Input ~ 0
VDD
Wire Wire Line
	3400 2050 5950 2050
Connection ~ 5750 2050
Wire Wire Line
	5600 3800 5850 3800
Wire Wire Line
	5850 3800 5850 2050
Connection ~ 5850 2050
Connection ~ 5600 4250
$Comp
L MOSFET_IPG20N04S4L11ATMA1 U502
U 1 1 591C1D78
P 6350 5250
AR Path="/58BE52C9/591C1D78" Ref="U502"  Part="1" 
AR Path="/58C7B0A8/591C1D78" Ref="U1002"  Part="1" 
F 0 "U1002" H 6350 5697 60  0000 C CNN
F 1 "MOSFET_IPG20N04S4L11ATMA1" H 6350 5591 60  0000 C CNN
F 2 "usevolt:PG-TDSON-8-4" H 6350 5750 60  0001 C CNN
F 3 "http://www.infineon.com/dgdl/Infineon-IPG20N04S4L_11-DS-v01_00-en.pdf?fileId=db3a30432ba3fa6f012bbf644b016c14" H 7400 6150 60  0001 C CNN
F 4 "Digikey" H 6850 6050 60  0001 C CNN "2nd Source"
F 5 "IPG20N04S4L11ATMA1CT-ND" H 6950 6150 60  0001 C CNN "2nd Source Part Number"
F 6 "2443439" H 6850 6250 60  0001 C CNN "1st Source Part Number"
F 7 "Farnell" H 6750 6150 60  0001 C CNN "1st Source"
F 8 "INFINEON" H 6550 5950 60  0001 C CNN "Manufacturer"
F 9 "IPG20N04S4L11ATMA1" H 6650 6050 60  0001 C CNN "Manufacturer Part Number"
	1    6350 5250
	1    0    0    -1  
$EndComp
$Comp
L MOSFET_IPG20N04S4L11ATMA1 U503
U 1 1 591C1E34
P 6500 4300
AR Path="/58BE52C9/591C1E34" Ref="U503"  Part="1" 
AR Path="/58C7B0A8/591C1E34" Ref="U1003"  Part="1" 
F 0 "U1003" H 6500 4747 60  0000 C CNN
F 1 "MOSFET_IPG20N04S4L11ATMA1" H 6500 4641 60  0000 C CNN
F 2 "usevolt:PG-TDSON-8-4" H 6500 4800 60  0001 C CNN
F 3 "http://www.infineon.com/dgdl/Infineon-IPG20N04S4L_11-DS-v01_00-en.pdf?fileId=db3a30432ba3fa6f012bbf644b016c14" H 7550 5200 60  0001 C CNN
F 4 "Digikey" H 7000 5100 60  0001 C CNN "2nd Source"
F 5 "IPG20N04S4L11ATMA1CT-ND" H 7100 5200 60  0001 C CNN "2nd Source Part Number"
F 6 "2443439" H 7000 5300 60  0001 C CNN "1st Source Part Number"
F 7 "Farnell" H 6900 5200 60  0001 C CNN "1st Source"
F 8 "INFINEON" H 6700 5000 60  0001 C CNN "Manufacturer"
F 9 "IPG20N04S4L11ATMA1" H 6800 5100 60  0001 C CNN "Manufacturer Part Number"
	1    6500 4300
	1    0    0    -1  
$EndComp
$EndSCHEMATC
