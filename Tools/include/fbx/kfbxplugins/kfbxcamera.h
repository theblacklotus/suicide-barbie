/*!  \file kfbxcamera.h
 */

#ifndef _FBXSDK_CAMERA_H_
#define _FBXSDK_CAMERA_H_

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
#include <kfbxplugins/kfbxcolor.h>

#include <kfbxmath/kfbxvector4.h>

#include <klib/kstring.h>

#ifdef KARCH_DEV_MACOSX_CFM
	#include <CFURL.h>
	#include <Files.h>
#endif

#include <fbxfilesdk_nsbegin.h>

class KFbxTexture;
class KFbxSdkManager;
class KFbxCamera_internal;

/** \brief This node attribute contains methods for accessing the properties of a camera.
  * \nosubgrouping
  * A camera can be set to automatically point at and follow 
  * another node in the hierarchy. To do this, the focus source
  * must be set to ECameraFocusSource::eCAMERA_INTEREST and the 
  * followed node associated with function KFbxNode::SetTarget().
  */
class KFBX_DLL KFbxCamera : public KFbxNodeAttribute
{
	KFBXOBJECT_DECLARE(KFbxCamera);
	
public:
	//! Return the type of node attribute which is EAttributeType::eCAMERA.
	virtual EAttributeType GetAttributeType() const;

	//! Reset the camera to default values.
	void Reset();

	/**
	  * \name Camera Position and Orientation Functions
	  */
	//@{

    /** Set the default XYZ camera position.
	  * \param pPosition     X, Y and Z values of the camera position, expressed as a vector.
	  * \remarks             If this attribute is not yet attached to a node, this method does nothing.
	  */
    void SetPosition(const KFbxVector4& pPosition);
	
    /** Get the default position of the camera.
	  * \return      The X, Y and Z values of the default position of the camera.
	  * \remarks     If this attribute is not attached to a node yet, this method will return 0,0,0.
	  */
    KFbxVector4 GetPosition() const;

    /** Set the camera's Up vector.
	  * \param pVector     The X, Y and Z values for the Up vector.
	  */
    void SetUpVector(const KFbxVector4& pVector);

    /** Get the current Up vector.
	  * \return     The X, Y and Z values of the currently set Up vector.
	  */
    KFbxVector4 GetUpVector() const;

    /** Set the default point the camera is looking at.
	  * \param pPosition     X, Y and Z values of the camera interest point.
	  * \remarks             During the computations of the camera position and orientation,
	  *                      the default camera interest position is overridden by the position of a 
	  *                      valid target in the parent node.
	  */
    void SetDefaultCameraInterestPosition(const KFbxVector4& pPosition);

    /** Return the camera interest to default coordinates.
	  * \return      The X,Y and Z values of the default interest point.
	  * \remarks     During the computations of the camera position and orientation,
	  *              the default camera interest position is overridden by the position of a 
	  *              valid target in the parent node.
	  */
    KFbxVector4 GetDefaultCameraInterestPosition() const;

    /** Set the camera roll angle.
	  * \param pRoll     The roll angle in degrees.
	  */
    void SetRoll(double pRoll);

    /** Get the camera roll angle.
	  * \return     The roll angle in degrees.
	  */
    double GetRoll() const;

    /** Set the camera turntable angle.
	  * \param pTurnTable     The turntable angle in degrees.
	  */
    void SetTurnTable(double pTurnTable);

    /** Get the camera turntable angle.
	  * \return     The turntable angle in degrees.
	  */
    double GetTurnTable() const;

    /** Camera projection types.
	  * \remarks     By default, the camera projection type is set to ePERSPECTIVE.
	  *              If the camera projection type is set to eORTHOGONAL, the following options 
	  *              are not relevant:
	  *                   - aperture format
	  *                   - aperture mode
	  *                   - aperture width and height
	  *                   - angle of view/focal length
	  *                   - squeeze ratio
	  */

	/** \enum ECameraProjectionType Camera projection types.
	  * - \e ePERSPECTIVE
	  * - \e eORTHOGONAL
	  */
    typedef enum 
    { 
	    ePERSPECTIVE, 
	    eORTHOGONAL 
    } ECameraProjectionType;

	/** Set the camera projection type.
	  * \param pProjectionType     The camera projection identifier.
	  */
    void SetProjectionType(ECameraProjectionType pProjectionType);

	/** Get the camera projection type.
	  * \return     The camera's current projection identifier.
	  */
    ECameraProjectionType GetProjectionType() const;

	//@}

	/**
	  * \name Viewing Area Functions
	  */
	//@{

	/** \enum ECameraFormat Camera formats.
	  * - \e eCUSTOM_FORMAT
	  * - \e eD1_NTSC
	  * - \e eNTSC
	  * - \e ePAL
	  * - \e eD1_PAL
	  * - \e eHD
	  * - \e e640x480
	  * - \e e320x200
	  * - \e e320x240
	  * - \e e128x128
	  * - \e eFULL_SCREEN
	  */
	typedef enum 
	{
		eCUSTOM_FORMAT,
		eD1_NTSC,
		eNTSC,
		ePAL,
		eD1_PAL,
		eHD,
		e640x480,
		e320x200,
		e320x240,
		e128x128,
		eFULL_SCREEN
	} ECameraFormat;

	/** Set the camera format.
	  * \param pFormat     The camera format identifier.
	  * \remarks           Changing the camera format sets the camera aspect 
	  *                    ratio mode to eFIXED_RESOLUTION and modifies the aspect width 
	  *                    size, height size, and pixel ratio accordingly.
	  */
    void SetFormat(ECameraFormat pFormat);

	/** Get the camera format.
	  * \return     The current camera format identifier.
	  */
    ECameraFormat GetFormat() const;

