/*!  \file kfbxdatastream.h
 */

#ifndef _FBXSDK_DATASTREAM_H_
#define _FBXSDK_DATASTREAM_H_

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

#include <klib/ktime.h>
#include <klib/kstring.h>



#include <fbxfilesdk_nsbegin.h>

class KFbxDataStream_internal;

/** This class handles streams of binary information
  * \nosubgrouping
  */
class KFBX_DLL KFbxDataStream
{
	public:

	//! Constructor.
	KFbxDataStream();

	//! Copy Constructor.
	KFbxDataStream(KFbxDataStream const &pDataStream);

	//! Destructor.
	~KFbxDataStream();

	/**
	  * \name Read
	  * The following functions let you read from a binary data stream.
	  */

	//@{

	/** Start reading the stream.
	  */
	bool ReadBegin();

	/** End reading the stream.
	  */
	bool ReadEnd();

	/** Reset the field read position.
	  */
	void FieldReadResetPosition();

	/** Return the number of fields.
	  */
	int FieldGetCount();

	/** Get the name of a field using an index indexed pFieldIndex.
	  * \param pFieldIndex
	  */
	char* FieldGetName(int pFieldIndex);

	/** Get number of instance field pFieldName has.
	  * \param pFieldName
	  */
	int FieldGetInstanceCount(char* pFieldName);

	/** Start to read field instance refered by field indexed pFieldIndex, instance indexed pInstance.
	  * \param pFieldName
	  * \param pInstance
	  */
	bool FieldReadBegin(int pFieldIndex, int pInstance);

	/** Start to read field pFieldName.
	  * \param pFieldName
	  */
	bool FieldReadBegin(char* pFieldName);

	/** Start to read field instance refered field pFieldName, instance indexed pInstance.
	  * \param pFieldName
	  * \param pInstance
	  */
	bool FieldReadBegin(char* pFieldName, int pInstance);

	/** Stop to read the current field.
	  */
	void FieldReadEnd();

	/** Return if current field is a block.
	  */
	bool FieldReadIsBlock();

	/** Start to read a field block.
	  */
	bool FieldReadBlockBegin();

	/** Stop to read a field block.
	  */
	void FieldReadBlockEnd();

	/** Return the number of read field.
	  */
	int FieldReadGetCount();

	/** Return the number of field remaining to be read.
	  */
	int FieldReadGetRemain();

	/** Return current field value type.
	  */
	char FieldReadGetType();

	/** Return current field value as a char.
	  */
	char FieldReadCH();

	/** Return field pFieldName's value as a char.
	  * \param pFieldName
	  * \param pDefault
	  */
	char FieldReadCH(char* pFieldName, char pDefault=0);

	/** Return current field value as a char pointer.
	  */
	char* FieldReadC();

	/** Return field pFieldName's value as a char pointer.
	  * \param pFieldName
	  * \param pDefault
	  */
	char* FieldReadC(char* pFieldName, char* pDefault="");

	/** Return current field value as a string (a char pointer).
	  */
	inline char* FieldReadS() {return FieldReadC ();}

	/** Return field pFieldName's value as a char pointer.
	  * \param pFieldName
	  * \param pDefault
	  */
	inline char* FieldReadS(char* pFieldName, char* pDefault="") {return FieldReadC (pFieldName, pDefault);}

	/** Return current field value as an bool.
	  */
	inline bool FieldReadB() { return FieldReadI() != 0; }

	/** Return field pFieldName's value as an integer.
	  * \param pFieldName
	  * \param pDefault
	  */
	inline bool FieldReadB(char* pFieldName, bool pDefault = false ) { return FieldReadI( pFieldName, pDefault ) != 0; }

	/** Return current field value as an integer.
	  */
	int FieldReadI();

	/** Return field pFieldName's value as an integer.
	  * \param pFieldName
	  * \param pDefault
	  */
	int FieldReadI(char* pFieldName, int pDefault=0);

	/** Return current field value as an integer.
	  */
	kLongLong FieldReadLL();

	/** Return field pFieldName's value as an integer.
	  * \param pFieldName
	  * \param pDefault
	  */
	kLongLong FieldReadLL(char* pFieldName, kLongLong pDefault=0);

	/** Return current field value as a float.
	  */
	float FieldReadF();

	/** Return field pFieldName's value as a float.
	  * \param pFieldName
	  * \param pDefault
	  */
	float FieldReadF(char* pFieldName, float pDefault=0);

	/** Return current field value as a double.
	  */
	double FieldReadD();

