# Zombie Siege

## Building the Project with Unreal Engine 5.5
Follow these steps to build and run the Zombie Siege project using Unreal Engine 5.5:

### Prerequisites
1. **Unreal Engine 5.5**:
    - Download and install Unreal Engine 5.5 from the [Epic Games Launcher](https://www.unrealengine.com/).

2. **Visual Studio**:
    - Install Visual Studio 2022 with the following components:
        - **Desktop development with C++**
        - **Game development with C++**
        - Ensure you have the latest version of the **MSVC toolset** and **Windows SDK**.

3. **FMOD**:
   - Download and install FMOD for Unreal 2.03.06 from the [FMOD website](https://www.fmod.com/download), you will need an FMOD account to download.
   - Place the `FMODStudio` and `FMODStudioNiagara` into `ZombieSiege/Plugins`.
     - Overwrite the existing files.

### Running the Project
You can either run the uproject file or run the project from the .sln file in you IDE which will require generating project files (See below).

### Generate Project Files:
1. Right-click on the `ZombieSiege.uproject` file and select Generate Visual Studio project files.
Alternatively, run the following command in the project directory:
UnrealVersionSelector.exe `/projectfiles Z[.gitignore](.gitignore)ombieSiege.uproject`

2. Open the generated .sln file in Visual Studio or open the ZombieSiege.uproject file in Unreal Engine 5.5.

3. **Build the Project**:
- In Visual Studio: Set the build configuration to Development Editor.
- In Unreal Engine: Open the project and let it compile the code automatically.

### Building FMOD Banks
To ensure FMOD audio works correctly in the Zombie Siege project you may need to reopen the Unreal project after its initial running or build the FMOD banks:

1. **Install FMOD Studio**:
   - Download and install FMOD Studio 2.03.06 from the [FMOD website](https://www.fmod.com/download), you will need an FMOD account to download.

2. **Open the FMOD Project**:
   - Open FMOD project file `ZombieSiege_FMODProject.fspro` in FMOD Studio found in the `ZombieSiege/ZombieSiege_FMODProject` directory.
     
3. **Set the built banks directory in FMOD Studio**
   - This should already be set to `../Content/FMOD` but if not:
     - Go to `Edit` > `Preferences` > `Build`.
     - Set the `Build banks output directory` to `../Content/FMOD`.

4. **Build the Banks**:
   - Go to the `File` menu and select `Build...`.

5. **Verify the Banks**:
   - After building, ensure the `.bank` files are present in the `ZombieSiege/Content/FMOD/Desktop` directory.

6. **Reload FMOD in Unreal Engine**:
   - Open the Zombie Siege project in Unreal Engine.
   - In Unreal click `Reload Banks` in `File` settings to ensure the latest banks are loaded.

This process ensures that all FMOD audio assets are correctly built and integrated into the project.
