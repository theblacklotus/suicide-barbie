/*!  \file fbxlogfile.h
 */

#ifndef _FBXSDK_LOG_FILE_H_
#define _FBXSDK_LOG_FILE_H_

/**************************************************************************************

 Copyright © 2000 - 2006 Autodesk, Inc. and/or its licensors.
 All Rights Reserved.

 The coded instructions, statements, computer programs, and/or related material
 (collectively the "Data") in these files contain unpublished information
 proprietary to Autodesk, Inc. and/or its licensors, which is protected by
 Canada and United States of America federal copyright law and by international
 treaties.

 The Data may not be disclosed or distributed to third parties, in whole or in
 part, without the prior written consent of Autodesk, Inc. ("Autodesk").

 THE DATA IS PROVIDED "AS IS" AND WITHOUT WARRANTY.
 ALL WARRANTIES ARE EXPRESSLY EXCLUDED AND DISCLAIMED. AUTODESK MAKES NO
 WARRANTY OF ANY KIND WITH RESPECT TO THE DATA, EXPRESS, IMPLIED OR ARISING
 BY CUSTOM OR TRADE USAGE, AND DISCLAIMS ANY IMPLIED WARRANTIES OF TITLE,
 NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE OR USE.
 WITHOUT LIMITING THE FOREGOING, AUTODESK DOES NOT WARRANT THAT THE OPERATION
 OF THE DATA WILL BE UNINTERRUPTED OR ERROR FREE.

 IN NO EVENT SHALL AUTODESK, ITS AFFILIATES, PARENT COMPANIES, LICENSORS
 OR SUPPLIERS ("AUTODESK GROUP") BE LIABLE FOR ANY LOSSES, DAMAGES OR EXPENSES
 OF ANY KIND (INCLUDING WITHOUT LIMITATION PUNITIVE OR MULTIPLE DAMAGES OR OTHER
 SPECIAL, DIRECT, INDIRECT, EXEMPLARY, INCIDENTAL, LOSS OF PROFITS, REVENUE
 OR DATA, COST OF COVER OR CONSEQUENTIAL LOSSES OR DAMAGES OF ANY KIND),
 HOWEVER CAUSED, AND REGARDLESS OF THE THEORY OF LIABILITY, WHETHER DERIVED
 FROM CONTRACT, TORT (INCLUDING, BUT NOT LIMITED TO, NEGLIGENCE), OR OTHERWISE,
 ARISING OUT OF OR RELATING TO THE DATA OR ITS USE OR ANY OTHER PERFORMANCE,
 WHETHER OR NOT AUTODESK HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH LOSS
 OR DAMAGE.

**************************************************************************************/

#include <kaydaradef.h>
#ifndef KFBX_DLL 
	#define KFBX_DLL K_DLLIMPORT
#endif

#include <klib/kstring.h>
#include <klib/kstringlist.h>

#ifndef MB_FBXSDK
#include <kbaselib_nsuse.h>
#endif

#include <fbxfilesdk_nsbegin.h>

// Not part of the public FBX SDK
#ifndef K_FBXSDK

/***********************************************************************
	CLASS FbxLogFile

	Description:

	  Log file allowing the writing of information to a text file
	  created at the destruction of the instance.

	TODO: 

	  Add old log content automatic deletion

************************************************************************/

class KFBX_DLL FbxLogFile
{
public:
	// Constructor.
	FbxLogFile(char* iFileName, 
			   char* iSessionDescription, 
			   bool iAppend = true,
			   bool iOutputLogOnDelete = true);
	
	// Destructor.  
	~FbxLogFile();

	// Writes the content to the log file
	void OutputLog(int pFileNo = -1);

	// Add a text line to the log
	FbxLogFile& operator+= (char* iLogString);

	// Accessor to the filename string 
	KString GetFileName() const { return mFileName; }

private:
	
	KString mFileName;
	KString mSessionDescription;
	bool mAppend;
	bool mOutputLogOnDelete;
	KStringList mLog;
	
};

#endif // #ifndef K_FBXSDK

#include <fbxfilesdk_nsend.h>

#endif // #ifndef _FBXSDK_LOG_FILE_H_


