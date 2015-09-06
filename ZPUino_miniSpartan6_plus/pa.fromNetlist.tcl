
# PlanAhead Launch Script for Post-Synthesis floorplanning, created by Project Navigator

create_project -name miniSpartan6_plus_ZPUino_Vanilla -dir "C:/FPGA/miniSpartan6/ZPUino_miniSpartan6_plus/planAhead_run_2" -part xc6slx25ftg256-3
set_property design_mode GateLvl [get_property srcset [current_run -impl]]
set_property edif_top_file "C:/FPGA/miniSpartan6/ZPUino_miniSpartan6_plus/miniSpartan6_plus_top.ngc" [ get_property srcset [ current_run ] ]
add_files -norecurse { {C:/FPGA/miniSpartan6/ZPUino_miniSpartan6_plus} }
set_property target_constrs_file "C:/FPGA/miniSpartan6/ZPUino_miniSpartan6_plus/ipcore_dir/miniSpartan6.ucf" [current_fileset -constrset]
add_files [list {C:/FPGA/miniSpartan6/ZPUino_miniSpartan6_plus/ipcore_dir/miniSpartan6.ucf}] -fileset [get_property constrset [current_run]]
link_design