	/** Return field pFieldName's value as a double.
	  * \param pFieldName
	  * \param pDefault
	  */
	double FieldReadD(char* pFieldName, double pDefault=0);

	/** Return field pFieldName's value as a time value.
	  * \param pFieldName
	  */
	KTime FieldReadT(char* pFieldName);

	/** Return field pFieldName's value as a time value.
	  */
	KTime FieldReadT();

	/** Return field pFieldName's value as a timespan value.
	  * \param pFieldName
	  */
	KTimeSpan FieldReadTS(char* pFieldName);

	/** Return field pFieldName's value as a timespan value.
	  */
	KTimeSpan FieldReadTS();

	/** Return current field value as a n floats array.
	  * \param pDefault
	  * \param pn
	  */
	inline void FieldReadFn(float* pValue, kUInt pn) {for (kUInt c=0; c<pn; c++) {pValue[c] = FieldReadF ();}}

	/** Return current field value as a 3 floats array.
	  * \param pDefault
	  */
	inline void FieldRead3F(float* pValue) {FieldReadFn (pValue, 3);}

	/** Return current field value as a 4 floats array.
	  * \param pDefault
	  */
	inline void FieldRead4F(float* pValue) {FieldReadFn (pValue, 4);}

	/** Return field pFieldName's value as n floats array.
	  * \param pFieldName
	  * \param pValue
	  * \param pDefault
	  * \param pn
	  */
	void FieldReadFn(char* pFieldName, float* pValue, float *pDefault, kUInt pn);

	/** Return field pFieldName's value as 4 floats array.
	  * \param pFieldName
	  * \param pValue
	  * \param pDefault
	  */
	inline void FieldRead3F(char* pFieldName, float* pValue, float* pDefault=NULL) {FieldReadFn (pFieldName, pValue, pDefault, 3);} 

	/** Return field pFieldName's value as 3 floats array.
	  * \param pFieldName
	  * \param pValue
	  * \param pDefault
	  */
	inline void FieldRead4F(char* pFieldName, float* pValue, float* pDefault=NULL) {FieldReadFn (pFieldName, pValue, pDefault, 4);}

	/** Return current field value as a n doubles array.
	  * \param pValue
	  * \param pn
	  */
	inline void FieldReadDn(double* pValue, kUInt pn) {for (kUInt c=0; c<pn; c++) {pValue[c] = FieldReadD ();}}

	/** Return current field value as a 3 doubles array.
	  * \param pValue
	  */
	inline void FieldRead3D(double* pValue) {FieldReadDn (pValue, 3);}

	/** Return current field value as a 4 doubles array.
	  * \param pValue
	  */
	inline void FieldRead4D(double* pValue) {FieldReadDn (pValue, 4);}

	/** Return field pFieldName's value as n doubles array.
	  * \param pFieldName
	  * \param pValue
	  * \param pDefault
	  */
	void FieldReadDn(char* pFieldName, double* pValue, double *pDefault, kUInt pn);

	/** Return field pFieldName's value as 4 doubles array.
	  * \param pFieldName
	  * \param pValue
	  * \param pDefault
	  */
	inline void FieldRead3D(char* pFieldName, double* pValue, double* pDefault=NULL) {FieldReadDn (pFieldName, pValue, pDefault, 3);} 

	/** Return field pFieldName's value as 3 doubles array.
	  * \param pFieldName
	  * \param pValue
	  * \param pDefault
	  */
	inline void FieldRead4D(char* pFieldName, double* pValue, double* pDefault=NULL) {FieldReadDn (pFieldName, pValue, pDefault, 4);}

	/** Return current field value as raw data. Only works when file is binary.
	  * \param pByteSize
	  */
	void* FieldReadR(int* pByteSize);

	/** Return field pFieldName's value as raw data. Only works when file is binary.
	  * \param pFieldName
	  * \param pByteSize
	  */
	void* FieldReadR(char* pFieldName,int* pByteSize);

	//@}

	/**
	  * \name Write
	  * The following functions let you write to a binary data stream.
	  */

	//@{

	/** Start write a new stream
	  */
	bool WriteBegin();

	/** Completed stream writing
	  */
	bool WriteEnd();

	/** Start to write a field called pFieldName.
	  * \param pFieldName
	  */
	void FieldWriteBegin(char* pFieldName);

	/** Stop to write the current field.
	  */
	void FieldWriteEnd();

	/** Start to write a field block.
	  */
	void FieldWriteBlockBegin();

	/** Start to write an object reference field.
	  * \param pObjectType
	  * \param pName
	  * \param pSubType
	  */
	void FieldWriteObjectBegin(char* pObjectType, char* pName, char* pSubType=NULL);

