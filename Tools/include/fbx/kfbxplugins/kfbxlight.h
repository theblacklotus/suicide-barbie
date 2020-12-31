/*!  \file kfbxlight.h
 */
 
#ifndef _FBXSDK_LIGHT_H_
#define _FBXSDK_LIGHT_H_

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

#include <kaydara.h>

#include <kfbxplugins/kfbxnodeattribute.h>
#include <fbxfilesdk_nsbegin.h>

class KFbxColor;
class KFbxSdkManager;
class KFbxTexture;
class KFbxLight_internal;

/** \brief This node attribute contains methods for accessing the properties of a light.
  * \nosubgrouping
  */
class KFBX_DLL KFbxLight : public KFbxNodeAttribute
{
	KFBXOBJECT_DECLARE(KFbxLight);

public:

	//! Return the type of node attribute which is EAttributeType::eLIGHT.
	virtual EAttributeType GetAttributeType() const;

	/**
	  * \name Light Property Names
	  */
	//@{	
	static const char* sDecayType;
	static const char* sDecayStart;
	static const char* sEnableNearAttenuation;
	static const char* sNearAttenuationStart;
	static const char* sNearAttenuationEnd;
	static const char* sEnableFarAttenuation;
	static const char* sFarAttenuationStart;
	static const char* sFarAttenuationEnd;
	static const char* sCastShadows;
	static const char* sShadowColor;

	//@}

	/**
	  * \name Light Properties
	  */
	//@{

    /** \enum ELightType Light types.
	  * - \e ePOINT
	  * - \e eDIRECTIONAL
	  * - \e eSPOT
	  */
    typedef enum  
    {
	    ePOINT, 
	    eDIRECTIONAL, 
	    eSPOT
    } ELightType;

    /** \enum EDecayType     Decay types. Used for setting the attenuation of the light.
	  * - \e eNONE          No decay. The light's intensity will not diminish with distance.		
	  * - \e eLINEAR        Linear decay. The light's intensity will diminish linearly with the distance from the light.
	  * - \e eQUADRATIC     Quadratic decay. The light's intensity will diminish with the squared distance from the light.
	  *                     This is the most physically accurate decay rate.
	  * - \e eCUBIC         Cubic decay. The light's intensity will diminish with the cubed distance from the light.
	  */
    typedef enum  
    {
	    eNONE = 0,
	    eLINEAR,
	    eQUADRATIC,
		eCUBIC
    } EDecayType;

    /** Set the light type. 
	  * \param pLightType     The light type.
	  */
    void SetLightType(ELightType pLightType);

	/** Get the light type.
	  * \return     The current light type.
	  */
	ELightType GetLightType() const;

	/** Activate or disable the light.
	  * \param pCastLight     Set to \c true to enable the light.
	  */
	void SetCastLight(bool pCastLight);

	/** Get the light state.
	  * \return     \c true if the light is currently active.
	  */
	bool GetCastLight() const;

	/** Set the shadow state for the light.
	  * \param pCastShadows    Set to \c true to have the light cast shadows.
	  */
	void SetCastShadows(bool pCastShadows);

	/** Get the shadow state for the light.
	  * \return     \c true if the light is currently casting shadows.
	  */
	bool GetCastShadows() const;

	/** Set the shadow color for the light.
	  * \param pColor     The shadow color for the light expressed as kFbxColor.
	  */
	void SetShadowColor( KFbxColor& pColor );

	/** Get the shadow color for the light.
	  * \return     The shadow color of the light expressed as kFbxColor.
	  */
	KFbxColor GetShadowColor() const;

	/** Set the shadow texture for the light.
	  * \param pTexture     The texture cast by the light shadow.
	  */
	void SetShadowTexture( KFbxTexture* pTexture );

	/** Get the light state.
	  * \return     Pointer to the texture cast by the light shadow, or \c NULL if the shadow texture has not been set.
	  */
	KFbxTexture* GetShadowTexture() const;

	//@}

	/**
	  * \name Gobo properties
	  */
	//@{

	/** Set the associated gobo file. 
	  * \param pFileName     The path of the gobo file.   
	  * \return              \c false if the pointer is null.
	  * \remarks             The gobo file name must be valid. In addition, the gobo file must be an 8 bit grayscale TIFF image file with 
	  *                      height and width dimensions must equal a power of two, and it cannot exceed 256 pixels.
	  */
    bool SetFileName(char* pFileName);
    
#ifdef KARCH_DEV_MACOSX_CFM
    bool SetFile(const FSSpec &pMacFileSpec);
    bool SetFile(const FSRef &pMacFileRef);
    bool SetFile(const CFURLRef &pMacURL);
#endif

    /** Get the associated gobo file path.
	  * \return     The associated gobo file path, or an empty string if the gobo file has not been set (see KFbxGobo::SetFileName())
	  */
    char const* GetFileName() const;

#ifdef KARCH_DEV_MACOSX_CFM
	bool GetFile(FSSpec &pMacFileSpec) const;
    bool GetFile(FSRef &pMacFileRef) const;
    bool GetFile(CFURLRef &pMacURL) const;
#endif


	/** Sets the decay type
	  * \param pDecayType     The decay type
	  */
	void SetDecayType( EDecayType pDecayType );

	/** Gets the decay type
	  * \return     The decay type
	  */
	EDecayType GetDecayType() const;

