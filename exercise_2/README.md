# Prerequisites #
1. MATLAB R2018b to R2022a on Windows are supported.

2. In MATLAB install MinGW-w64 C/C++ Compiler using the instruction manual found in Blackboard.
Course work ➔ Matlab_c_compiler_install_procedure.
It is also possible running Exercise 2 on Apple Mac. You must install Xcode via App Store. 
Alternatively you can access MATLAB using Microsoft Remote Desktop connecting to NTNU softwarefarm.

3. Go this link https://github.com/pesc-master-student/TET4120 and download the zip file
by clicking on code menu and Download ZIP.

3. Store the extracted files in the current working directory of the MATLAB (found above the command window).

5. Go inside tet4120-main\exercise_2 folder. You can find one .slx file, two .cpp files and one .hpp file.

7. Enter the following line in the MATLAB command window and press enter. <br/>
```mex sfun_load_emu.cpp SwLoadEmulator.cpp -DSIMULINK -D_USE_MATH_DEFINES```

9. A new sfun_load_emu.mexw64 file will be created.
10. Open Ex2_Loadmodelling.slx file and double click the Load model with C++ block to
check if pump is selected. Run the model and check the C++ scope to see the load in
Simulink compared with the C++ load model.