	/** Stop to write an object reference field.
	  */
	void FieldWriteObjectEnd(); 

	/** Start to write a field block in file pFileName.
	* This function is disabled but kept accessible for the FB SDK.
	  * \param pFileName
	*/
	void FieldWriteBlockBegin(char* pFileName);

	/** Stop to write a block of field.
	  */
	void FieldWriteBlockEnd ();

	/** Write field value as a char.
	  * \param pValue
	  */
	void FieldWriteCH(char pValue);

	/** Write field pFieldName field with a char as a value.
	  * \param pFieldName
	  * \param pValue
	  */
	inline void FieldWriteCH(char* pFieldName, char pValue) {FieldWriteBegin (pFieldName); FieldWriteCH (pValue); FieldWriteEnd ();}

	/** Write field value as char pointer pValue.
	  * \param pValue
	  */
	void FieldWriteC(char* pValue);

	/** Write field pFieldName with a char pointer as a value.
	  * \param pFieldName
	  * \param pValue
	  */
	inline void FieldWriteC(char* pFieldName, char* pValue) {FieldWriteBegin (pFieldName); FieldWriteC (pValue); FieldWriteEnd ();}

	/** Write field value as KString pValue.
	  * \param pValue
	  */
	inline void FieldWriteS(KString pValue) {FieldWriteC (pValue.Buffer ());}

	/** Write field pFieldName field with a KString as a value.
	  * \param pFieldName
	  * \param pValue
	  */
	inline void FieldWriteS(char* pFieldName, KString pValue) {FieldWriteC (pFieldName, pValue.Buffer ());}

	/** Write field value as bool.
	  * \param pValue
	  */
	inline void FieldWriteB(bool pValue){ FieldWriteI( pValue ); }

	/** Write field pFieldName field with a bool value.
	  * \param pFieldName
	  * \param pValue
	  */
	inline void FieldWriteB(char* pFieldName, bool pValue) {FieldWriteBegin (pFieldName); FieldWriteB (pValue); FieldWriteEnd ();}

	/** Write field value as integer.
	  * \param pValue
	  */
	void FieldWriteI(int pValue);

	/** Write field pFieldName field with an int as a value.
	  * \param pFieldName
	  * \param pValue
	  */
	inline void FieldWriteI(char* pFieldName, int pValue) {FieldWriteBegin (pFieldName); FieldWriteI (pValue); FieldWriteEnd ();}

	/** Write field value as 64 bit integer.
	  * \param pValue
	  */
	void FieldWriteLL(kLongLong pValue);

	/** Write field pFieldName field with an 64 bit int as a value.
	  * \param pFieldName
	  * \param pValue
	  */
	inline void FieldWriteLL(char* pFieldName, kLongLong pValue) {FieldWriteBegin (pFieldName); FieldWriteLL (pValue); FieldWriteEnd ();}

	/** Write field value as float.
	  * \param pValue
	  * \remarks Only compatible with 1) MotionBuilder 4.0 and later 2) FBX SDK 3.6.1 and later.
	  */
	void FieldWriteF(float pValue);

	/** Write field pFieldName field with a float as a value.
	  * \param pFieldName
	  * \param pValue
	  * \remarks Only compatible with 1) MotionBuilder 4.0 and later 2) FBX SDK 3.6.1 and later.
	  */
	inline void FieldWriteF(char* pFieldName, float pValue) {FieldWriteBegin (pFieldName); FieldWriteF (pValue); FieldWriteEnd ();}

	/** Write field value as double.
	  * \param pValue
	  */
	void FieldWriteD(double  pValue);

	/** Write field pFieldName field with a double as a value.
	  * \param pFieldName
	  * \param pValue
	  */
	inline void FieldWriteD(char* pFieldName, double pValue) {FieldWriteBegin (pFieldName); FieldWriteD (pValue); FieldWriteEnd ();}

	/** Write field value as time value.
	  * \param pTime
	  */
	void FieldWriteT(KTime pTime);

	/** Write field pFieldName field with a time as a value.
	  * \param pFieldName
	  * \param pValue
	  */
	inline void FieldWriteT(char* pFieldName,KTime pValue) {FieldWriteBegin (pFieldName); FieldWriteT (pValue); FieldWriteEnd ();}

	/** Write field value as timespan value.
	  * \param pTimeSpan
	  */
	void FieldWriteTS(KTimeSpan pTimeSpan);

	/** Write field pFieldName field with a timespan as a value.
	  * \param pFieldName
	  * \param pValue
	  */
	inline void FieldWriteTS(char* pFieldName,KTimeSpan pValue) {FieldWriteBegin (pFieldName); FieldWriteTS (pValue); FieldWriteEnd ();}

