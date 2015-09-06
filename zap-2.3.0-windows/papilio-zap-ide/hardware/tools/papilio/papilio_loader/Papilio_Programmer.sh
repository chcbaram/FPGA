# Copyright 2009-2011 Jack Gassett
# Creative Commons Attribution license
# Made for the Papilio FPGA boards

set -e

bitfile=bitfile
	
# ./dialog --timeout 3 --title "Papilio Programmer" \
		# --menu "Use the arrow keys to select your option:" 15 55 5 \
		# "1" "Program FPGA - Temporary" \
		# "2" "Program SPI Flash - Permanent" \
		# "3" "Save bit file to Favorites folder" 2> $bitfile
return_value="2"	
#echo $return_value

case "$return_value" in
	"1")
	
		echo "Programming the FPGA"
		./papilio-prog -v -f "$1" -v
		return_value=$?
		;;	
	"2")
		echo "Programming to SPI Flash"
		# Find device id and choose appropriate bscan bit file
	
		device_id=`./papilio-prog -j | ./gawk '{print $9}'`
		return_value=$?
		
		case $device_id in
			XC3S250E)
				echo "Programming a Papilio One 250K"
				bscan_bitfile=bscan_spi_xc3s250e.bit
				;;	
			XC3S500E)
				echo "Programming a Papilio One 500K"
				bscan_bitfile=bscan_spi_xc3s500e.bit
				;;
			XC6SLX9)
				echo "Programming a Papilio Plus LX9"
				bscan_bitfile=bscan_spi_lx9.bit
				;;				
			*)
				echo "Unknown Papilio Board"
				;;
		esac

		./papilio-prog -v -f "$1" -b $bscan_bitfile -sa -r
		#Cause the Papilio to restart
		./papilio-prog -c
		return_value=$?
		;;
	"3")
		echo "Copy to Favorites"
		#TODO figure how to do in Windows and Linux
		./cp $1 "$2/Favorites/"
		cd "$2/Favorites/"
		explorer .	
		;;
	*)
		echo "Programming the FPGA - Default"
		./papilio-prog -v -f "$1" -v
		return_value=$?
		;;
		#exit 1
esac
		
