/*!  \file kfbxuserparams.h
 */

#ifndef _FBXSDK_USER_PARAMS_H_
#define _FBXSDK_USER_PARAMS_H_

/**************************************************************************************

 Copyright © 2001 - 2006 Autodesk, Inc. and/or its licensors.
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

#include <klib/karrayul.h>
#include <klib/kstring.h>

#ifndef MB_FBXSDK
#include <kbaselib_nsuse.h>
#endif
#include <kbaselib_forward.h>

// Not part of the public FBX SDK
#ifndef K_FBXSDK

#include <fbxfilesdk_nsbegin.h>

//
// TOKEN Definition
//

#define TOKEN_USER_PARAMS	"UserParams"

#define TOKEN_NO_SCALE_CONVERSION -1

enum EParamID
{
	// Internal use
	eEXPORT_MASTERUNITSTRING,
	eIMPORT_MASTERUNITSTRING,
	eIMPORT_MASTERFRAMERATESTRING,
    eIMPORT_USEMATRIXFROM_BINDPOSE,

	// common
	eSHOW_WARNING_DIALOG,
	eASCII_FBX,

	// Export
	eEXPORT_MASTERSCALE,
	eEXPORT_SAMPLINGRATE,
	eEXPORT_ANIMATION,
	eEXPORT_EMBEDTEXTURES,
	eEXPORT_CONVERT_2TIFF,
	eEXPORT_CAMERAS,
	eEXPORT_LIGHTS,
	eEXPORT_GEOMETRIES,
	eEXPORT_SKINS,
	eEXPORT_SHAPES,
	eEXPORT_BINDPOSE,
	eEXPORT_PIVOTSTONULL,
	eFBX_EXPORT_FILE_VERSION,
	eEXPORT_QUATROT,
	eEXPORT_UNIT,

	// filters
	eEXPORT_CONSTANTKEYREDUCER,
	eEXPORT_CONSTANTKEYREDUCER_OTHERS_PRECISION,
	eEXPORT_CONSTANTKEYREDUCER_TRANSLATION_PRECISION,
	eEXPORT_CONSTANTKEYREDUCER_ROTATION_PRECISION,
	eEXPORT_CONSTANTKEYREDUCER_SCALING_PRECISION,
	eEXPORT_KEYREDUCER,
	eEXPORT_KEYREDUCER_PRECISION,
	eEXPORT_KEYSYNC,
	eEXPORT_KEYSONFRAME,
	eEXPORT_UNROLL,
	eEXPORT_UNROLL_QUALITY,
	eEXPORT_UNROLL_PATH,


	// Import
	eIMPORT_LOADMODE,
	eIMPORT_MASTERSCALE,
	eIMPORT_SAMPLINGRATE,
	eIMPORT_ANIMATION,
	eIMPORT_CAMERAS,
	eIMPORT_LIGHTS,
	eIMPORT_GEOMETRIES,
	eIMPORT_SKINS,
	eIMPORT_SHAPES,
	eIMPORT_MARKERS,
	eIMPORT_NULLSTOPIVOT,
	eIMPORT_QUATROT,
	eFBX_IMPORT_FRAMERATE_SOURCE,
	eIMPORT_UNIT,

	// filters
	eIMPORT_CONSTANTKEYREDUCER,
	eIMPORT_CONSTANTKEYREDUCER_OTHERS_PRECISION,
	eIMPORT_CONSTANTKEYREDUCER_TRANSLATION_PRECISION,
	eIMPORT_CONSTANTKEYREDUCER_ROTATION_PRECISION,
	eIMPORT_CONSTANTKEYREDUCER_SCALING_PRECISION,
	eIMPORT_KEYREDUCER,
	eIMPORT_KEYREDUCER_PRECISION,
	eIMPORT_KEYSYNC,
	eIMPORT_KEYSONFRAME,
	eIMPORT_UNROLL,
	eIMPORT_UNROLL_QUALITY,
	eIMPORT_UNROLL_PATH,

	ePACKAGE_SPECIFIC_BASE // derived classes need to start their ID from here
};

class KFbxUserParamEntry
{
public:
	enum ETypeID
	{
		eBOOL,
		eDOUBLE,
		eSTRING
	};

	KFbxUserParamEntry(KString pToken, ETypeID pType, KString pValue, int pID) :
		mToken(pToken),
		mType(pType),
		mValue(pValue),
		mID(pID)
	{
		// nothing else to do!
	}

	KString GetToken() { return mToken; }
	ETypeID GetType()  { return mType;  }
	KString GetValue() { return mValue; }
	int     GetID()    { return mID;    }

	void    SetValue(KString pVal) { mValue = pVal; }

private:
	KString mToken;
	ETypeID mType;
	KString mValue;
	int     mID;
};


/** KFbxUserParams
*	Class used to store and access user defined parameters.
*/
class KFBX_DLL KFbxUserParams
{
public:

	//! Destructor
	virtual ~KFbxUserParams();

	//! Reset all the params (both import & export) to their default value.
	virtual void Reset();

	//! Load the parameters from a resource file.
	virtual void Load() = 0;

	/** Load the parameters from a resource file.
	*	\param pFile.
	*	\param pBlockName.
	*/
	void Load(KConfigFile& pFile, char* pBlockName);

	//! Save the parameters to a resource file.
	virtual void Save() = 0;

	/** Save the parameters to a resource file.
	*	\param pFile.
	*	\param pBlockName.
	*/
	void Save(KConfigFile& pFile, char* pBlockName) const;

	// =======================================================
	//
	// PARAMETERS
	//
	// =======================================================
	int GetNbParameters() const;

	enum ELoadMode {
		eEXCLUSIVE_MERGE,
		eMERGE,
		eCREATE
	};

	
	enum EQuaternionMode {
		eAS_QUATERNION,
		eAS_EULER,
		eRESAMPLE
	};

	bool    GetBoolParam(kUInt pID)   const;
	double	GetDoubleParam(kUInt pID) const;
	KString GetStringParam(kUInt pID) const;

	void    SetBoolParam(kUInt pID, bool pValue);
	void    SetDoubleParam(kUInt pID, double pValue);
	void	SetStringParam(kUInt pID, KString pValue);

	virtual KString GetMasterUnitStr(bool pImport = false) const;
	virtual double  GetMasterScale(bool pImport = false) const;
	virtual void    SetMasterScale(double pMs, bool pImport = false);
	virtual KString GetMasterFrameRateStr() const;
	virtual double  GetResamplingRate(bool pImport = false) const;
	virtual void    SetResamplingRate(double pNewRate, bool pImport = false);

	KString   LoadModeToString(ELoadMode pLoadMode) const;
	ELoadMode StringToLoadMode(KString& pStr) const;
	
	KString QuaternionModeModeToString(EQuaternionMode pQIM) const;
	EQuaternionMode StringToQuaternionMode(KString& pStr) const;

	enum ESysUnits
	{
		kUNITS_USER,
		kUNITS_INCHES, 
		kUNITS_FEET,
		kUNITS_YARDS,
		kUNITS_MILES,
		kUNITS_MILLIMETERS,
		kUNITS_CENTIMETERS,
		kUNITS_METERS,
		kUNITS_KILOMETERS
	};

	enum ESysFrameRate
	{
		kFRAMERATE_USER,
		kFRAMERATE_HOURS,
		kFRAMERATE_MINUTES,
		kFRAMERATE_SECONDS,
		kFRAMERATE_MILLISECONDS,
		kFRAMERATE_GAMES_15,
		kFRAMERATE_FILM_24,
		kFRAMERATE_PAL_25,
		kFRAMERATE_NTSC_30,
		kFRAMERATE_SHOWSCAN_48,
		kFRAMERATE_PALFIELD_50,
		kFRAMERATE_NTSCFIELD_60
	};


protected:	

	/* Constructor protected because it's a virtual class */
	KFbxUserParams();

	// Returns the entry corresponding to the pID.
	KFbxUserParamEntry* Entry(kUInt pId) const;
	KFbxUserParamEntry* Entry(kUInt pId);

	// Convert a string to a boolean value
	bool GetBool(char* pString) const;
	
	// Convert a string to a boolean value
	double GetDouble(char* pString) const;

	// Convert a boolean to a string
	KString& GetString(bool pFlag, KString& pString) const;

	// Convert a boolean to a string
	KString& GetString(double pValue, KString& pString) const;

	// Allow derived classes to expand the mData array.
	void AddUserParams(KArrayTemplate<KFbxUserParamEntry*>& pUserParams);

	void ComputeMasterScale(bool pImport, bool pDoNotSetMasterScale = true);
	void ComputeMasterFrameRate(bool pImport);

	virtual ESysUnits GetSystemUnitInfo() const = 0;
	virtual double    GetSystemScale(ESysUnits pUnits) const = 0;

	virtual ESysFrameRate GetSystemFrameRate() const = 0;

	virtual void ResetExport();
	virtual void ResetImport();
	
private:
	
	KArrayTemplate<KFbxUserParamEntry*> mData;
};

#include <fbxfilesdk_nsend.h>

#endif // #ifndef K_FBXSDK
#endif // #define _FBXSDK_USER_PARAMS_H_