	/** Write field value as an array of n floats (nF vector).
	  * \param pValue
	  * \param pn
	  */
	inline void FieldWriteFn(float* pValue, kUInt pn) {for (kUInt c=0; c<pn; c++) {FieldWriteF (pValue[c]);}}

	/** Write field pFieldName field with a array of n floats as a value.
	  * \param pFieldName
	  * \param pValue
	  * \param pn
	  */
	inline void FieldWriteFn(char* pFieldName, float* pValue, kUInt pn) {FieldWriteBegin (pFieldName); FieldWriteFn (pValue, pn); FieldWriteEnd ();}

	/** Write field value as an array of 3 floats (3F vector). 
	  * \param pValue
	  */
	inline void FieldWrite3F(float* pValue) {FieldWriteFn (pValue, 3);}

	/** Write field pFieldName field with a array of 3 floats as a value.
	  * \param pFieldName
	  * \param pValue
	  */
	inline void FieldWrite3F(char* pFieldName, float* pValue) {FieldWriteFn (pFieldName, pValue, 3);}

	/** Write field value as an array of 4 floats (4F vector).
	  * \param pValue
	  */
	inline void FieldWrite4F(float* pValue) {FieldWriteFn (pValue, 4);}

	/** Write field pFieldName field with a array of 4 floats as a value.
	  * \param pFieldName
	  * \param pValue
	  */
	inline void FieldWrite4F(char* pFieldName, float* pValue) {FieldWriteFn (pFieldName, pValue, 4);}

	/** Write field value as an array of n doubles (nD vector).
	  * \param pValue
	  * \param pn
	  */
	inline void FieldWriteDn(double* pValue, kUInt pn) {for (kUInt c=0; c<pn; c++) {FieldWriteD (pValue[c]);}}

	/** Write field pFieldName field with a array of n doubles as a value.
	  * \param pFieldName
	  * \param pValue
	  * \param pn
	  */
	inline void FieldWriteDn(char* pFieldName, double* pValue, kUInt pn) {FieldWriteBegin (pFieldName); FieldWriteDn (pValue, pn); FieldWriteEnd ();}

	/** Write field value as an array of 3 doubles (3D vector). 
	  * \param pValue
	  */
	inline void FieldWrite3D(double* pValue) {FieldWriteDn (pValue, 3);}

	/** Write field pFieldName field with a array of 3 doubles as a value.
	  * \param pFieldName
	  * \param pValue
	  */
	inline void FieldWrite3D(char* pFieldName, double* pValue) {FieldWriteDn (pFieldName, pValue, 3);}

	/** Write field value as an array of 4 doubles (4D vector).
	  * \param pValue
	  */
	inline void FieldWrite4D(double* pValue) {FieldWriteDn (pValue, 4);}

	/** Write field pFieldName field with a array of 4 doubles as a value.
	  * \param pFieldName
	  * \param pValue
	  */
	inline void FieldWrite4D(char* pFieldName, double* pValue) {FieldWriteDn (pFieldName, pValue, 4);}

	/** Write field value as a raw data. 
	  * \param pRawData
	  * \param pByteSize
	  * \remarks Only works when file is binary.
	  */
	void FieldWriteR(void* pRawData, int pByteSize);

	/** Write field pFieldName field with raw data as a value. 
	  * \param pFieldName
	  * \param pRawData
	  * \param pByteSize
	  * \remarks Only works when file is binary.
	  */
	inline void FieldWriteR(char* pFieldName, void* pRawData, int pByteSize) {FieldWriteBegin (pFieldName); FieldWriteR (pRawData, pByteSize); FieldWriteEnd ();}

	/** Write object reference pName in the current field.
	  * \param pName
	  */
	inline void FieldWriteObjectReference(char* pName) {FieldWriteC (pName);}

	/** Write object reference pName in field pFieldName.
	  * \param pFieldName
	  * \param pName
	  */
	inline void FieldWriteObjectReference(char* pFieldName, char* pName) {FieldWriteC (pFieldName, pName);}

	/** Write comments, only effective in ASCII mode.
	  * \param pFieldName
	  */
	void WriteComments(char* pFieldName);
	//@}


	//! Assignment operator.
	KFbxDataStream& operator=(const KFbxDataStream& pDataStream);

	private:
		KFbxDataStream_internal*	mInternal;
};

#include <fbxfilesdk_nsend.h>

#endif // #ifndef _FBXSDK_DATASTREAM_H_


