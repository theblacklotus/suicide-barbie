/*!  \file kfbxproperty.h
 */
#ifndef _KFbxProperty_h
#define _KFbxProperty_h

/**************************************************************************************

 Copyright © 2004 - 2006 Autodesk, Inc. and/or its licensors.
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

#ifndef MB_FBXSDK
#include <kbaselib_nsuse.h>
#endif
#include <kbaselib_forward.h>

#include <klib/kstring.h>
#include <kfbxplugins/kfbxplug.h>
#include <kfbxplugins/kfbxtypes.h>
#include <kfcurve/kfcurvenode.h>
#include <fbxfilesdk_nsbegin.h>

	class KFbxObject;

	class KFbxColor;
	class KFbxObject;
	class KFbxObject_internal;
	class KFbxProperty_internal;
	class KFbxSdkManager;

	/** \brief Class to hold user properties.
	  * \nosubgrouping
	  */
	class KFBX_DLL KFbxProperty : public KFbxPlug
	{
		KFBXPLUG_DECLARE(KFbxProperty);

		/**
		  * \name Constructor and Destructors.
		  */
		//@{
		public:
			/** Create a dynamic property
			  * \param pObject
			  * \param pName
			  * \param pDataType
			  * \param pLabel
			  */
			static KFbxProperty Create(KFbxObject* pObject, char const* pName, KFbxDataType const &pDataType=KFbxDataType(), char const* pLabel="");

			/** Create a dynamic property from an other property
			  * \param pObject
			  * \param pFromProperty
			  */
			static KFbxProperty Create(KFbxObject* pObject, KFbxProperty& pFromProperty);

			/** Create a dynamic property
			  * \param pCompoundProperty
			  * \param pName
			  * \param pDataType
			  * \param pLabel
			  */
			static KFbxProperty Create(KFbxProperty const &pCompoundProperty, char const* pName, KFbxDataType const &pDataType=KFbxDataType(), char const* pLabel="");

			/** Static Property Creation
			  * \param pObject
			  * \param pName
			  * \param pDataType
			  * \param pLabel
			  * \remarks Can only be used in the constuctor of an KFbxObject. For dynamic construction of property use the create function
			  */
			KFbxProperty const & StaticInit(KFbxObject* pObject, char const* pName, KFbxDataType const &pDataType, char const* pLabel);

			/** Destroy a dynamic property
			  */
			void Destroy(bool pRecursive = true, bool pDependents = false);

			/** Static Property Constructors 
			  */
			KFbxProperty();

			/** Copy constructor for properties
			  */
			KFbxProperty(KFbxProperty const &pProperty);

			/** Static Property destructor 
			  */
			~KFbxProperty();

		public:
		//@}
		/**
		  * \name Property Identification.
		  */
		//@{
		public:
			/** Get the property datatype definition.
			  * \return The properties KFbxDataType 
			  */
			KFbxDataType		GetPropertyDataType() const;

			/** Get the property internal name.
			  * \return Property internal name string.
			  */
			KString				GetName() const;

			/** Get the property label.
			  * \param pReturnNameIfEmpty If \c true, allow this method to return the internal name.
			  * \return The property label if set, or the property internal name if the pReturnNameIfEmpty
			  * flag is set to \c true and the label has not been defined.
			  * \remarks Some applications may choose to ignore the label field and work uniquely with the
			  * internal name. Therefore, it should not be taken for granted that a label exists. Also, remember
			  * that the label does not get saved in the FBX file. It only exist while the property object is
			  * in memory.
			  */
			KString				GetLabel(bool pReturnNameIfEmpty=true);


			/** Set a label to the property.
			  * \param pLabel Label string.
			  */
			void				SetLabel(KString pLabel);

			/** Get the object that contains the property.
			  * \return the property object owner or null if the property is an orphan.
			  */
			KFbxObject*			GetFbxObject();

			void				SetUserTag(int pTag);
			int					GetUserTag();
		//@}

		/**
		  * \name Property Initialization.
		  */
		//@{

			/** \enum EFlags Property attribute flags.
			  * - \e eNO_FLAG		No special attributes set.
			  * - \e eANIMATABLE	An animation FCurve can be attached to this property.
			  * - \e eUSER			User defined property.
			  * - \e ePUBLISHED		This property has been published for user connection ( only valid for MotionBuilder ).
			  */
			typedef enum
			{
				eNO_FLAG    = 0,
				eANIMATABLE = 1, 
				eUSER       = 1<<1,
				eTEMPORARY  = 1<<2,  // System property
				ePUBLISHED	= 1<<3, 
				eSTATIC		= 1<<4, 
			} EFlags;

			/** Change the attributes of the property.
			  * \param pFlag Property attribute identifier.
			  * \param pValue New state.
			  */
			void				ModifyFlag(EFlags pFlag, bool pValue);

			/** Get the property attribute state.
			  * \param pFlag Property attribute identifier.
			  * \return The currently set property attribute state.
			  */
			bool                GetFlag(EFlags pFlag);
		//@}

		//! Operators.
		KFbxProperty &		operator=  (KFbxProperty const &pKProperty);
		bool                operator==(KProperty const* pKProperty) const;
		bool                operator== (KFbxProperty const &pKProperty) const;
		bool                operator!= (KFbxProperty const &pKProperty) const;
		bool CompareValue(KFbxProperty const& pProp) const;

		/** Copy value of a property.
		  * \param pProp Property to get value from.
		  * \return true if value has been copied, false if not.
		  */
		bool CopyValue(KFbxProperty const& pProp);


		/**
		  * \name Value management.
		  */
		//@{
			bool			IsValid() const; 

			/** set value function 
			  * \return true if it was succesfull and type were compatible.
			  */
			bool Set(void const *pValue,EFbxType pValueType);

			/** get value function 
			  * \return true if it was succesfull and type were compatible.
			  */
			bool Get(void *pValue,EFbxType pValueType) const;

			// usefull set and get functions
			template <class T> inline bool  Set( T const &pValue )	{ return Set( &pValue,FbxTypeOf(pValue) ); }
			template <class T> inline T		Get( T const *pFBX_TYPE) const { T lValue; Get( &lValue,FbxTypeOf(lValue) ); return lValue; }
//			template <class T> inline T		Get( T *pValue) const			{ T lValue; Get( &lValue,FbxTypeOf(lValue) ); if (pValue) { *pValue= lValue; } return lValue; }
		//@}

		/**
		  * \name Property Limits.
		  * Property limits are provided for convenience if some applications desire to
		  * bound the range of possible values for a given type property. Note that these
		  * limits are meaningless for the boolean type. It is the responsibility of the
		  * calling application to implement the necessary instructions to limit the property.
		  */
		//@{
			/** Set the minimum limit value of the property.
			  * \param pMin Minimum value allowed.
			  */
			void				SetMinLimit(double pMin);

			/** Get the minimum limit value of the property.
			  * \return Currently set minimum limit value.
			  */
			double				GetMinLimit();

			/** Set the maximum limit value of the property.
			  * \param pMax Maximum value allowed.
			  */
			void				SetMaxLimit(double pMax);
			/** Get the maximum limit value of the property.
			  * \return Currently set maximum limit value.
			  */
			double				GetMaxLimit();

			/** Set the minimum and maximum limit value of the property.
			  * \param pMin Minimum value allowed.
			  * \param pMax Maximum value allowed.
			  */
			void				SetLimits(double pMin, double pMax);
		//@}

		/**
		  * \name Enum and property list 
		  */
		//@{
			/** Add a string value at the end of the list.
			  * \param pStringValue Value of the string to be added.
			  * \return The index in the list where the string was added.
			  * \remarks This function is only valid when the property type is eENUM.
			  * Empty strings are not allowed.
			  */
			int                 AddEnumValue(char const *pStringValue);

			/** Insert a string value at the specified index.
			  * \param pIndex Zero bound index.
			  * \param pStringValue Value of the string for the specified index.
			  * \remarks This function is only valid when the property type is eENUM.
			  * pIndex must be in the range [0, ListValueGetCount()].
			  * Empty strings are not allowed.
			  */
			void                InsertEnumValue(int pIndex, char const *pStringValue);

			/** Get the number of elements in the list.
			  * \return The number of elements in the list.
			  * \remarks This function will return -1 if the property type is not eENUM.
			  */
			int                 GetEnumCount();

			/** Set a string value for the specified index.
			  * \param pIndex Zero bound index.
			  * \param pStringValue Value of the string for the specified index.
			  * \remarks This function is only valid when the property type is eENUM.
			  * The function will assign the specified string to the specified index.
			  * A string value must exists at the specified index in order to be changed.
			  * Empty strings are not allowed.
			  */
			void                SetEnumValue(int pIndex, char const *pStringValue);

			/** Remove the string value at the specified index.
			  * \param pIndex of the string value to be removed.
			  */
			void                RemoveEnumValue(int pIndex);

			/** Get a string value for the specified index
			  * \param pIndex Zero bound index.
			  * \param pStringValue Value of the string for the specified index.
			  * \remarks This function is only valid when the property type is eENUM.
			  */
			char *				GetEnumValue(int pIndex);
		//@}

		/**
		  * \name Hierarchical properties
		  */
		//@{
			int				GetPropertyCount();
			KFbxProperty	GetProperty(int pIndex);
		//@}

		/**
		  * \name Array Management
		  */
		//@{
			bool			SetArraySize( int pSize, bool pVariableArray );
			int				GetArraySize();
			KFbxProperty	GetArrayItem(int pIndex);
			inline KFbxProperty	operator[](int pIndex) { return GetArrayItem(pIndex); }
		//@}

		/**
		  * \name FCurve Management
		  */
		//@{
			/** Create a KFCurveNode on a take
			  * \param pTakeName Name of the take to create the KFCurveNode on
			  */
			KFCurveNode* CreateKFCurveNode(const char* pTakeName=NULL);

			/** Get the KFCurveNode from a take
			  * \param pTakeName Name of the take to get the KFCurveNode from
			  * \param pCreateAsNeeded Create the KFCurveNode if not found.
			  * \return Pointer to the KFCurveNode of the proprety on the given take.
			  */
			KFCurveNode* GetKFCurveNode(bool pCreateAsNeeded=false, const char* pTakeName=NULL);
			

		//@}

		/**
		  * \name General Property Connection and Relationship Management
		  */
		//@{
			// Properties
			inline int			GetSrcPropertyCount		()										{ return GetFbxSrcCount(KFbxProperty::ClassId); }
			inline KFbxProperty	GetSrcProperty			(int pIndex=0)							{ return *((KFbxProperty *)GetFbxSrc(pIndex,KFbxProperty::ClassId)); }
			inline KFbxProperty	FindSrcProperty			(const char *pName,int pStartIndex=0)	{ return *((KFbxProperty *)FindFbxSrc(pName,KFbxProperty::ClassId,pStartIndex)); }
			inline bool			ConnectSrcProperty		(KFbxProperty const & pProperty)		{ return ConnectFbxSrc((KFbxPlug *)&pProperty);		}
			inline bool			IsConnectedSrcProperty		(KFbxProperty const & pProperty)	{ return IsConnectedFbxSrc((KFbxPlug *)&pProperty); }
			inline bool			DisconnectSrcProperty	(KFbxProperty const & pProperty)		{ return DisconnectFbxSrc((KFbxPlug *)&pProperty);	}

			inline int			GetDstPropertyCount		()										{ return GetFbxDstCount(KFbxProperty::ClassId); }
			inline KFbxProperty	GetDstProperty			(int pIndex=0)							{ return *((KFbxProperty *)GetFbxDst(pIndex,KFbxProperty::ClassId)); }
			inline KFbxProperty	FindDstProperty			(const char *pName,int pStartIndex=0)	{ return *((KFbxProperty *)FindFbxDst(pName,KFbxProperty::ClassId,pStartIndex)); }
			inline bool			ConnectDstProperty		(KFbxProperty const & pProperty)		{ return ConnectFbxDst((KFbxPlug *)&pProperty);		}
			inline bool			IsConnectedDstProperty	(KFbxProperty const & pProperty)		{ return IsConnectedFbxDst((KFbxPlug *)&pProperty); }
			inline bool			DisconnectDstProperty	(KFbxProperty const & pProperty)		{ return DisconnectFbxDst((KFbxPlug *)&pProperty);	}
		//@}

		virtual void  SetUserDataPtr(void* pUserData);
		virtual void* GetUserDataPtr();


		// Deprecated function calls
		typedef enum {   eUNIDENTIFIED,eBOOL,eREAL,eCOLOR,eINTEGER,eVECTOR,eLIST, eMATRIX} EUserPropertyType;

//		K_DEPRECATED static				KFbxProperty Create(KFbxSdkManager& pManager, char const* pName, KFbxDataType const &pDataType=KFbxDataType(), char const* pLabel="");
		K_DEPRECATED static char*		GetPropertyTypeName(EUserPropertyType pType);
		K_DEPRECATED char *				GetPropertyTypeName();
		K_DEPRECATED EUserPropertyType	GetPropertyType();

		K_DEPRECATED void				SetDefaultValue(bool pValue);
		K_DEPRECATED void				SetDefaultValue(double pValue);
		K_DEPRECATED void				SetDefaultValue(KFbxColor& pValue);
		K_DEPRECATED void				SetDefaultValue(int pValue);
		K_DEPRECATED void				SetDefaultValue(double pValue1, double pValue2, double pValue3);
		K_DEPRECATED void				GetDefaultValue(bool& pValue);
		K_DEPRECATED void				GetDefaultValue(double& pValue);
		K_DEPRECATED void				GetDefaultValue(KFbxColor& pValue);
		K_DEPRECATED void				GetDefaultValue(int& pValue);
		K_DEPRECATED void				GetDefaultValue(double& pValue1, double& pValue2, double& pValue3);

		///////////////////////////////////////////////////////////////////////////////
		//
		//  WARNING!
		//
		//	Anything beyond these lines may not be documented accurately and is 
		// 	subject to change without notice.
		//
		///////////////////////////////////////////////////////////////////////////////
		#ifndef DOXYGEN_SHOULD_SKIP_THIS
		protected:
			 /* The pLabel parameter is defined for backward compatibility. Older FBX files
				saved it along with the other attributes. Now, the label attribute exists only while
				the property object is in memory and is lost when saving to the FBX file.
			  */

			//! Constructor / Destructor
			static KFbxProperty* Create(KFbxSdkManager& pManager, KProperty* pProperty);
			KFbxProperty(KFbxSdkManager& pManager, KProperty* pProperty);
			KFbxProperty(KFbxSdkManager& pManager, char const* pName, KFbxDataType const &pDataType=KFbxDataType(), char const* pLabel="");
			KFbxProperty(KFbxObject* pObject, char const* pName, KFbxDataType const &pDataType=KFbxDataType(), char const* pLabel="");
			KFbxProperty(KFbxProperty_internal *pPropertyInfo);

			void Init(KFbxSdkManager& pManager, char const* pName, KFbxDataType const &pDataType=KFbxDataType(), char const* pLabel="");

			// Virtual plug overides
			virtual KFbxSdkManager*		GetFbxSdkManager();
			virtual KPlug*				GetInternalPlug(bool pCreateOnDemand=false);
			virtual KPlug const*		GetInternalPlug(bool pCreateOnDemand=false) const;
			bool						CreateKProperty();
			bool						DestroyKProperty();
			KProperty*					GetKProperty();
			bool						FbxPlugNotify(KFbxPlugEvent const &pEvent);

			void						SetFbxObject(KFbxObject* pKFbxObject);

//			void						SetKFbxObject_internal(KFbxObject_internal* pKFbxObject_internal);

			private:
				//! Internal management
				KFbxProperty_internal* mData;

			// Needed internal 
			friend class KFbxObject;
			friend class KFbxTakeNodeContainer;
			friend class KFbxObject_internal;
			friend class KFbxReaderFbx;

		#endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS 
	};

	typedef KFbxProperty* HKFbxProperty;

	template <class T> inline bool  KFbxSet( KFbxProperty &pProperty,T const &pValue )	{ return pProperty.Set( &pValue,FbxTypeOf(pValue) ); }
	template <class T> inline bool  KFbxGet( KFbxProperty const& pProperty,T &pValue )		{ return pProperty.Get( &pValue,FbxTypeOf(pValue) ); }
	template <class T> inline T		KFbxGet( KFbxProperty const& pProperty)					{ T pValue; pProperty.Get( &pValue,FbxTypeOf(pValue) ); return pValue; }

	template <class T> class KFbxTypedProperty : public KFbxProperty {
		public:
			inline KFbxTypedProperty() : KFbxProperty()											{}
			inline KFbxTypedProperty(KFbxProperty const &pProperty) : KFbxProperty(pProperty)	{}
			inline ~KFbxTypedProperty()															{}

			inline KFbxProperty const & StaticInit(KFbxObject* pObject, char const* pName,EFlags pFlags=eNO_FLAG) 
			{
				KFbxProperty::StaticInit(pObject, pName, GetFbxDataType(FbxTypeOf(*((T *)0))), "");
				ModifyFlag(pFlags, true);
				return *this;
			}
			inline KFbxProperty const & StaticInit(KFbxObject* pObject, char const* pName,T const &pValue,EFlags pFlags=eNO_FLAG) 
			{
				KFbxProperty::StaticInit(pObject, pName, GetFbxDataType(FbxTypeOf(*((T *)0))), "");
				Set(pValue);
				ModifyFlag(pFlags, true);
				return *this;
			}

			inline KFbxProperty const & StaticInit(KFbxObject* pObject, char const* pName, KFbxDataType const &pDataType,EFlags pFlags=eNO_FLAG) 
			{
				KFbxProperty::StaticInit(pObject, pName, pDataType, "");
				ModifyFlag(pFlags, true);
				return *this;
			}
			inline KFbxProperty const & StaticInit(KFbxObject* pObject, char const* pName, KFbxDataType const &pDataType,T const &pValue,EFlags pFlags=eNO_FLAG)
			{
				KFbxProperty::StaticInit(pObject, pName, pDataType, "");
				Set(pValue);
				ModifyFlag(pFlags, true);
				return *this;
			}
		public:
			KFbxTypedProperty &operator =(T const &pValue)		{ KFbxSet(*this,pValue); return *this; }
			bool	 Set(T const &pValue)	{ return KFbxSet(*this,pValue); }
			T		 Get() const			{ T lValue; KFbxGet(*this,lValue); return lValue; }

		friend class KFbxObject;
	};

	// For use with deprecated type functions 
	KFbxDataType						EUserPropertyTypeToDataType(KFbxProperty::EUserPropertyType);
	KFbxProperty::EUserPropertyType		DataTypeToEUserPropertyType(const KFbxDataType &pDataType);


	typedef KFbxTypedProperty<fbxBool1>			KFbxPropertyBool1;
	typedef KFbxTypedProperty<fbxInteger1>		KFbxPropertyInteger1;
	typedef KFbxTypedProperty<fbxDouble1>		KFbxPropertyDouble1;
	typedef KFbxTypedProperty<fbxDouble3>		KFbxPropertyDouble3;
	typedef KFbxTypedProperty<fbxDouble4>		KFbxPropertyDouble4;
	typedef KFbxTypedProperty<fbxString>		KFbxPropertyString;
 

#include <fbxfilesdk_nsend.h>

#endif // _KFbxProperty_h