	/** \enum ECameraAspectRatioMode Camera aspect ratio modes.
	  * - \e eWINDOW_SIZE
	  * - \e eFIXED_RATIO
	  * - \e eFIXED_RESOLUTION
	  * - \e eFIXED_WIDTH
	  * - \e eFIXED_HEIGHT
	  */
    typedef enum 
    {
	    eWINDOW_SIZE,
	    eFIXED_RATIO,
	    eFIXED_RESOLUTION,
	    eFIXED_WIDTH,
	    eFIXED_HEIGHT
    } ECameraAspectRatioMode;

	/** Set the camera aspect.
	  * \param pRatioMode     Camera aspect ratio mode.
	  * \param pWidth         Camera aspect width, must be a positive value.
	  * \param pHeight        Camera aspect height, must be a positive value.
	  * \remarks              Changing the camera aspect sets the camera format to eCustom.
	  *                            - If the ratio mode is eWINDOW_SIZE, both width and height values aren't relevant.
	  *                            - If the ratio mode is eFIXED_RATIO, the height value is set to 1.0 and the width value is relative to the height value.
	  *                            - If the ratio mode is eFIXED_RESOLUTION, both width and height values are in pixels.
	  *                            - If the ratio mode is eFIXED_WIDTH, the width value is in pixels and the height value is relative to the width value. 
	  *                            - If the ratio mode is eFIXED_HEIGHT, the height value is in pixels and the width value is relative to the height value. 
	  */
    void SetAspect(ECameraAspectRatioMode pRatioMode, double pWidth, double pHeight);

	/** Get the camera aspect ratio mode.
	  * \return     The current aspect ratio identifier.
	  */
    ECameraAspectRatioMode GetAspectRatioMode() const;

	/** Get the aspect width.
	  * \return     The aspect width value or an undefined value if aspect ratio mode is set to eWINDOW_SIZE.
	  */
	double GetAspectWidth() const;

	/** Get the aspect height.
	  * \return     The aspect height value or an undefined value if aspect ratio mode is set to eWINDOW_SIZE.
	  */
	double GetAspectHeight() const;

	/** Set the pixel ratio.
	  * \param pRatio     The pixel ratio value.
	  * \remarks          The value must be a positive number. Comprised between 0.05 and 20.0. Values
	  *                   outside these limits will be clamped. Changing the pixel ratio sets the camera format to eCUSTOM_FORMAT.
	  */
	void SetPixelRatio(double pRatio);

	/** Get the pixel ratio.
	  * \return     The current camera's pixel ratio value.
	  */
	double GetPixelRatio() const;

	/** Set the near plane distance from the camera.
	  * The near plane is the minimum distance to render a scene on the camera display.
	  * \param pDistance     The near plane distance value.
	  * \remarks             The near plane value is limited to the range [0.001, 600000.0] and
	  *                      must be inferior to the far plane value. 
	  */
	void SetNearPlane(double pDistance);

	/** Get the near plane distance from the camera.
	  * The near plane is the minimum distance to render a scene on the camera display.
	  * \return     The near plane value.
	  */
	double GetNearPlane() const;

	/** Set the far plane distance from camera.
	  * The far plane is the maximum distance to render a scene on the camera display.
	  * \param pDistance     The far plane distance value.
	  * \remarks             The far plane value is limited to the range [0.001, 600000.0] and
	  *                      must be superior to the near plane value. 
	  */
	void SetFarPlane(double pDistance);
	
	/** Get the far plane distance from camera.
	  * The far plane is the maximum distance to render a scene on the camera display.
	  * \return     The far plane value.
	  */
	double GetFarPlane() const;

	/** Set interactive camera lock flag.
	  * \param pMouseLock     If \c true, disable modifications of the view area controls using mouse and keyboard commands.
	  * \remarks              It is the responsibility of the client application to perform the required tasks according to the state
	  *                       of this flag.
	  */
	void SetMouseLock(bool pMouseLock);

	/** Get the camera lock flag.
	  * \return     \c true If modifications of the view area controls using mouse and keyboard commands are disabled.
	  * \remark     It is the responsibility of the client application to perform the required tasks according to the state
	  *             of this flag.
	  */
	bool GetMouseLock() const;

	//@}

	/**
	  * \name Aperture and Film Functions
	  * The aperture mode determines which values drive the camera aperture. When the aperture mode is \e eHORIZONTAL_AND_VERTICAL,
	  * \e eHORIZONTAL or \e eVERTICAL, the field of view is used. When the aperture mode is \e eFOCAL_LENGTH, the focal length is used.
      *
      * It is possible to convert the aperture mode into field of view or vice versa using functions ComputeFieldOfView and
      * ComputeFocalLength. These functions use the camera aperture width and height for their computation.
	  */
	//@{

    /** \enum ECameraApertureFormat Camera aperture formats.
	  * - \e eCUSTOM_APERTURE_FORMAT
	  * - \e e16MM_THEATRICAL
	  * - \e eSUPER_16MM
	  * - \e e35MM_ACADEMY
	  * - \e e35MM_TV_PROJECTION
      * - \e e35MM_FULL_APERTURE
	  * - \e e35MM_185_PROJECTION
	  * - \e e35MM_ANAMORPHIC
	  * - \e e70MM_PROJECTION
	  * - \e eVISTAVISION
	  * - \e eDYNAVISION
	  * - \e eIMAX
	  */
    typedef enum 
    {
		eCUSTOM_APERTURE_FORMAT = 0,
		e16MM_THEATRICAL,
		eSUPER_16MM,
		e35MM_ACADEMY,
		e35MM_TV_PROJECTION,
		e35MM_FULL_APERTURE,
		e35MM_185_PROJECTION,
		e35MM_ANAMORPHIC,
		e70MM_PROJECTION,
		eVISTAVISION,
		eDYNAVISION,
		eIMAX		
	} ECameraApertureFormat;

