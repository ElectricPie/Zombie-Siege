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
   - Download and install FMOD for Unreal 2.03.07 from the [FMOD website](https://www.fmod.com/download).
   - Place the FMODStudio and FMODStudioNiagara into ZombieSiege/Plugins.

### Generate Project Files:
1. Right-click on the ZombieSiege.uproject file and select Generate Visual Studio project files.
Alternatively, run the following command in the project directory:
UnrealVersionSelector.exe /projectfiles ZombieSiege.uproject

2. Open the generated .sln file in Visual Studio or open the ZombieSiege.uproject file in Unreal Engine 5.5.

3. **Build the Project**:
- In Visual Studio: Set the build configuration to Development Editor.
- In Unreal Engine: Open the project and let it compile the code automatically.

Launch the game by clicking the Play button in Unreal Engine.