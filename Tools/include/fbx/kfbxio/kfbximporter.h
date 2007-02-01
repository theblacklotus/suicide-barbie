/*!  \file kfbximporter.h
 */

#ifndef _FBXSDK_IMPORTER_H_
#define _FBXSDK_IMPORTER_H_

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

#include <kfbxio/kfbxio.h>
			
#include <klib/kstring.h>
#include <klib/karrayul.h>

#include <kbaselib_forward.h>
#ifndef MB_FBXSDK
#include <kbaselib_nsuse.h>
#endif

#ifdef KARCH_DEV_MACOSX_CFM
	#include <CFURL.h>
	#include <Files.h>
#endif

#include <kfbxobject/kfbxobject_forward.h>
#ifndef MB_FBXSDK
#include <kfbxobject/kfbxobject_nsuse.h>
#endif

#include <fbxfilesdk_nsbegin.h>

class KFbxScene;
class KFbxImporterImp;
class KFbxStreamOptionsFbx;
class KFbxTakeInfo;
class KFbxSceneInfo;
class KFbxStreamOptionsFbxReader;


/** \brief Class to import an FBX file into SDK objects.
  * \nosubgrouping
  *	Typical workflow for using the KFbxImporter class:
  *		-# create an importer
  *		-# initialize the importer with a file name
  *	    -# set numerous states, take information, defining how the importer will behave
  *		-# call KFbxImporter::Import() with an empty scene
  */
class KFBX_DLL KFbxImporter : public KFbxIO
{
	KFBXOBJECT_DECLARE(KFbxImporter);
public:
	/** 
	  * \name Import Functions
	  */
	//@{

	/** Initialize object.
	  *	\param pFileName     Name of file to access.
	  *	\return              \c true on success, \c false otherwise.
	  * \remarks             To identify the error that occurred, call KFbxIO::GetLastErrorID().	  
	  */
	virtual bool Initialize(const char *pFileName);

#ifdef KARCH_DEV_MACOSX_CFM
    virtual bool Initialize(const FSSpec &pMacFileSpec);
    virtual bool Initialize(const FSRef &pMacFileRef);
    virtual bool Initialize(const CFURLRef &pMacURL);
#endif

	/** Get the FBX version number of the FBX file.
	  * FBX version numbers start at 5.0.0.
	  * \param pMajor        Version major number.
	  * \param pMinor        Version minor number.
	  * \param pRevision     Version revision number.
	  *	\remarks             This function must be called after KFbxImporter::Initialize().	  
	  */
	void GetFileVersion(int& pMajor, int& pMinor, int& pRevision);

	/**	Get the default rendering resolution if present in the file header.
	  * \param pCamName            Returned name of the camera.
	  * \param pResolutionMode     Returned resolution mode.
	  * \param pW                  Returned width.
	  * \param pH                  Returned height.
	  * \return                    \c true if the default rendering settings are defined in the file, otherwise
	  *                            returns \c false with empty parameters.
	  */
	bool GetDefaultRenderResolution(KString& pCamName, KString& pResolutionMode, double& pW, double& pH);

	/** \enum EStreamOptionsGeneration Stream options identifiers.
	  * - \e eSTREAMOPTIONS_PARSE_FILE
	  * - \e eSTREAMOPTIONS_FAST		Do not parse the file.
	  * - \e eSTREAMOPTIONS_COUNT
	  */
	typedef enum 
	{
		eSTREAMOPTIONS_PARSE_FILE,
		eSTREAMOPTIONS_FAST,        // Do not parse the file
		eSTREAMOPTIONS_COUNT
	} EStreamOptionsGeneration;

    /** Read the currently opened file header to retrieve information related to takes.
	  * \param pStreamOptionsGeneration     Stream options identifier.
	  *	\return                             Pointer to file import options or \c NULL on failure.
	  * \remarks                            Caller gets ownership of the returned structure.
      */
	KFbxStreamOptions* GetImportOptions(EStreamOptionsGeneration pStreamOptionsGeneration = eSTREAMOPTIONS_PARSE_FILE);	

    /** Read the currently opened file header to retrieve information related to takes.
	  * \param pFbxObject     Target FBX file.
	  *	\return               Pointer to stream import options or \c NULL on failure.
	  * \remarks              Caller gets ownership of the returned structure.
      */
	KFbxStreamOptions* GetImportOptions(IFbxObject* pFbxObject);

