# Copyright 2009-2011 Jack Gassett
# Creative Commons Attribution license
# Made for the Papilio FPGA boards

COMPORT=COM17

# putty.exe -serial $COMPORT &
sleep 1
		echo "Starting Papilio Plus Test"
		# Find device id and choose appropriate bscan bit file
	
		device_id=`./papilio-prog.exe -j | ./gawk '{print $9}'`
		return_value=$?
		
		case $device_id in
			XC3S250E)
				echo "This test is not for the Papilio One 250K"
				exit
				;;	
			XC3S500E)
				echo "This test is not for the Papilio One 500K"
				exit
				;;
			XC6SLX4)
				echo "Papilio Pro LX4 detected"
				echo "This test is not for the Papilio Pro LX4"
				exit			
				;;			
			XC6SLX9)
				echo "Papilio Pro LX9 detected"
				bitfile=Papilio_Test_Plan_Board_Under_Test_LX9.bit
				bscan_bitfile=bscan_spi_xc6slx9.bit
				quickstartbitfile=QuickStart-Zpuino-1.0-PapilioPro-S6LX9-RetroCade-1.04.bit			
				;;						
			*)
				echo "Unknown Papilio Board"
				;;
		esac

		./papilio-prog.exe -v -f $quickstartbitfile -b $bscan_bitfile -sa -r
		return_value=$?	
		#Cause the Papilio to restart
		./papilio-prog.exe -c
		# return_value=$?	
		
if [ $return_value == 1 ] #If programming failed then show error.
then
	./dialog --timeout 5 --msgbox "The bit file failed to program to the Papilio, please check any issues with the FTDI chip, USB connector, voltage regulators, or solder joints." 15 55
	read -n1 -r -p "Press any key to continue..." key
fi		
		
echo
echo
echo "Step 1 - The JTAG Programming, Memory, and serial communication test has been loaded to the Papilio, please complete the test in Hyperterminal before proceeding."
#read -n1 -r -p "Press any key to continue with step 2 - I/O Test." key		
		
		# ./papilio-prog.exe -v -f $bitfile -v
		# return_value=$?
		
# if [ $return_value == 1 ] #If programming failed then show error.
# then
	# ./dialog --timeout 5 --msgbox "The bit file failed to program to the Papilio, please check any issues with the FTDI chip, USB connector, voltage regulators, or solder joints." 15 55
	# read -n1 -r -p "Press any key to continue..." key
# fi

# echo
# echo
# echo "Step 2 - The I/O Test has been loaded to the Papilio, please complete the test in Hyperterminal before proceeding."
# read -n1 -r -p "Press any key to continue with loading the default 'QuickStart' Papilio bit file to SPI Flash once the I/O Test is complete." key

		# ./papilio-prog.exe -v -f $quickstartbitfile -b $bscan_bitfile -sa -r
		#Cause the Papilio to restart
		# ./papilio-prog.exe -c
		# return_value=$?

# if [ $return_value == 1 ] #If programming failed then show error.
# then
	# ./dialog --timeout 5 --msgbox "The bit file failed to program to the SPI Flash of the Papilio, please check for any problems with the SPI Flash." 15 55
	# read -n1 -r -p "Press any key to continue..." key
# fi		

# echo
# echo
# echo "The QuickStart bit file has been loaded to SPI Flash please verify that you see the ASCII table in Hyperterminal ."
read -n1 -r -p "Verification is complete, please press any key to quit." key