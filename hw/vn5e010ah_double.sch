EESchema Schematic File Version 4
LIBS:esb-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 13 13
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 7600 3500 2    60   Output ~ 0
OUT1
Text HLabel 4300 3500 0    60   Input ~ 0
IN1
Text HLabel 5200 4100 0    60   Output ~ 0
SENSE1
Wire Wire Line
	5200 3600 5450 3600
Wire Wire Line
	4700 3600 4900 3600
Wire Wire Line
	4300 3500 4450 3500
Wire Wire Line
	5200 4100 5450 4100
$Comp
L power:VDD #PWR01203
U 1 1 5B1A4079
P 5450 3300
AR Path="/5B1A3D6E/5B1A4079" Ref="#PWR01203"  Part="1" 
AR Path="/5A13680F/5B1A4079" Ref="#PWR01103"  Part="1" 
AR Path="/5C202EB2/5B1A4079" Ref="#PWR0104"  Part="1" 
F 0 "#PWR0104" H 5450 3150 50  0001 C CNN
F 1 "VDD" H 5467 3473 50  0000 C CNN
F 2 "" H 5450 3300 50  0000 C CNN
F 3 "" H 5450 3300 50  0000 C CNN
	1    5450 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 3400 5950 3400
Wire Wire Line
	7000 3600 7100 3600
Wire Wire Line
	7100 3600 7100 3500
Wire Wire Line
	7000 3500 7100 3500
Connection ~ 7100 3500
Wire Wire Line
	5450 3300 5450 3400
Wire Wire Line
	5450 4100 5450 3600
Connection ~ 5450 3600
$Comp
L power:GND #PWR01201
U 1 1 5B1A4087
P 4700 3900
AR Path="/5B1A3D6E/5B1A4087" Ref="#PWR01201"  Part="1" 
AR Path="/5A13680F/5B1A4087" Ref="#PWR01101"  Part="1" 
AR Path="/5C202EB2/5B1A4087" Ref="#PWR0105"  Part="1" 
F 0 "#PWR0105" H 4700 3650 50  0001 C CNN
F 1 "GND" H 4705 3727 50  0000 C CNN
F 2 "" H 4700 3900 50  0000 C CNN
F 3 "" H 4700 3900 50  0000 C CNN
	1    4700 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 3600 4700 3850
$Comp
L yageo:1K R?
U 1 1 5B1A4092
P 4450 3650
AR Path="/5A01611C/5B1A4092" Ref="R?"  Part="1" 
AR Path="/5A019746/5B1A4092" Ref="R?"  Part="1" 
AR Path="/5A01A445/5B1A4092" Ref="R?"  Part="1" 
AR Path="/5A01AB59/5B1A4092" Ref="R?"  Part="1" 
AR Path="/5A120DD2/5B1A4092" Ref="R?"  Part="1" 
AR Path="/5A12FF64/5B1A4092" Ref="R?"  Part="1" 
AR Path="/5A130D3E/5B1A4092" Ref="R?"  Part="1" 
AR Path="/5A1349CF/5B1A4092" Ref="R?"  Part="1" 
AR Path="/5A1358B0/5B1A4092" Ref="R?"  Part="1" 
AR Path="/5A13680F/5B1A4092" Ref="R1101"  Part="1" 
AR Path="/5A14370A/5B1A4092" Ref="R?"  Part="1" 
AR Path="/5B1A3D6E/5B1A4092" Ref="R1202"  Part="1" 
AR Path="/5C202EB2/5B1A4092" Ref="R7"  Part="1" 
F 0 "R7" V 4458 3708 45  0000 L CNN
F 1 "1K" V 4542 3708 45  0000 L CNN
F 2 "yageo:0603" H 4480 3800 20  0001 C CNN
F 3 "" H 4419 3689 60  0001 C CNN
F 4 "" H 4819 4089 60  0001 C CNN "1st Source Part Number"
F 5 "Digikey" H 4719 3989 60  0001 C CNN "1st Source"
F 6 "RC0603FR-071KL" H 4619 3889 60  0001 C CNN "Manufacturer Part Number"
F 7 "YAGEO" H 4519 3789 60  0001 C CNN "Manufacturer"
	1    4450 3650
	0    1    1    0   
$EndComp
Wire Wire Line
	4450 3550 4450 3500
Connection ~ 4450 3500
Wire Wire Line
	4450 3850 4700 3850