    /** Import the currently opened file into a scene. 
      * \param pScene          Scene to fill with file content.
	  * \param pStreamOptions  Pointer to file import options.
	  *	\return                \c true on success, \c false otherwise.
	  * \remarks               To identify the error that occurred, call KFbxIO::GetLastErrorID().
	  *                        If the imported file is password protected and the password is not
	  *                        set or wrong, function KFbxIO::GetLastErrorID() returns 
	  *                        KFbxIO::ePASSWORD_ERROR.
      */
    bool Import(KFbxScene& pScene, KFbxStreamOptions* pStreamOptions = NULL);	

    /** Import the FBX file into a scene. 
      * \param pScene              Scene to fill with file content.
	  * \param pStreamOptions      Pointer to file import options.
	  * \param pFbxObject          Source FBX file.
	  *	\return                    \c true on success, \c false otherwise.
	  * \remarks                   To identify the error that occurred, call KFbxIO::GetLastErrorID().
	  *                            If the imported file is password protected and the password is not
	  *                            set or wrong, function KFbxIO::GetLastErrorID() returns 
	  *                            KFbxIO::ePASSWORD_ERROR.
      */
	bool Import(KFbxScene& pScene, KFbxStreamOptions* pStreamOptions, IFbxObject* pFbxObject);

    /** Release the file import options. 
	  * \param pStreamOptions     Pointer to file import options.
	  */
	void ReleaseImportOptions(KFbxStreamOptions* pStreamOptions);

	//@}

	/** Set the password.
	  * All subsequently imported files are opened with the given password.
      * \param pPassword     Password string.
      */
    void SetPassword(char* pPassword);

	/** 
	  * \name Take Description Access
	  */
	//@{
	
	/** Get the number of available takes in the file.
	  * \return      Number of takes.
	  *	\remarks     This function must be called after KFbxImporter::Initialize().
	  */
	int GetTakeCount();

	/** Get the take information about an available take.
	  * Use the returned reference to a KFbxTakeInfo object to set whether the indexed take is imported.
	  *	\param pIndex     Index of the requested take.
	  *	\return           Take information or \c NULL if function failed.
	  *	\remarks          This function must be called after KFbxImporter::Initialize().
	  */
	KFbxTakeInfo* GetTakeInfo(int pIndex);

	/** Return the current take name.
	  *	\return     Current take name if there is one, otherwise returns an empty string.
	  *	\remarks    This function must be called after KFbxImporter::Initialize().
	  */
	char* GetCurrentTakeName();

	//@}

	/** 
	  * \name Scene Description Access
	  */
	//@{

	/** Get the scene info.
	  * \return     Pointer to the scene info or \c NULL if no scene information
	  *             is available in the file.
	  */
	KFbxSceneInfo* GetSceneInfo();

	//@}
	/** 
	  * \name File Format
	  */
	//@{

	/** Set the imported file format.
	  *	\param pFileFormat     File format identifier.
	  */
	void SetFileFormat (int pFileFormat);

	/** Get the format of the imported file.
	  *	\return     File format identifier.
	  */
	int GetFileFormat ();

	/** \return     \c true if the file format is a recognized FBX format.
	  */
	bool IsFBX();
	//@}

///////////////////////////////////////////////////////////////////////////////
//
//  WARNING!
//
//	Anything beyond these lines may not be documented accurately and is 
// 	subject to change without notice.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef DOXYGEN_SHOULD_SKIP_THIS

		bool UpdateImportOptions(KFbxStreamOptions* pStreamOptions);

protected:


	void SetImportOptionsInfo(KFbxStreamOptionsFbxReader* pStreamOptionsFbx);
	void GetImportOptionsInfo(KFbxStreamOptionsFbxReader* pStreamOptionsFbx);

	KFbxImporter(KFbxSdkManager& pManager,char const* pName);
	virtual ~KFbxImporter();

	void Reset();

    bool FileOpen();
	void FileClose();


    bool IsNativeExtension ();

	KFbxImporterImp* mImp;

	bool mImportOptionsDone;
	KArrayTemplate<KFbxTakeInfo*> mTakeInfo;
	KFbxSceneInfo* mSceneInfo;
	KString mCurrentTakeName;
	int mMajorVersion;
	int mMinorVersion;
	int mRevisionVersion;

	// Used to retrieve the default render resolution
	// stored in the extended header
	bool    mDefaultCameraResolutionIsOK;
	KString mDefaultCameraName;
	KString mDefaultResolutionMode;
	double mDefaultResolutionW;
	double mDefaultResolutionH;

#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS

};

#include <fbxfilesdk_nsend.h>

#endif // #ifndef _FBXSDK_IMPORTER_H_


