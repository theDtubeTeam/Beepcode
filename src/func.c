#include <func.h>
#define NOMINMAX
// TODO:
//     All done!
//
//
//
//
//
//
//
//
#include <bits/stdc++.h>
#include <windows.h> // For loading the InpOut32.dll
#include <stdio.h>   // It's for standard(std) io (io) functions. YOU SHOULD KNOW THAT BY NOW!
#include <string>    // Used to convert the integers from the .beep file from strings to ints
#include <iostream>  // I forgot the purpose this library serves
#include <math.h>    // Do your math kids.
#include <fstream>   // FileStream
#include <cstring>   // 
#include <vector>    //
#include <cstdlib>   //
#include <signal.h>  //
#include <unistd.h>  //

using namespace std;

//start of InpOut32.dll function list
typedef void	(__stdcall *lpOut32)(short, short);
typedef short	(__stdcall *lpInp32)(short);
typedef BOOL	(__stdcall *lpIsInpOutDriverOpen)(void);
typedef BOOL	(__stdcall *lpIsXP64Bit)(void);
lpOut32 gfpOut32;
lpInp32 gfpInp32;
lpIsInpOutDriverOpen gfpIsInpOutDriverOpen;
lpIsXP64Bit gfpIsXP64Bit;
//end of InpOut32.dll function list
static void keyLight(int nKey)
{
	while(gfpInp32(0x64) & 0x2)
	{
		gfpOut32(0x60, 0xed);
		gfpOut32(0x60, nKey);
    }
}
static void Beep(int nFrequence) {
	int Div;
	int tmp;
	keyLight(2);
 
		//Set the PIT to the desired frequency
	Div = 1193180 / nFrequence;
	gfpOut32(0x43, 0xb6);
	gfpOut32(0x42, (uint8_t) (Div) );
	gfpOut32(0x42, (uint8_t) (Div >> 8));
 
		//And play the sound using the PC speaker
	tmp = gfpInp32(0x61);
	if (tmp != (tmp | 3)) {
 		gfpOut32(0x61, tmp | 3);
	}
}
void StopBeep()
{
	gfpOut32(0x61, (gfpInp32(0x61) & 0xFC)); // BEEEE-- *SLAP* SHUT THE F*(K UP
}
void signal_callback_handler(int signum) {
   cout << "Ctrl+C pressed, Exit code:" << signum << endl;
   // Terminate program
   StopBeep();
   exit(2);
}
bool isNumber(const string& str)
{
    for (char const &c : str) {
        if (std::isdigit(c) == 0) {
			return false;
		}
    }
    return true;
}
int removeSpecialCharacter(string s)
{
	std::string x = s;
    for (int i = 0; i < s.size(); i++) {
         
        // Finding the character whose
        // ASCII value fall under this
        // range
        if (s[i] < '0' || s[i] > '9')
        {  
            // erase function to erase
            // the character
            s.erase(i, 1);
            i--;
        }
	}
    for (int z = 0; z < x.size(); z++) {
         
        // Finding the character whose
        // ASCII value fall under this
        // range
        if (x[z] < 'A' || x[z] > 'Z' &&
			x[z] < 'a' || x[z] > 'z')
        {  
            // erase function to erase
            // the character
            x.erase(z, 1);
            z--;
        }
    }
	if(x.c_str() != " " || "")
	{
		printf("%s\n", x.c_str());
	}
    return stoi(s.c_str());
}
int playFile(char* finm)
{
	signal(SIGINT, signal_callback_handler);
    if(finm == NULL) // if the arguments were null then we print the information
    {
		printf("Usage: beepcode.exe [filename]\nUse --help for more info\n");
    }
	else if(finm == std::string("--help"))
	{
		printf("Usage: beepcode.exe [filename]\nConverting midi files - Use index.html in the 'midi-beepcode-master' folder to convert midi files");
	}
	else
	{
		//Dynamically load the DLL at runtime (not linked at compile time)
		HINSTANCE hInpOutDll ;
		hInpOutDll = LoadLibrary ( "InpOut32.DLL" ) ;	//The 32bit DLL. If we are building x64 C++ 
														//applicaiton then use InpOutx64.dll
		if ( hInpOutDll != NULL )
		{
			gfpOut32 = (lpOut32)GetProcAddress(hInpOutDll, "Out32");
			gfpInp32 = (lpInp32)GetProcAddress(hInpOutDll, "Inp32");
			gfpIsInpOutDriverOpen = (lpIsInpOutDriverOpen)GetProcAddress(hInpOutDll, "IsInpOutDriverOpen");
			gfpIsXP64Bit = (lpIsXP64Bit)GetProcAddress(hInpOutDll, "IsXP64Bit");

			if (gfpIsInpOutDriverOpen())
			{
				 printf("Playing %s%s", finm, ",\nPress Ctrl+C to stop\n");
				 int freq; // The Current frequency of the note
				 int dur;  // The Current duration of the note
				 int lico = 0; // The current line counter
				 int curl = 0; // The second line counter for comments/lyrics
				 FILE * fp; // the beep file
				 char * line = NULL; // the line
				 size_t len = 0; // a length
				 ssize_t read; // somthing
				 fp = fopen(finm, "r"); // Open the file with the requested file name
				 if (fp == NULL) // File was null so return an error
					 exit(EXIT_FAILURE);
				 while ((read = getline(&line, &len, fp)) != -1) {
					 lico++;
					 curl++;
					 if(lico == 1)
					 {
						 //printf("%s", line);
						 std::string ltc  = line; // Converts the char* to std::string
						 //printf("Current line: %d\n", curl);
						 freq = removeSpecialCharacter(ltc); // Removes the comments
					 }
					 if(lico == 2)
					 {
						 std::string ltc  = line; // Converts the char* to std::string
						 //printf("Current line: %d\n", curl);
						 dur = removeSpecialCharacter(ltc); // Removes the comments						 //printf("Dur: %d\n", dur);
						 //printf("Freq: %d\n", freq);
						 if(freq == 0) // FIX: If beep() tried to play a 0hz tone it will crash.
						 {
							 Sleep(dur); // So we just sleep for the duration of the 0hz tone
						 }
						 else // But if it was not zero we must beep!
						 {
							 Beep(freq); // Beep
							 Sleep(dur); // sleep
						 }
						 // DEBUG:
						 //Uncomment the below line if your speaker doesent shut the fuck up when we hit ctrl+c
						 //
						 //StopBeep();

						 lico = 0; // Set the Line Counter to 0 to repeat this function until the file is done.
					 }
					 StopBeep(); // Tell the PC Speaker to SHUT UP (Developing this was hell)
				 }

				 fclose(fp);
				 if (line)
						free(line);

			}
			else
			{
				printf("Unable to open InpOut32 Driver!\n");
			}

			//All done
			FreeLibrary( hInpOutDll ); // Be a good boy and free the wild dlls
			return 0;
		}
		else // Yea um boss, We can't seem to find the file in our records.
		{
			printf("Unable to load InpOut32 DLL!\n");
			return -1;
		}
	}
	return -2;
}
int checkArgv(char* arg, const char* what)
{
	if(arg == NULL)
	{
		return 7;
	}
	else if(arg == std::string(what))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}