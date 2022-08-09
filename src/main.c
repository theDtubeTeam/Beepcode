#include <stdio.h>
#include <func.h>

int main(int argc, char* argv[])
{
	int arg1 = checkArgv(argv[1], "friends.beep");
	playFile(argv[1]);
}