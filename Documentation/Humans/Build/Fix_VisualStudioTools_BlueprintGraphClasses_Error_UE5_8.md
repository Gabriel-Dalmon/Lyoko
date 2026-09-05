# Fix `BlueprintGraphClasses.h` Error in VisualStudioTools (UE 5.8)

## Issue

In Unreal Engine 5.8, the **VisualStudioTools** plugin may fail to compile with a `C1083` error related to the missing `BlueprintGraphClasses.h` file.

## Solution

### 1. Install the Plugin to Your Project

In Visual Studio, choose **Install to Project** for the VisualStudioTools plugin instead of **Install to Engine**.

This places the plugin source directly inside your project's `Plugins` folder, allowing it to be modified and compiled with your project.

### 2. Update the Include File

Open:

```
VisualStudioToolsBlueprintBreakpointExtension.cpp
```

Located in the plugin's `Private` folder.

At the top of the file, replace:

```cpp
#include "BlueprintGraphClasses.h"
```

with:

```cpp
#include "K2Node_CallFunction.h"
```

### 3. Regenerate Project Files

Right-click your `.uproject` file and select:

```
Generate Visual Studio project files
```

This updates the build configuration with the modified plugin source.

### 4. Rebuild the Project

Open the generated Visual Studio solution and rebuild the project.

The VisualStudioTools plugin should now compile successfully.

## Optional: Move the Compiled Plugin

If required, copy the compiled plugin from your project's `Plugins` folder back into the Unreal Engine Marketplace plugin directory.