	/** Sets the distance at which the light's intensity will decay.
	  * \param     The distance
	  */
	void SetDecayStart( double pDist );

	/** Gets the distance at which the light instensity will decay
	  * \return     The distance
	  */
	double GetDecayStart() const;

    /** Enable gobo ground projection.
	  * \param pEnable     Set to \c true to have the gobo project on the ground/floor.
      */
    void SetGroundProjection (bool pEnable);

    /** Get gobo ground projection flag.
	  * \return     \c true if gobo ground projection is enabled.
	  */
    bool GetGroundProjection() const;

    /** Enable gobo volumetric light projection.
	  * \param pEnable     Set to \c true to enable volumetric lighting projection.
	  */
    void SetVolumetricProjection(bool pEnable);

    /** Get gobo volumetric light projection flag.
	  * \return     \c true if gobo volumetric light projection is enabled.
	  */
    bool GetVolumetricProjection() const;

    /** Enable gobo front volumetric projection.
	  * \param pEnable     Set to \c true to enable front volumetric lighting projection.
	  * \remarks           This option is not supported in MotionBuilder.
	  */
    void SetFrontVolumetricProjection(bool pEnable);

    /** Get gobo front volumetric light projection flag.
	  * \return     \c true if gobo front volumetric light projection is enabled.
	  * \remarks    This option is not supported in MotionBuilder.
	  */
    bool GetFrontVolumetricProjection() const;

	//@}

	/**
	  * \name Default Animation Values
	  * This set of functions provide direct access to default animation values specific to a light. The default animation 
	  * values are found in the default take node of the associated node. These functions only work if the light has been associated
	  * with a node.
	  */
	//@{

	/** Set default color.
	  * \param pColor     The color of the light. 
	  * \remarks          The default value is white.
	  */
	void SetDefaultColor(KFbxColor& pColor);

	/** Get default color.
	  * \param pColor     The color of the light.
	  * \return           Input parameter filled with appropriate data.
	  * \remarks          The default value is white.
	  */
	KFbxColor& GetDefaultColor(KFbxColor& pColor) const;

	/** Set default intensity.
	  * \param pIntensity     The intensity value of the light.
	  * \remarks              The intensity range is from 0 to 200, where 200 is full intensity. The default value is 100.
	  */
	void SetDefaultIntensity(double pIntensity);

	/** Get default intensity.
	  * \return      The intensity value of the light.
	  * \remarks     The intensity range is from 0 to 200, where 200 is full intensity. The default value is 100.
	  */
	double GetDefaultIntensity() const;

	/** Set default cone angle in degrees.
	  * \param pConeAngle     The cone angle value of the light.
	  * \remarks              The cone angle has range is from 0 to 160 degrees. The default value is 45 degrees. This function has no effect 
	  *                       if the light type is not set to eSPOT.
	  */
	void SetDefaultConeAngle(double pConeAngle);

	/** Get default cone angle in degrees.
	  * \return      The cone angle value of the light.
	  * \remarks     The cone angle has range is from 0 to 160 degrees. The default value is 45 degrees. This function has no effect 
	  *              if the light type is not set to eSPOT.
	  */
	double GetDefaultConeAngle() const;

	/** Set default fog.
	  * \param pFog     The fog value of the light.
	  * \remarks        This fog range is from 0 to 200, where 200 is full fog opacity. The default value is 50. This function has no effect 
	  *                 if the light type is not set to eSPOT.
	  */
	void SetDefaultFog(double pFog);

	/** Get default fog.
	  * \return      The fog value of the light.
	  * \remarks     This fog range is from 0 to 200, where 200 is full fog opacity. The default value is 50. This function has no effect 
	  *              if the light type is not set to eSPOT.
	  */
	double GetDefaultFog() const;

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

public:

	virtual KObject*  GetFbxObject_internal();
	virtual KObject const*  GetFbxObject_internal() const;

	// Clone
	virtual KFbxObject* Clone(KFbxObject::ECloneType pCloneType) const;

protected:
	static char const* GetNamePrefix() { return 0; }

	KFbxLight(KFbxSdkManager& pManager, char const* pName);
	virtual ~KFbxLight();

	//! Assignment operator.
    KFbxLight& operator= (KFbxLight const& pLight);

	virtual void Destruct(bool pRecursive, bool pDependents);

	/** Add channels specific to a light attribute.
      * \param pTakeNode     Take node to which specialized channels are added.
	  */
	virtual void AddChannels(KFbxTakeNode *pTakeNode);

	/** Remove channels specific to a light attribute.
      * \param pTakeNode     Take node from which specialized channels are removed.
	  */
	virtual void RemoveChannels(KFbxTakeNode *pTakeNode);

	/**
	  *	Initialize the properties
	  */
	virtual void Init();

	virtual bool FbxPlugNotify(KFbxPlugEvent const &pEvent);

	/**
	  * Used to retrieve the KProperty list from an attribute.
	  */
	KFbxLight_internal* mPH;

	virtual KString		GetTypeName() const;
	virtual KStringList	GetTypeFlags() const;

	KString mFileName;	

#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS

};

typedef KFbxLight* HKFbxLight;

#include <fbxfilesdk_nsend.h>

#endif // #ifndef _FBXSDK_LIGHT_H_



