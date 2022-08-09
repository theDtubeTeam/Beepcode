@echo off
echo 'src("func.c")'
g++ -I include -shared -c src/func.c -lmingw32
echo 'linker("func.o")'
g++ -I include -shared func.o -o beepcode.dll
echo 'src("main.c")'
g++ -I include -L./ src/main.c res/resource.res -o beepcode -lbeepcode
pause