	/** Set the camera aperture format.
	  * \param pFormat     The camera aperture format identifier.
	  * \remarks           Changing the aperture format modifies the aperture width, height, and squeeze ratio accordingly.
	  */
    void SetApertureFormat(ECameraApertureFormat pFormat);

	/** Get the camera aperture format.
      * \return     The camera's current aperture format identifier.
	  */
    ECameraApertureFormat GetApertureFormat() const;

    /** \enum ECameraApertureMode
	  * Camera aperture modes. The aperture mode determines which values drive the camera aperture. If the aperture mode is \e eHORIZONTAL_AND_VERTICAL,
	  * \e eHORIZONTAL, or \e eVERTICAL, then the field of view is used. If the aperture mode is \e eFOCAL_LENGTH, then the focal length is used.
	  * - \e eHORIZONTAL_AND_VERTICAL
	  * - \e eHORIZONTAL
	  * - \e eVERTICAL
	  * - \e eFOCAL_LENGTH
	  */
    typedef enum 
    { 
	    eHORIZONTAL_AND_VERTICAL,
	    eHORIZONTAL,
	    eVERTICAL,
		eFOCAL_LENGTH
    } ECameraApertureMode;

	/** Set the camera aperture mode.
	  * \param pMode     The camera aperture mode identifier.
	  */
    void SetApertureMode(ECameraApertureMode pMode);

	/** Get the camera aperture mode.
	  * \return     The camera's current aperture mode identifier.
	  */
    ECameraApertureMode GetApertureMode() const;

	/** Set the camera aperture width in inches.
	  * \param pWidth     The aperture width value.
	  * \remarks          Must be a positive value. The minimum accepted value is 0.0001. 
	  *                   Changing the aperture width sets the camera aperture format to eCUSTOM_FORMAT.
	  */
	void SetApertureWidth(double pWidth);
	
	/** Get the camera aperture width in inches.
	  * \return     The camera's current aperture width value in inches.
	  */
	double GetApertureWidth() const;

	/** Set the camera aperture height in inches.
	  * \param pHeight     The aperture height value.
	  * \remarks           Must be a positive value. The minimum accepted value is 0.0001. 
	  *                    Changing the aperture height sets the camera aperture format to eCUSTOM_FORMAT.
	  */
	void SetApertureHeight(double pHeight);

	/** Get the camera aperture height in inches.
	  * \return     The camera's current aperture height value in inches.
	  */
	double GetApertureHeight() const;

	/** Set the squeeze ratio.
	  * \param pRation     The sqeeze ratio value.
	  * \remarks           Must be a positive value. The minimum accepted value is 0.0001. 
	  *                    Changing the squeeze ratio sets the camera aperture format to eCUSTOM_FORMAT.
	  */
	void SetSqueezeRatio(double pRatio);

	/** Get the camera squeeze ratio.
	  * \return     The camera's current squeeze ratio value.
	  */
	double GetSqueezeRatio() const;

    /** Set the camera ortographic zoom
	  * \param pOrthoZoom     This parameter's default value is 1.0.
	  * \remarks              This parameter is not used if the camera is not orthographic.
      */
    void SetOrthoZoom(const double& pOrthoZoom);

    /** Get the camera ortographic zoom.
	  * \return     The camera's current orthographic zoom.
	  */
    double GetOrthoZoom() const;

    /** \enum ECameraGateFit
	  * Camera gate fit modes.
	  * - \e eNO_FIT			No resoluton gate fit.
	  * - \e eVERTICAL_FIT		Fit the resolution gate vertically within the film gate.
	  * - \e eHORIZONTAL_FIT	Fit the resolution gate horizontally within the film gate.
	  * - \e eFILL_FIT			Fit the resolution gate within the film gate.
  	  * - \e eOVERSCAN_FIT		Fit the film gate within the resolution gate.
	  * - \e eSTRETCH_FIT		Fit the resolution gate to the film gate.
	  */
    typedef enum 
    { 
		eNO_FIT,
		eVERTICAL_FIT,
		eHORIZONTAL_FIT,
		eFILL_FIT,
		eOVERSCAN_FIT,
		eSTRETCH_FIT
    } ECameraGateFit;

    /** Set the camera gate fit.
	  * \param pGateFit     This parameter's default value is eNO_FIT.
      */
    void SetGateFit(const ECameraGateFit pGateFit);

    /** Get the camera gate fit.
	  * \return     The camera's current gate fit.
	  */
    ECameraGateFit GetGateFit() const;

	/** Compute the angle of view based on the given focal length, the aperture width, and aperture height.
	  * \param pFocalLength     The focal length in millimeters
	  * \return                 The computed angle of view in degrees
	  */
	double ComputeFieldOfView(double pFocalLength) const;

	/** Compute the focal length based on the given angle of view, the aperture width, and aperture height.
	  * \param pAngleOfView     The angle of view in degrees
	  * \return                 The computed focal length in millimeters
	  */
    double ComputeFocalLength(double pAngleOfView);
	//@}

	/**
	  * \name Background Functions
	  */
	//@{

    /** Set the associated background image file. 
      * \param pFileName     The path of the background image file.   
	  * \remarks             The background image file name must be valid.
      */
	void SetBackgroundFileName(char* pFileName);

#ifdef KARCH_DEV_MACOSX_CFM
    bool SetBackgroundFile(const FSSpec &pMacFileSpec);
    bool SetBackgroundFile(const FSRef &pMacFileRef);
    bool SetBackgroundFile(const CFURLRef &pMacURL);
#endif

