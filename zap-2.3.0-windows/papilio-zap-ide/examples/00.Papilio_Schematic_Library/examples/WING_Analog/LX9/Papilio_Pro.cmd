xst -intstyle ise -ifn "C:/dropbox/GadgetFactory/GadgetFactory_Engineering/Papilio-Schematic-Library/examples/WING_Analog/LX9/Papilio_Pro.xst" -ofn "C:/dropbox/GadgetFactory/GadgetFactory_Engineering/Papilio-Schematic-Library/examples/WING_Analog/LX9/Papilio_Pro.syr" 
ngdbuild -intstyle ise -dd _ngo -aul -nt timestamp -uc C:/dropbox/GadgetFactory/GadgetFactory_Engineering/Papilio-Schematic-Library/examples/WING_Analog/Libraries/ZPUino_1/board_Papilio_Pro/papilio_pro.ucf -p xc6slx9-tqg144-2 Papilio_Pro.ngc Papilio_Pro.ngd  
map -intstyle ise -p xc6slx9-tqg144-2 -w -logic_opt off -ol high -t 1 -xt 0 -register_duplication off -r 4 -global_opt off -mt off -ir off -pr off -lc off -power off -o Papilio_Pro_map.ncd Papilio_Pro.ngd Papilio_Pro.pcf 
par -w -intstyle ise -ol high -mt off Papilio_Pro_map.ncd Papilio_Pro.ncd Papilio_Pro.pcf 
trce -intstyle ise -v 3 -s 2 -n 3 -fastpaths -xml Papilio_Pro.twx Papilio_Pro.ncd -o Papilio_Pro.twr Papilio_Pro.pcf 
bitgen -intstyle ise -f Papilio_Pro.ut Papilio_Pro.ncd 
