/*!  \file kfbxfilterset.h
 */

#ifndef _FBXSDK_FILTER_SET_H_
#define _FBXSDK_FILTER_SET_H_

/**************************************************************************************

 Copyright © 2002 - 2006 Autodesk, Inc. and/or its licensors.
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

#include <kcharacterdef/kcharacterdef_forward.h>
#ifndef MB_FBXSDK
#include <kcharacterdef/kcharacterdef_nsuse.h>
#endif

#include <fbxfilesdk_nsbegin.h>

/** \class KFbxFilterSet
  *
  * \brief This class contains all the parameters for one character.
  *
  * Use this class to import or export all parameters for one character.
  * You get access to the character's filter set with the KFbxCharacter
  * member function KFbxFilterSet& KFbxCharacter::GetFilterSet().
  *
  */
class KFBX_DLL KFbxFilterSet
{

public:

	/** \enum EParameterModeType     Parameter mode types.
      * - \e eNoMode                 Parameter has no mode and is always active.
      * - \e eOffOn                  Parameter can be on or off.
      * - \e eOffAutoUser            Parameter can be off, auto or user.
      * - \e eAutoUser               Parameter can be auto or user.
      */
	typedef enum 
	{
		eNoMode,		
		eOffOn,			
		eOffAutoUser,	
		eAutoUser		
	} EParameterModeType;

	/** \enum EParameterValueType     Parameter value types.
	  * - \e eScalar                  Value contains a single element.
	  * - \e eVector                  Value contains a vector of 3 elements.
	  */
	typedef enum 
	{ 
		eScalar,
		eVector
	} EParameterValueType;

	/** \enum EParameterUnit     Parameter units.
      * - \e eNoUnit             Parameter has no unit.
	  * - \e ePercent            Parameter is expressed as a percent.
      * - \e eSecond             Parameter is expressed in seconds.
	  * - \e eCentimeter         Parameter is expressed in centimeters.
      * - \e eDegree             Parameter is expressed in degrees.
      */
	typedef enum 
	{ 
		eNoUnit,
		ePercent,
		eSecond,
		eCentimeter,
		eDegree
	} EParameterUnit;

    /** Reset all parameter to default values.
      */
	void    Reset();

	/** Set parameter mode.
      * \param pParameterIndex     Parameter index.
      * \param pParameterMode      Parameter mode.
	  *                                 - If parameter mode type is eOffOn: 0 is off, 1 is on.
	  *                                 - If parameter mode type is eOffAutoUser: 0 is off, 1 is auto, 2 is user.
	  *                                 - If parameter mode type is eAutoUser: 0 is auto, 1 is user.
	  * \remarks                   This function has no effect if the given pParameterMode is not allowed.
	  */
	void    SetParameterMode(int pParameterIndex, int pParameterMode);
	
	/** Get parameter mode.
      * \param pParameterIndex     Parameter index.
      * \return                    Parameter mode.
	  *                                 - If parameter mode type is eOffOn: 0 is off, 1 is on.
	  *                                 - If parameter mode type is eOffAutoUser: 0 is off, 1 is auto, 2 is user.
	  *                                 - If parameter mode type is eAutoUser: 0 is auto, 1 is user.
	  */
	int    GetParameterMode(int pParameterIndex);
	
	/** Set parameter value.
	  * \param pParameterIndex     Parameter index.
	  * \param pValue              Parameter value.
	  * \param pIndex              Value index. 
      *                                 - If value type is eScalar, index can only equal 0.
      *                                 - If value type is eVector, index can equal 0, 1 or 2.
	  */ 
	void    SetParameterValue(int pParameterIndex, double pValue, int pIndex = 0);	

	/** Get parameter value.
	  * \param pParameterIndex     Parameter index.
	  * \param pIndex              Value index.
      *                                 - If value type is eScalar, index can only equal 0.
      *                                 - If value type is eVector, index can equal 0, 1 or 2.
	  * \return                    Parameter value.
	  */ 
	double GetParameterValue(int pParameterIndex, int pIndex = 0);	

	
	/** Get number of parameters.
      * \return     Total parameter count.
      */
	static int                 GetParameterCount();

	/** Get parameter name.
      * \param pParameterIndex     Parameter index.
      * \return                    Parameter name.
      */
	static char*             GetParameterName(int pParameterIndex);
	
	/** Get parameter index by name.
      * \param pParameterName     Parameter name.
      * \return                   Parameter index.
	  * \remarks                  If no parameter with pParameterName exists, -1 is returned as the index.
	  */
	static int                 GetParameterIndex(char* pParameterName);

	/** Get parameter mode type.
      * \param pParameterIndex     Parameter index.
      * \return                    Parameter mode type, see enum EParameterModeType for possible values.
      */
	static EParameterModeType   GetParameterModeType(int pParameterIndex);

	/** Get parameter default mode.
      * \param pParameterIndex     Parameter index.
      * \return                    Default parameter mode
	  *                                 - If parameter mode type is eOffOn: 0 is off, 1 is on.
	  *                                 - If parameter mode type is eOffAutoUser: 0 is off, 1 is auto, 2 is user.
	  *                                 - If parameter mode type is eAutoUser: 0 is auto, 1 is user.
	  */
	static int                 GetParameterDefaultMode(int pParameterIndex);

	/** Get parameter value type.
      * \param pParameterIndex      Parameter index.
      * \return                     Parameter value type, see enum EParameterValueType for possible value.
      */
	static EParameterValueType  GetParameterValueType(int pParameterIndex);

	/** Get parameter unit.
      * \param pParameterIndex     Parameter index.
      * \return                    Parameter unit type, see enum EParameterUnit for possible value.
      */
	static EParameterUnit       GetParameterUnit(int pParameterIndex);

	/** Get parameter default value.
      * \param pParameterIndex     Parameter index.
      * \return                    Default parameter value.
      */
	static double              GetParameterDefaultValue(int pParameterIndex);

    /** Get parameter minimum value.
      * \param pParameterIndex     Parameter index.
	  * \return                    Parameter minimum value.
      */
	static double              GetParameterMinValue(int pParameterIndex);

	/** Get parameter maximum value.
      * \param pParameterIndex     Parameter index.
	  * \return                    Parameter maximum value.
      */
	static double              GetParameterMaxValue(int pParameterIndex);    

///////////////////////////////////////////////////////////////////////////////
//
//  WARNING!
//
//	Anything beyond these lines may not be documented accurately and is 
// 	subject to change without notice.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef DOXYGEN_SHOULD_SKIP_THIS	

private:

	KFbxFilterSet();
	~KFbxFilterSet();

	KFilterSetBase* mFilterSetBase;

	friend class KFbxCharacter;

#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS

};

#include <fbxfilesdk_nsend.h>

#endif // #ifndef _FBXSDK_FILTER_SET_H_


