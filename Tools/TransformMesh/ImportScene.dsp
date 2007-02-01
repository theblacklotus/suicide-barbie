# Microsoft Developer Studio Project File - Name="ImportScene" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ImportScene - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ImportScene.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ImportScene.mak" CFG="ImportScene - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ImportScene - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "ImportScene - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ImportScene - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\obj\debug\ImportScene\"
# PROP Intermediate_Dir "..\..\obj\debug\ImportScene\"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /I "..\..\include\\" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "K_PLUGIN" /D "K_FBXSDK" /D "K_NODLL" /FR /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 user32.lib advapi32.lib fbxsdk_mt.lib zlib_mt.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\bin\ImportScene.exe" /pdbtype:sept /libpath:"..\..\lib\\"

!ELSEIF  "$(CFG)" == "ImportScene - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ImportScene___Win32_Release"
# PROP BASE Intermediate_Dir "ImportScene___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\obj\release\ImportScene\"
# PROP Intermediate_Dir "..\..\obj\release\ImportScene\"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /Gm /GX /ZI /Od /I "..\..\include\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "K_PLUGIN" /D "K_FBXSDK" /FR /FD /GZ /c
# ADD CPP /nologo /MT /W4 /Gm /GX /ZI /Od /I "..\..\include\\" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "K_PLUGIN" /D "K_FBXSDK" /D "K_NODLL" /FR /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib fbxsdk.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\bin\ImportScene.exe" /pdbtype:sept /libpath:"..\..\lib\\"
# ADD LINK32 zlib_mt.lib user32.lib advapi32.lib fbxsdk_mt.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\bin\ImportScene.exe" /pdbtype:sept /libpath:"..\..\lib\\"

!ENDIF 

# Begin Target

# Name "ImportScene - Win32 Debug"
# Name "ImportScene - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Common\Common.cxx
# End Source File
# Begin Source File

SOURCE=.\DisplayAnimation.cxx
# End Source File
# Begin Source File

SOURCE=.\DisplayCamera.cxx
# End Source File
# Begin Source File

SOURCE=.\DisplayCommon.cxx
# End Source File
# Begin Source File

SOURCE=.\DisplayGlobalSettings.cxx
# End Source File
# Begin Source File

SOURCE=.\DisplayHierarchy.cxx
# End Source File
# Begin Source File

SOURCE=.\DisplayLight.cxx
# End Source File
# Begin Source File

SOURCE=.\DisplayLink.cxx
# End Source File
# Begin Source File

SOURCE=.\DisplayMarker.cxx
# End Source File
# Begin Source File

SOURCE=.\DisplayMaterial.cxx
# End Source File
# Begin Source File

SOURCE=.\DisplayMesh.cxx
# End Source File
# Begin Source File

SOURCE=.\DisplayNurb.cxx
# End Source File
# Begin Source File

SOURCE=.\DisplayPatch.cxx
# End Source File
# Begin Source File

SOURCE=.\DisplayPivotsAndLimits.cxx
# End Source File
# Begin Source File

SOURCE=.\DisplayPose.cxx
# End Source File
# Begin Source File

SOURCE=.\DisplayShape.cxx
# End Source File
# Begin Source File

SOURCE=.\DisplaySkeleton.cxx
# End Source File
# Begin Source File

SOURCE=.\DisplayTexture.cxx
# End Source File
# Begin Source File

SOURCE=.\DisplayUserProperties.cxx
# End Source File
# Begin Source File

SOURCE=.\main.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\Common.h
# End Source File
# Begin Source File

SOURCE=.\DisplayAnimation.h
# End Source File
# Begin Source File

SOURCE=.\DisplayCamera.h
# End Source File
# Begin Source File

SOURCE=.\DisplayCommon.h
# End Source File
# Begin Source File

SOURCE=.\DisplayGlobalSettings.h
# End Source File
# Begin Source File

SOURCE=.\DisplayHierarchy.h
# End Source File
# Begin Source File

SOURCE=.\DisplayLight.h
# End Source File
# Begin Source File

SOURCE=.\DisplayLink.h
# End Source File
# Begin Source File

SOURCE=.\DisplayMarker.h
# End Source File
# Begin Source File

SOURCE=.\DisplayMaterial.h
# End Source File
# Begin Source File

SOURCE=.\DisplayMesh.h
# End Source File
# Begin Source File

SOURCE=.\DisplayNurb.h
# End Source File
# Begin Source File

SOURCE=.\DisplayPatch.h
# End Source File
# Begin Source File

SOURCE=.\DisplayPivotsAndLimits.h
# End Source File
# Begin Source File

SOURCE=.\DisplayPose.h
# End Source File
# Begin Source File

SOURCE=.\DisplayShape.h
# End Source File
# Begin Source File

SOURCE=.\DisplaySkeleton.h
# End Source File
# Begin Source File

SOURCE=.\DisplayTexture.h
# End Source File
# End Group
# End Target
# End Project
