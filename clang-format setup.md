# Automated source code formatting
In order to improve code quality it is recommended to use automatic code formatter. The one chosen for this code base is clang-format. You can download the utility suit that contains the formatter from [LLVM website](https://releases.llvm.org/download.html).  
[Direct link to v13.0.0 github repo](https://github.com/llvm/llvm-project/releases/tag/llvmorg-13.0.0).  
When installing, note the location on disk where the LLVM toochain is installed.  
# Eclipse modifications
Certain mods need to be done to CubeIDE in order to make it use clang-format. Open CubeIDE. 
## Install CppStyle
Within CubeIDE:  
1. Help -> Eclipse Marketplace...
2. In Search panel type "CppStyle"
3. If not already installed click _Install_
Wait until plugin is installed and then restart CubeIDE.
## Configure CppStyle
After restarting CubeIDE:
1. Windows -> Preferences -> C/C++ -> CppStyle
2. In _Clang-format path:_ dialog either paste the path to clang-format.exe utility or browse and select it
3. Tick box _Run clang-format on file save_

This is it. You should now have a working autoformatter for your source code. The specific settings of how code is formatted are controlled by the file _.clang-format_ in the root of the project.  
__Disclaimer:__ If you have any dislikes or concerns about how code formatter works, please, raise them with code owners. However, bear in mind that these rules have been polished over years of combined experience. Consider whether your objection is just your personal dislike or an actual objective shortcoming of the format. 