	/** Get the background image file name.
	  * \return     Pointer to the background filename string or \c NULL if not set.
	  */
	char const* GetBackgroundFileName() const;

#ifdef KARCH_DEV_MACOSX_CFM
	bool GetBackgroundFile(FSSpec &pMacFileSpec) const;
    bool GetBackgroundFile(FSRef &pMacFileRef) const;
    bool GetBackgroundFile(CFURLRef &pMacURL) const;
#endif

    /** Set the media name associated to the background image file. 
      * \param pFileName     The media name of the background image file.   
	  * \remarks             The media name is a unique name used to identify the background image file.
      */
	void SetBackgroundMediaName(char* pFileName);

	/** Get the media name associated to the background image file.
	  * \return     Pointer to the media name string or \c NULL if not set.
	  */
	char const* GetBackgroundMediaName() const;

	/** \enum ECameraBackgroundDisplayMode Background display modes.
	  * - \e eDISABLED
	  * - \e eALWAYS
	  * - \e eWHEN_MEDIA
	  */
	typedef enum 
	{
		eDISABLED,
		eALWAYS,  
		eWHEN_MEDIA
	} ECameraBackgroundDisplayMode;

	/** Set the background display mode.
	  * \param pMode     The background display mode identifier.
	  */
	void SetBackgroundDisplayMode(ECameraBackgroundDisplayMode pMode);

	/** Get the background display mode.
	  * \return     The currently set background display mode identifier.
	  */
	ECameraBackgroundDisplayMode GetBackgroundDisplayMode() const;

	/** \enum ECameraBackgroundDrawingMode Background drawing modes.
	  * - \e eBACKGROUND                 Image is drawn behind models.
	  * - \e eFOREGROUND                 Image is drawn in front of models based on alpha channel.
	  * - \e eBACKGROUND_AND_FOREGROUND  Image is drawn behind and in front of models depending on alpha channel.
	  */
	typedef enum 
	{
		eBACKGROUND,
		eFOREGROUND,
		eBACKGROUND_AND_FOREGROUND
	} ECameraBackgroundDrawingMode;

	/** Set the background drawing mode.
	  * \param pMode     The background drawing mode identifier.
	  */
	void SetBackgroundDrawingMode(ECameraBackgroundDrawingMode pMode);

	/** Get the background drawing mode.
	  * \return The currently set background drawing mode identifier.
	  */
	ECameraBackgroundDrawingMode GetBackgroundDrawingMode() const;

	/** Set the foreground matte threshold flag.
	  * \param pEnable     If \c true enable foreground matte threshold. 
	  * \remarks           It is the responsibility of the client application to perform the required tasks according to the state
	  *                    of this flag.
	  */
	void SetForegroundMatteThresholdEnable(bool pEnable);
    
	/** Get the foreground matte threshold flag.
	  * \return            \c true if foreground matte threshold is enabled, otherwise \c false.
	  * \remark            It is the responsibility of the client application to perform the required tasks according to the state
	  *                    of this flag.
	  */
	bool GetForegroundMatteThresholdEnable() const;

	/** Set foreground matte threshold. 
	  * \param pThreshold     Threshold value on a range from 0.0 to 1.0.
	  * \remarks              This option is only relevant if the background drawing mode is set to eFOREGROUND or eBACKGROUND_AND_FOREGROUND.
	  */
	void SetForegroundMatteThreshold(double pThreshold);
    
	/** Get foreground matte threshold. 
	  * \return      Threshold value on a range from 0.0 to 1.0.
	  * \remarks     This option is only relevant if the background drawing mode is set to eFOREGROUND or eBACKGROUND_AND_FOREGROUND.
	  */
	double GetForegroundMatteThreshold() const;

	/** \enum ECameraBackgroundPlacementOptions Background placement options.
	  * - \e eFIT
	  * - \e eCENTER
	  * - \e eKEEP_RATIO
	  * - \e eCROP
	  */
	typedef enum 
	{
		eFIT = 1<<0,
		eCENTER = 1<<1,
		eKEEP_RATIO = 1<<2,
		eCROP = 1<<3
	} ECameraBackgroundPlacementOptions;
	
	/** Set background placement options.
	  * \param pOptions     Bitwise concatenation of one or more background placement options.
	  */
	void SetBackgroundPlacementOptions(kUInt pOptions);

	/** Get background placement options.
	  * \return     The bitwise concatenation of the currently set background placement options.
	  */
	kUInt GetBackgroundPlacementOptions() const;

	/** ECamerabackgroundDistanceMode Background distance modes.
	  * - \e eRELATIVE_TO_INTEREST
	  * - \e eABSOLUTE_FROM_CAMERA
	  */
	typedef enum 
    {
	    eRELATIVE_TO_INTEREST,
	    eABSOLUTE_FROM_CAMERA
    } ECameraBackgroundDistanceMode;

	/** Set the background distance mode.
      * \param pMode     The background distance mode identifier.
	  */
    void SetBackgroundDistanceMode(ECameraBackgroundDistanceMode pMode);

	/** Get the background distance mode.
	  * \return     Return the background distance mode identifier.
	  */
    ECameraBackgroundDistanceMode GetBackgroundDistanceMode() const;

	/** Set the background distance.
	  * \param pDistance     Distance of the background plane. This value can be either relative to the camera interest point or 
      *                      absolute from the camera position.
	  */
	void SetBackgroundDistance(double pDistance);

	/** Get the background distance.
      * \return     The distance of the background plane.
	  */
	double GetBackgroundDistance() const;

	//@}

	/**
	  * \name Camera View Functions
	  * It is the responsibility of the client application to perform the required tasks according to the state
	  * of the options that are either set or returned by these methods.
	  */
	//@{

	/** Change the camera interest visibility flag.
	  * \param pEnable     Set to \c true if the camera interest is shown.
	  */
	void SetViewCameraInterest(bool pEnable);

	/** Get current visibility state of the camera interest.
	  * \return     \c true if the camera interest is shown, or \c false if hidden.
	  */
	bool GetViewCameraInterest() const;