Connection ~ 4700 3850
$Comp
L usevolt:VN5E010AH U1202
U 1 1 5B1A40AC
P 6700 3600
AR Path="/5B1A3D6E/5B1A40AC" Ref="U1202"  Part="1" 
AR Path="/5A13680F/5B1A40AC" Ref="U1103"  Part="1" 
AR Path="/5C202EB2/5B1A40AC" Ref="U2"  Part="1" 
F 0 "U2" H 6650 4047 60  0000 C CNN
F 1 "VN5E010AH" H 6650 3941 60  0000 C CNN
F 2 "usevolt:HPAK" H 6350 4550 60  0001 C CNN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/1e/52/ef/e0/cc/90/4e/2e/CD00240716.pdf/files/CD00240716.pdf/jcr:content/translations/en.CD00240716.pdf" H 6450 4650 60  0001 C CNN
F 4 "STMicroelectronics" H 6550 4750 60  0001 C CNN "Manufacturer"
F 5 "VN5E010AHTR-E" H 6650 4850 60  0001 C CNN "Manufacturer Part Number"
F 6 "Digikey" H 6750 4950 60  0001 C CNN "1st Source"
F 7 "497-10505-2-ND" H 6850 5050 60  0001 C CNN "1st Source Part Number"
	1    6700 3600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR01204
U 1 1 5B1A40B3
P 5650 3900
AR Path="/5B1A3D6E/5B1A40B3" Ref="#PWR01204"  Part="1" 
AR Path="/5A13680F/5B1A40B3" Ref="#PWR01104"  Part="1" 
AR Path="/5C202EB2/5B1A40B3" Ref="#PWR0112"  Part="1" 
F 0 "#PWR0112" H 5650 3650 50  0001 C CNN
F 1 "GND" H 5655 3727 50  0000 C CNN
F 2 "" H 5650 3900 50  0000 C CNN
F 3 "" H 5650 3900 50  0000 C CNN
	1    5650 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 3700 5650 3800
Wire Wire Line
	5650 3800 6150 3800
Wire Wire Line
	6300 3700 5650 3700
Connection ~ 5650 3800
Wire Wire Line
	7100 3500 7300 3500
Wire Wire Line
	5450 3600 6300 3600
Wire Wire Line
	4450 3500 6050 3500
Wire Wire Line
	4700 3850 4700 3900
Wire Wire Line
	5650 3800 5650 3900
$Comp
L usevolt:VN5E010AH U1201
U 1 1 5B1A4293
P 6700 2800
AR Path="/5B1A3D6E/5B1A4293" Ref="U1201"  Part="1" 
AR Path="/5A13680F/5B1A4293" Ref="U1102"  Part="1" 
AR Path="/5C202EB2/5B1A4293" Ref="U1"  Part="1" 
F 0 "U1" H 6650 3247 60  0000 C CNN
F 1 "VN5E010AH" H 6650 3141 60  0000 C CNN
F 2 "usevolt:HPAK" H 6350 3750 60  0001 C CNN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/1e/52/ef/e0/cc/90/4e/2e/CD00240716.pdf/files/CD00240716.pdf/jcr:content/translations/en.CD00240716.pdf" H 6450 3850 60  0001 C CNN
F 4 "STMicroelectronics" H 6550 3950 60  0001 C CNN "Manufacturer"
F 5 "VN5E010AHTR-E" H 6650 4050 60  0001 C CNN "Manufacturer Part Number"
F 6 "Digikey" H 6750 4150 60  0001 C CNN "1st Source"
F 7 "497-10505-2-ND" H 6850 4250 60  0001 C CNN "1st Source Part Number"
	1    6700 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 3000 6150 3000
Wire Wire Line
	6150 3000 6150 3800
Connection ~ 6150 3800
Wire Wire Line
	6150 3800 6300 3800
Wire Wire Line
	6300 2700 6050 2700
Wire Wire Line
	6050 2700 6050 3500
Connection ~ 6050 3500
Wire Wire Line
	6050 3500 6300 3500
Wire Wire Line
	5950 3400 5950 2600
Wire Wire Line
	5950 2600 6300 2600
Connection ~ 5950 3400
Wire Wire Line
	5950 3400 6300 3400
Wire Wire Line
	7000 2800 7100 2800
Wire Wire Line
	7100 2800 7100 3500
Wire Wire Line
	7000 2700 7100 2700
Wire Wire Line
	7100 2700 7100 2800
Connection ~ 7100 2800
Wire Wire Line
	6300 2900 6150 2900
Wire Wire Line
	6150 2900 6150 3000
