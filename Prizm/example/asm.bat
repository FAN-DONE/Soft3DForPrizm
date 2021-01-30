cls
cls
cls
..\..\bin\sh3eb-elf-g++ -MMD -MP -MF D:\PRIZMSDK\projects\example\main.d   -Wall -O3 -mb -m4a-nofpu -mhitachi -nostdlib   -ID:\PRIZMSDK\projects\example -ID:\PRIZMSDK\include -ffunction-sections -fdata-sections  -c D:\PRIZMSDK\projects\example\src\main.cpp -S
..\..\bin\sh3eb-elf-g++ -MMD -MP -MF D:\PRIZMSDK\projects\example\main.d  -Wall  -O3 -mb -m4a-nofpu -mhitachi -nostdlib  -ID:\PRIZMSDK\include -ffunction-sections -fdata-sections  -c D:\PRIZMSDK\projects\example\src\main.cpp -o mainasm.o
..\..\bin\sh3eb-elf-g++  mainasm.o -mb -m4a-nofpu -mhitachi -nostdlib -TD:/PRIZMSDK/toolchain/prizm.x -Wl,-static -Wl,-gc-sections  -LD:/PRIZMSDK/lib -lgcc -lfxcg -o D:/PRIZMSDK/projects/example/asm.bin
..\..\bin\sh3eb-elf-objdump asm.bin -d -t > asmmain.o.txt
main.s