	/** Change the camera near and far planes visibility flag.
	  * \param pEnable     Set to \c true if the near and far planes are shown.
	  */
	void SetViewNearFarPlanes(bool pEnable);

	/** Get current visibility state of the camera near and far planes.
	  * \return     \c true if the near and far planes are shown.
	  */
	bool GetViewNearFarPlanes() const;
	
	/** Change the draw floor grid flag.
	  * \param pEnable     Set to \c true if the floor grid is shown.
	  */
	void SetShowGrid(bool pEnable);
	
	/** Get current floor grid draw state.
	  * \return    \c true if the floor grid is shown, or \c false if hidden.
	  */
	bool GetShowGrid() const;

	/** Change the draw system axis flag.
	  * \param pEnable     Set to \c true if the system axis is shown.
	  */
	void SetShowAxis(bool pEnable);

	/** Get current system axis draw state.
	  * \return     \c true if the system axis is shown, or \c false if hidden.
	  */
	bool GetShowAxis() const;

	/** Change the show camera name flag.
	  * \param pEnable     Set to \c true if the camera name is shown.
	  */
	void SetShowName(bool pEnable);

	/** Get current camera name show state.
	  * \return     \c true if the camera name is shown, or \c false if hidden.
	  */
	bool GetShowName() const;

	/** Change the show info on moving flag.
	  * \param pEnable     Set to \c true if info on moving is shown.
	  */
	void SetShowInfoOnMoving(bool pEnable);

	/** Get current info on moving show state.
	  * \return     \c true if info on moving is shown, or \c false if hidden.
	  */
	bool GetShowInfoOnMoving() const;

	/** Change the timecode show flag.
	  * \param pEnable     Set to \c true if the timecode is shown.
	  */
	void SetShowTimeCode(bool pEnable);
	
	/** Get current timecode show state.
	  * \return     \c true if the timecode is shown, or \c false if hidden.
	  */
	bool GetShowTimeCode() const;

	/** Change the display camera safe area flag.
	  * \param pEnable     Set to \c true if the safe area is shown.
	  */
	void SetDisplaySafeArea(bool pEnable);

	/** Get current safe area display state.
	  * \return     \c true if safe area is shown, or \c false if hidden.
	  */
	bool GetDisplaySafeArea() const;

	/** Change the display of the camera's safe area on render flag.
	  * \param pEnable     Set to \c true if safe area is shown on render.
	  */
	void SetDisplaySafeAreaOnRender(bool pEnable);

	/** Get current safe area on render display state.
	  * \return     \c true if the safe area is shown on render, or \c false if it is hidden on render.
	  */
	bool GetDisplaySafeAreaOnRender() const;

    /** \enum ECameraSafeAreaStyle Camera safe area display styles.
	  * - \e eROUND
	  * - \e eSQUARE
	  */
    typedef enum 
    { 
	    eROUND = 0, 
	    eSQUARE = 1 
    } ECameraSafeAreaStyle;

	/** Set the safe area style.
	  * \param pStyle    Safe area style identifier.
	  */
    void SetSafeAreaStyle(ECameraSafeAreaStyle pStyle);

	/** Get the currently set safe area style.
	  * \return     Safe area style identifier.
	  */
    ECameraSafeAreaStyle GetSafeAreaStyle() const;

	/** Change the show audio flag.
	  * \param pEnable     Set to \c true if audio waveform is shown.
	  */
	void SetShowAudio(bool pEnable);

	/** Get current audio show state.
	  * \return     \c true if audio is shown, or \c false if hidden.
	  */
	bool GetShowAudio() const;

	/** Set audio color.
	  * \param pColor     RGB values for the audio waveform color.
	  */
	void SetAudioColor(const KFbxColor& pColor);

	/** Get audio color.
	  * \return     Currently set audio waveform color.
	  */
	KFbxColor GetAudioColor() const;

	/** Change the use frame color flag.
	  * \param pEnable     Set to \c true if the frame color is used.     
	  */
	void SetUseFrameColor(bool pEnable);

	/** Get the use frame color state.
	  * \return     \c true if the frame color is used, or \c false otherwise.
	  */
	bool GetUseFrameColor() const;

	/** Set frame color.
	  * \param pColor     RGB values for the frame color.
	  */
	void SetFrameColor(const KFbxColor& pColor);

	/** Get frame color.
	  * \return     Currently set frame color.
	  */
	KFbxColor GetFrameColor() const;

	//@}

	/**
	  * \name Render Functions
	  * It is the responsibility of the client application to perform the required tasks according to the state
	  * of the options that are either set or returned by these methods.
	  */
	//@{

    /** \enum ECameraRenderOptionsUsageTime Render options usage time.
	  * - \e eINTERACTIVE
	  * - \e eAT_RENDER
	  */
    typedef enum 
    {
	    eINTERACTIVE,
	    eAT_RENDER
    } ECameraRenderOptionsUsageTime;

	/** Set the render options usage time.
	  * \param pUsageTime     The render options usage time identifier.
	  */
    void SetRenderOptionsUsageTime(ECameraRenderOptionsUsageTime pUsageTime);

	/** Get the render options usage time.
	  * \return     Render options usage time identifier.
	  */
    ECameraRenderOptionsUsageTime GetRenderOptionsUsageTime() const;

	/** Change the use antialiasing flag.
	  * \param     Set to \c true if antialiasing is enabled.
	  */
	void SetUseAntialiasing(bool pEnable);

	/** Get the use antialiasing state.
	  * \return     \c true if antialiasing is enabled, or \c false if disabled.
	  */
	bool GetUseAntialiasing() const;

	/** Set antialiasing intensity.
	  * \param pIntensity     Antialiasing intensity value.
	  */
	void SetAntialiasingIntensity(double pIntensity);

