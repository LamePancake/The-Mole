The Mole is a zany 2D platformer starring Borin the mole. Navigate your way through a selection of classic LamePancake environments as you work to defeat the Grays and lead the Vikings to victory.


To build:
1.	Set Configuration to "x86" instead of (the default) "x64"
2.	Right-click TheMole (project, not Solution) -> Properties.
3.	Expand "Linker" tab -> Select "Input"
4.	Click on the drop-down arrow for "Additional Dependencies"
5.	Add SDL2.lib;SDL2main.lib;SDL2_image.lib;SDL2_ttf.lib;SDL2_mixer.lib;SDL2pp.lib;%(AdditionalDependencies)
6.	Select "VC++ Directories" tab
7.	Set "Library Directories" to $(VC_LibraryPath_x86);$(WindowsSDK_LibraryPath_x86);$(NETFXKitsDir)Lib\um\x86;$(SolutionDir)Lib\SDL2pp\Debug;$(SolutionDir)Lib\SDL2;
	[]	In Release mode, change $(SolutionDir)Lib\SDL2pp\Debug; to $(SolutionDir)Lib\SDL2pp\RelWithDebInfo; if trying to debug or $(SolutionDir)Lib\SDL2pp\Release; for an actual deployment
8.	Select "Debugging" tab
9.	Set "Working Directory" to $(SolutionDir)
10.	Set "Environment" to PATH=$(SolutionDir)Lib\SDL2pp\Debug;$(SolutionDir)Lib\SDL2;$(Path);
11.	Repeat 8-11 for Release and Debug
	[]	Change "$(SolutionDir)Lib\SDL2pp\Debug" to "$(SolutionDir)Lib\SDL2pp\RelWithDebInfo" for debugging a Release build or "$(SolutionDir)Lib\SDL2pp\Release" for a deployment build
    
    $(VC_LibraryPath_x86);$(WindowsSDK_LibraryPath_x86);$(NETFXKitsDir)Lib\um\x86;$(SolutionDir)Lib\SDL2pp\Release;$(SolutionDir)Lib\SDL2;