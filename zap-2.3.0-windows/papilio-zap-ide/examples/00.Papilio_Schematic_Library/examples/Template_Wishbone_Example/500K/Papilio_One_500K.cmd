xst -intstyle ise -ifn "C:/dropbox/GadgetFactory/GadgetFactory_Engineering/Papilio-Schematic-Library/Papilio_Schematic_Projects/Template_Wishbone_Example/500K/Papilio_One_500K.xst" -ofn "C:/dropbox/GadgetFactory/GadgetFactory_Engineering/Papilio-Schematic-Library/Papilio_Schematic_Projects/Template_Wishbone_Example/500K/Papilio_One_500K.syr" 
ngdbuild -intstyle ise -dd _ngo -aul -nt timestamp -uc C:/dropbox/GadgetFactory/GadgetFactory_Engineering/Papilio-Schematic-Library/Libraries/ZPUino_1/board_Papilio_One_500k/papilio_one.ucf -p xc3s500e-vq100-5 Papilio_One_500K.ngc Papilio_One_500K.ngd  
map -intstyle ise -p xc3s500e-vq100-5 -cm area -ir off -pr off -c 100 -o Papilio_One_500K_map.ncd Papilio_One_500K.ngd Papilio_One_500K.pcf 
par -w -intstyle ise -ol high -t 1 Papilio_One_500K_map.ncd Papilio_One_500K.ncd Papilio_One_500K.pcf 
trce -intstyle ise -v 3 -s 5 -n 3 -fastpaths -xml Papilio_One_500K.twx Papilio_One_500K.ncd -o Papilio_One_500K.twr Papilio_One_500K.pcf 
bitgen -intstyle ise -f Papilio_One_500K.ut Papilio_One_500K.ncd 