	/** Get the antialiasing intensity.
	  * \return     Return the current antialiasing intensity value. 
	  */
	double GetAntialiasingIntensity() const;

    /** \enum ECameraAntialiasingMethod Antialiasing methods.
	  * - \e eOVERSAMPLING_ANTIALIASING
	  * - \e eHARDWARE_ANTIALIASING
	  */
    typedef enum 
    {
	    eOVERSAMPLING_ANTIALIASING, 
	    eHARDWARE_ANTIALIASING
    } ECameraAntialiasingMethod;

	/** Set antialiasing method.
	  * \param pMethod     The antialiasing method identifier.
	  */
    void SetAntialiasingMethod(ECameraAntialiasingMethod pMethod);

	/** Get antialiasing method.
	  * \return     The current antialiasing method identifier.
	  */
    ECameraAntialiasingMethod GetAntialiasingMethod() const;

	/** Set the number of samples used to process oversampling.
	  * \param pNumberOfSamples     Number of samples used to process oversampling.
	  * \remarks                    This option is only relevant if antialiasing method is set to eOVERSAMPLING_ANTIALIASING.
	  */
	void SetNumberOfSamples(int pNumberOfSamples);

	/** Get the number of samples used to process oversampling.
	  * \return      The current number of samples used to process oversampling.
	  * \remarks     This option is only relevant if antialiasing method is set to eOVERSAMPLING_ANTIALIASING.
	  */
	int GetNumberOfSamples() const;

    /** \enum ECameraSamplingType Oversampling types.
	  * - \e eUNIFORM
	  * - \e eSTOCHASTIC
	  */
    typedef enum 
    {
	    eUNIFORM,
	    eSTOCHASTIC
    } ECameraSamplingType;

	/** Set sampling type.
	  * \param pType     Sampling type identifier.
	  * \remarks         This option is only relevant if antialiasing type is set to eOVERSAMPLING_ANTIALIASING.
	  */
    void SetSamplingType(ECameraSamplingType pType);

	/** Get sampling type.	
	  * \return      The current sampling type identifier.
	  * \remarks     This option is only relevant if antialiasing type is set to eOVERSAMPLING_ANTIALIASING.
	  */
	ECameraSamplingType GetSamplingType() const;

	/** Change the use accumulation buffer flag.
      * \param pUseAccumulationBuffer     Set to \c true to enable use of the accumulation buffer.
	  */
	void SetUseAccumulationBuffer(bool pUseAccumulationBuffer);

	/** Get the state of the use accumulation buffer flag.
	  * \return     \c true if the use accumulation buffer flag is enabled, \c false otherwise.
	  */
	bool GetUseAccumulationBuffer() const;

	/** Change use depth of field flag.
	  * \param pUseDepthOfField     Set to \c true if depth of field is used.
	  */
	void SetUseDepthOfField(bool pUseDepthOfField);

	/** Get use depth of field state.
	  * \return     \c true if depth of field is used, \c false otherwise.
	  */
	bool GetUseDepthOfField() const;

    /** \enum ECameraFocusDistanceSource Camera focus sources.
	  * - \e eCAMERA_INTEREST
	  * - \e eSPECIFIC_DISTANCE
	  */
    typedef enum 
    {
	    eCAMERA_INTEREST, 
	    eSPECIFIC_DISTANCE
    } ECameraFocusDistanceSource;

	/** Set source of camera focus distance.
	  * \param pSource     Focus distance source identifier.
	  */
    void SetFocusDistanceSource(ECameraFocusDistanceSource pSource);

	/** Get source of camera focus distance.
	  * \return     Focus distance source identifier.
	  */
	ECameraFocusDistanceSource GetFocusDistanceSource() const;

	/** Set the focus distance of the lens in millimiters.
	  * \param pDistance     Focus distance value.
	  * \remarks             This option is only relevant if focus distance source is set to eSPECIFIC_DISTANCE.
	  */
	void SetSpecificDistance(double pDistance);

	/** Get the focus distance of the lens in millimiters.
	  * \return      Focus distance value.
	  * \remarks     This option is only relevant if focus distance source is set to eSPECIFIC_DISTANCE.
	  */
	double GetSpecificDistance() const;

	/** Set the focus angle in degrees.
	  * \param pAngle     Focus angle value.
	  */
	void SetFocusAngle(double pAngle);
	
	/** Get the focus angle in degrees.
	  * \return     Focus angle value.
	  */
	double GetFocusAngle() const;

	//@}

	/**
	  * \name Default Animation Values
	  * These functions provide direct access to default animation values specific to a camera. 
      * Since the default animation values are found in the default take node of the associated node,
	  * these functions only work if the camera has been associated with a node.
	  */
	//@{

	/** Set default field of view in degrees.
      * Use this function to set the default field of view value when the camera aperture mode is set to either \e eHORIZONTAL or \e eVERTICAL.
	  * When the camera aperture mode is set to \e eHORIZONTAL, this function sets the horizontal field of view in degrees and the vertical field of
	  * view is adjusted accordingly. When the camera aperture mode is set to \e eVERTICAL, this function sets the vertical field of view in 
      * degrees and the horizontal field of view is adjusted accordingly.
	  * \param pFieldOfView     Field of view value.
      * \remarks                This function has no effect when the camera aperture mode is set to either \e eHORIZONTAL_AND_VERTICAL or \e eFOCAL_LENGTH.
      *                         The default field of view value is 25.115.
	  */
	void SetDefaultFieldOfView(double pFieldOfView);