Connection ~ 6150 3000
$Comp
L yageo:1K R1203
U 1 1 5B2322BF
P 5000 3600
AR Path="/5B1A3D6E/5B2322BF" Ref="R1203"  Part="1" 
AR Path="/5A13680F/5B2322BF" Ref="R1102"  Part="1" 
AR Path="/5C202EB2/5B2322BF" Ref="R8"  Part="1" 
F 0 "R8" H 5050 3784 45  0000 C CNN
F 1 "1K" H 5050 3700 45  0000 C CNN
F 2 "yageo:0603" H 5030 3750 20  0001 C CNN
F 3 "" H 4969 3639 60  0001 C CNN
F 4 "" H 5369 4039 60  0001 C CNN "1st Source Part Number"
F 5 "Digikey" H 5269 3939 60  0001 C CNN "1st Source"
F 6 "RC0603FR-071KL" H 5169 3839 60  0001 C CNN "Manufacturer Part Number"
F 7 "YAGEO" H 5069 3739 60  0001 C CNN "Manufacturer"
	1    5000 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 4400 5450 4100
Connection ~ 5450 4100
$Comp
L yageo:10K R6
U 1 1 5BA96DF9
P 7300 3700
AR Path="/5B1A3D6E/5BA96DF9" Ref="R6"  Part="1" 
AR Path="/5A13680F/5BA96DF9" Ref="R5"  Part="1" 
AR Path="/5C202EB2/5BA96DF9" Ref="R9"  Part="1" 
F 0 "R9" V 7308 3758 45  0000 L CNN
F 1 "10K" V 7392 3758 45  0000 L CNN
F 2 "yageo:0603" H 7330 3850 20  0001 C CNN
F 3 "" H 7269 3739 60  0001 C CNN
F 4 "" H 7669 4139 60  0001 C CNN "1st Source Part Number"
F 5 "Digikey" H 7569 4039 60  0001 C CNN "1st Source"
F 6 "RC0603FR-0710KL" H 7469 3939 60  0001 C CNN "Manufacturer Part Number"
F 7 "YAGEO" H 7369 3839 60  0001 C CNN "Manufacturer"
	1    7300 3700
	0    1    1    0   
$EndComp
Wire Wire Line
	7300 3600 7300 3500
Connection ~ 7300 3500
Wire Wire Line
	7300 3500 7600 3500
$Comp
L power:GND #PWR0111
U 1 1 5BA9746E
P 7300 4000
AR Path="/5B1A3D6E/5BA9746E" Ref="#PWR0111"  Part="1" 
AR Path="/5A13680F/5BA9746E" Ref="#PWR0110"  Part="1" 
AR Path="/5C202EB2/5BA9746E" Ref="#PWR0113"  Part="1" 
F 0 "#PWR0113" H 7300 3750 50  0001 C CNN
F 1 "GND" H 7305 3827 50  0000 C CNN
F 2 "" H 7300 4000 50  0000 C CNN
F 3 "" H 7300 4000 50  0000 C CNN
	1    7300 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 4000 7300 3900
$Comp
L usevolt:ZENER_MM3Z3V6T1G D8
U 1 1 5BAAE03B
P 5400 4400
AR Path="/5B1A3D6E/5BAAE03B" Ref="D8"  Part="1" 
AR Path="/5A13680F/5BAAE03B" Ref="D7"  Part="1" 
AR Path="/5C202EB2/5BAAE03B" Ref="D9"  Part="1" 
F 0 "D9" H 5325 4520 45  0000 C CNN
F 1 "ZENER_MM3Z3V6T1G" H 5675 4775 45  0001 L BNN
F 2 "usevolt:SOD-323" H 5375 4775 20  0001 C CNN
F 3 "http://www.mouser.com/ds/2/308/MM3Z2V4T1-D-269162.pdf" H 5750 4975 60  0001 C CNN
F 4 "Mouser" H 5720 4720 60  0001 C CNN "1st Source"
F 5 "MM3Z3V6T1G" H 5620 4620 60  0001 C CNN "Manufacturer Part Number"
F 6 "863-MM3Z3V6T1G" H 5820 4820 60  0001 C CNN "1st Source Part Number"
F 7 "ON Semiconductor" H 5675 4875 60  0001 C CNN "Manufacturer"
	1    5400 4400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 5BAAE0D6
P 4900 4500
AR Path="/5B1A3D6E/5BAAE0D6" Ref="#PWR0103"  Part="1" 
AR Path="/5A13680F/5BAAE0D6" Ref="#PWR0102"  Part="1" 
AR Path="/5C202EB2/5BAAE0D6" Ref="#PWR0114"  Part="1" 
F 0 "#PWR0114" H 4900 4250 50  0001 C CNN
F 1 "GND" H 4905 4327 50  0000 C CNN
F 2 "" H 4900 4500 50  0000 C CNN
F 3 "" H 4900 4500 50  0000 C CNN
	1    4900 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 4500 4900 4400
Wire Wire Line
	4900 4400 5200 4400
$EndSCHEMATC
