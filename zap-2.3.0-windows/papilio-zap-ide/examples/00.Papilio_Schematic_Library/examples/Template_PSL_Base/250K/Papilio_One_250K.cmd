xst -intstyle ise -ifn "C:/dropbox/GadgetFactory/GadgetFactory_Engineering/Papilio-Schematic-Library/examples/Template_PSL_Base/250K/Papilio_One_250K.xst" -ofn "C:/dropbox/GadgetFactory/GadgetFactory_Engineering/Papilio-Schematic-Library/examples/Template_PSL_Base/250K/Papilio_One_250K.syr" 
ngdbuild -intstyle ise -dd _ngo -aul -nt timestamp -uc C:/dropbox/GadgetFactory/GadgetFactory_Engineering/Papilio-Schematic-Library/examples/Template_PSL_Base/Libraries/ZPUino_1/board_Papilio_One_250k/papilio_one.ucf -p xc3s250e-vq100-4 Papilio_One_250K.ngc Papilio_One_250K.ngd  
map -intstyle ise -p xc3s250e-vq100-4 -cm area -ir off -pr off -c 100 -o Papilio_One_250K_map.ncd Papilio_One_250K.ngd Papilio_One_250K.pcf 
par -w -intstyle ise -ol high -t 1 Papilio_One_250K_map.ncd Papilio_One_250K.ncd Papilio_One_250K.pcf 
trce -intstyle ise -v 3 -s 4 -n 3 -fastpaths -xml Papilio_One_250K.twx Papilio_One_250K.ncd -o Papilio_One_250K.twr Papilio_One_250K.pcf 
bitgen -intstyle ise -f Papilio_One_250K.ut Papilio_One_250K.ncd 