	/** Get default field of view.
	  * Use this function to get the default field of view value when the camera aperture mode is set to either \e eHORIZONTAL or \e eVERTICAL.
	  * \return     If the camera aperture mode is set to either \e eHORIZONTAL or \e eVERTICAL, this function returns either the horizontal or vertical 
      *             field of view value in degrees. If the camera aperture mode is set to either \e eHORIZONTAL_AND_VERTICAL or \e eFOCAL_LENGTH, this 
      *             function has no effect and returns 0.
      * \remarks    The default field of view value is 25.115.
	  */
	double GetDefaultFieldOfView() const;

	/** Set default field of view X.
      * Use this function to set the default field of view horizontal value when the camera aperture mode is set to \e eHORIZONTAL_AND_VERTICAL.
	  * This function sets the horizontal field of view in degrees.
	  * \param pFieldOfViewX     Field of view value.
      * \remarks                 This function has no effect if the camera aperture mode is set to \e eHORIZONTAL, \e eVERTICAL, or \e eFOCAL_LENGTH.
      *                          The default field of view horizontal value is 40.
	  */
	void SetDefaultFieldOfViewX(double pFieldOfViewX);

	/** Get default field of view X.
      * Use this function to get the default field of view horizontal value when the camera aperture mode is set to \e eHORIZONTAL_AND_VERTICAL.
	  * \return      If the camera aperture mode is set to \e eHORIZONTAL_AND_VERTICAL, return the current field of view horizontal value in degrees.
	  *              If the camera aperture mode is set to \e eHORIZONTAL, \e eVERTICAL, or \e eFOCAL_LENGTH, this function has no effect and returns 0.
      * \remarks     The default field of view X value is 40 degrees.
	  */
	double GetDefaultFieldOfViewX() const;

	/** Set default field of view Y.
      * Use this function to set the default field of view vertical value when the camera aperture mode is set to \e eHORIZONTAL_AND_VERTICAL.
	  * \param pFieldOfViewY     Field of view value.
      * \remarks                 This function has no effect if the camera aperture mode is set to \e eHORIZONTAL, \e eVERTICAL, or \e eFOCAL_LENGTH.
      *                          The default field of view horizontal value is 40.
	  */
	void SetDefaultFieldOfViewY(double pFieldOfViewY);

	/** Get default field of view Y.
      * Use this function to get the default field of view vertical value when the camera aperture mode is set to \e eHORIZONTAL_AND_VERTICAL.
	  * \return      If the camera aperture mode is set to \e eHORIZONTAL_AND_VERTICAL, return the current field of view vertical value in degrees.
	  *              If the camera aperture mode is set to \e eHORIZONTAL, \e eVERTICAL, or \e eFOCAL_LENGTH, this function has no effect and returns 0.
      * \remarks     The default field of view Y value is 40 degrees.
	  */
	double GetDefaultFieldOfViewY() const;

	/** Set default optical center X.
      * Use this function to set the default optical center horizontal value when the camera aperture mode is set to \e eHORIZONTAL_AND_VERTICAL.
	  * \param pOpticalCenterX     Optical center offset.
      * \remarks                   This function has no effect if the camera aperture mode is set to \e eHORIZONTAL, \e eVERTICAL, or \e eFOCAL_LENGTH.
      *                            The default optical center horizontal offset is 0.
	  */
	void SetDefaultOpticalCenterX(double pOpticalCenterX);

	/** Get default optical center X.
      * Use this function to get the default optical center horizontal offset when the camera aperture mode is set to \e eHORIZONTAL_AND_VERTICAL.
	  * \return      If the camera aperture mode is set to \e eHORIZONTAL_AND_VERTICAL, return the current optical center horizontal offset.
	  *              If the camera aperture mode is set to \e eHORIZONTAL, \e eVERTICAL, or \e eFOCAL_LENGTH, this function has no effect and returns 0.
      * \remarks     The default optical center X offset is 0.
      */
	double GetDefaultOpticalCenterX() const;

	/** Set default optical center Y.
      * Use this function to set the default optical center vertical offset when the camera aperture mode is set to \e eHORIZONTAL_AND_VERTICAL.
	  * \param pOpticalCenterY     Optical center offset.
      * \remarks                   This function has no effect if the camera aperture mode is set to \e eHORIZONTAL, \e eVERTICAL, or \e eFOCAL_LENGTH.
      *                            The default optical center vertical offset is 0.
	  */
	void SetDefaultOpticalCenterY(double pOpticalCenterY);

	/** Get default optical center Y.
      * Use this function to get the default optical center vertical offset when the camera aperture mode is set to \e eHORIZONTAL_AND_VERTICAL.
	  * \return      If the camera aperture mode is set to \e eHORIZONTAL_AND_VERTICAL, return the current optical center vertical offset.
	  *              If the camera aperture mode is set to \e eHORIZONTAL, \e eVERTICAL, or \e eFOCAL_LENGTH, this function has no effect and returns 0.
      * \remarks     The default optical center X offset is 0.
	  */
	double GetDefaultOpticalCenterY() const;

	/** Set default focal length
      * Use this function to set the default focal length when the camera aperture mode is set to \e eFOCAL_LENGTH.
	  * \param pFocalLength     Focal length value.
      * \remarks                This function has no effect if the camera aperture mode is set to \e eHORIZONTAL, \e eVERTICAL, or \e eHORIZONTAL_AND_VERTICAL.
      *                         The default focal length is 0.
	  */
	void SetDefaultFocalLength(double pFocalLength);

	/** Get default focal length
      * Use this function to get the default focal length when the camera aperture mode is set to \e eFOCAL_LENGTH.
	  * \return      If the camera aperture mode is set to \e eFOCAL_LENGTH, return the current default focal length.
	  *              If the camera aperture mode is set to \e eHORIZONTAL, \e eVERTICAL, or \e eHORIZONTAL_AND_VERTICAL, this function has no effect and 
      *              returns 0.
      * \remarks     The default focal length is 0.
	  */
	double GetDefaultFocalLength() const;

