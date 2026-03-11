@echo off

Rem Path to the executable exe of the Vector HexView tool on your PC
set "HexViewPath=D:\sandbox\01_tools\HexView\HexView\hexview.exe"

Rem Filled input file Input_Hex_File
set "Input_Hex_File=D:\03_project\stm32\f103\01_code\APP\stm32f103_APP\stm32_Bootload_UDS\build\Debug\a.hex"  

Rem The output file Output_Hex_File after filling
set "Output_Hex_File=D:\03_project\stm32\f103\01_code\APP\stm32f103_APP\stm32_Bootload_UDS\build\Debug\a.hex"

echo Filling the hex file to 32K...

Rem For a new Output_Hex_File, fill in the characters
%HexViewPath% %Input_Hex_File% /S /FA: /AF:0xFF /XI:32 -o %Output_Hex_File%