	/** Set default camera roll in degrees.
	  * \param pRoll     Roll value.
      * \remarks         The default roll value is 0.
	  */
	void SetDefaultRoll(double pRoll);

	/** Get default camera roll in degrees.
	  * \return     Current roll value.
      * \remarks    The default roll value is 0.
	  */
	double GetDefaultRoll() const;

	/** Set default turntable value in degrees.
	  * \param pTurnTable     Turntable value.
      * \remarks              The default turntable value is 0.
	  */
	void SetDefaultTurnTable(double pTurnTable);

	/** Get default turntable in degrees.
	  * \return     Current turntable value.
      * \remarks    The default turntable value is 0.
	  */
	double GetDefaultTurnTable() const;

	/** Set default background color.
	  * \param pColor     RGB values of the background color.
      * \remarks          The default background color is black.
	  */
	void SetDefaultBackgroundColor(const KFbxColor& pColor);

	/** Get default background color.
	  * \return      Current background color.
      * \remarks     The default background color is black.
	  */
	KFbxColor GetDefaultBackgroundColor() const;

	//@}

	/**
	  * \name Obsolete Functions
	  * These functions are obsolete since animated background color, animated field of view, and animated focal length are now supported.
	  */
	//@{

	/** Set background color. This method is replaced by the SetDefaultBackgroundColor when setting a non-animated value. For animated values, 
      * the client application must access the BackgroundColor fcurves in the take.
	  * \remarks     Deprecated: use SetDefaultBackgroundColor(pColor) instead.
	  */
	K_DEPRECATED void SetBackgroundColor(const KFbxColor& pColor);

	/** Get background color. This method is replaced by the GetDefaultBackgroundColor when getting a non-animated value. For animated values, 
      * the client application must access the BackgroundColor fcurves in the take.
	  * \remarks     Deprecated: use GetDefaultBackgroundColor() instead.
	  */
	K_DEPRECATED KFbxColor GetBackgroundColor() const;

    /** Set the camera angle of view in degrees. 
	  * \param pAngleOfView     The camera angle of view value in degrees. This value is limited to the range [1.0, 179.0].
	  * \remarks                Deprecated: use SetDefaultFieldOfView, SetDefaultFieldOfViewX, or SetDefaultFieldOfViewY instead.
      * \warning                Modifying the angle of view will automatically change the focal length.
      */
    K_DEPRECATED void SetAngleOfView(double pAngleOfView);

    /** Get the camera angle of view in degrees.
	  * \return      The camera's current angle of view value in degrees.
	  * \remarks     Deprecated; use GetDefaultFieldOfView(), GetDefaultFieldOfViewX() GetDefaultFieldOfViewY() instead.
	  */
    K_DEPRECATED double GetAngleOfView() const;

	/** Set the focal length of the camera in millimeters. 
	  * \param pFocalLength     The focal length in mm. 
	  * \remarks                Deprecated: use SetDefaultFocalLength(pFocalLength) instead.
	  * \warning                Modifying the focal length will automatically change the angle of view.
      */
    K_DEPRECATED void SetFocalLength(double pFocalLength);

    /** Get the camera focal length in millimeters.
	  * \return      The camera's current focal length value.
	  * \remarks     Deprecated: use GetDefaultFocalLength() instead.
	  */
    K_DEPRECATED double GetFocalLength() const;

	//@}


	void SetBackgroundTexture(KFbxTexture* pTexture);

///////////////////////////////////////////////////////////////////////////////
//
//  WARNING!
//
//	Anything beyond these lines may not be documented accurately and is 
// 	subject to change without notice.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef DOXYGEN_SHOULD_SKIP_THIS

	friend class KFbxGlobalCameraSettings;
	
public:

	virtual KObject* GetFbxObject_internal();
	virtual KObject const*  GetFbxObject_internal() const;

	// Clone
	virtual KFbxObject* Clone(KFbxObject::ECloneType pCloneType) const;

protected:
	static char const* GetNamePrefix() { return 0; }

	//! Assignment operator.
    KFbxCamera& operator=(KFbxCamera const& pCamera);

	KFbxCamera(KFbxSdkManager& pManager, char const* pName);
	virtual ~KFbxCamera ();

	virtual void Destruct(bool pRecursive, bool pDependents);

	/** Add channels specific to a camera attribute.
      * \param pTakeNode Take node to add specialized channels to.
	  */
	virtual void AddChannels(KFbxTakeNode *pTakeNode);

	/** Remove channels specific to a camera attribute.
      * \param pTakeNode Take node to remove specialized channels from.
	  */
	virtual void RemoveChannels(KFbxTakeNode *pTakeNode);

	/**
	  * Used to retrieve the KProperty list from an attribute
	  */
	KFbxCamera_internal* mPH;

	virtual KString		GetTypeName() const;
	virtual KStringList	GetTypeFlags() const;

private:

	double ComputePixelRatio(kUInt pWidth, kUInt pHeight, double pScreenRatio = 1.3333333333);

	// Camera Position and Orientation
    KFbxVector4 mUpVector;
	KFbxVector4 mDefaultCameraInterestPosition;

	// Background Properties
	KString mBackgroundMediaName;
	KString mBackgroundFileName;

	// Camera Ortho Zoom
	double mOrthoZoom;

	// Camera View Options
	bool mShowInfoOnMoving;
	bool mDisplaySafeAreaOnRender;
	bool mShowAudio;
	KFbxColor mAudioColor;

	// Render Options
	ECameraAntialiasingMethod mAntialiasingMethod;
	ECameraSamplingType mSamplingType;

	friend class KFbxNode;

#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS

};

typedef KFbxCamera* HKFbxCamera;

#include <fbxfilesdk_nsend.h>

#endif // #ifndef _FBXSDK_CAMERA_H_


