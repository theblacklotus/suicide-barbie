/*!  \file kstring.h
 */

#ifndef _FBXSDK_KSTRING_H_
#define _FBXSDK_KSTRING_H_

/**************************************************************************************

 Copyright © 1995 - 2006 Autodesk, Inc. and/or its licensors.
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
#include <kbaselib_h.h>

#if defined(KARCH_ENV_WIN32)

// Conversions between WideChar, UTF8 and ANSI
	#define UTF8_to_WIDECHAR( inUTF8,outWideChar ) \
		wchar_t outWideChar[1024]; \
        MultiByteToWideChar( CP_UTF8, 0, inUTF8, -1, outWideChar, 1024 );

	#define WIDECHAR_to_UTF8( inWideChar,outUTF8 ) \
		char outUTF8[1024]; \
        WideCharToMultiByte( CP_UTF8,0,inWideChar,-1,outUTF8,1024,NULL,NULL);

	#define WIDECHAR_to_ANSI( inWideChar,outANSI ) \
		char outANSI[1024]; \
        WideCharToMultiByte( CP_ACP,0,inWideChar,-1,outANSI,1024,NULL,NULL);

	#define ANSI_to_WIDECHAR( inANSI,outWideChar ) \
		wchar_t outWideChar[1024]; \
        MultiByteToWideChar( CP_ACP, 0, inANSI, -1, outWideChar, 1024 );

	#define ANSI_to_UTF8( inANSI,outUTF8 ) { \
        wchar_t tempWideChar[1024]; \
        MultiByteToWideChar( CP_ACP, 0, inANSI, -1, tempWideChar, 1024); \
        WideCharToMultiByte( CP_UTF8, 0, tempWideChar, -1, outUTF8, 1024, NULL, NULL); \
	}

	#define UTF8_to_ANSI( inUTF8,outANSI, dummy, dummy1 ) { \
        wchar_t tempWideChar[1024]; \
        MultiByteToWideChar( CP_UTF8, 0, inUTF8, -1, tempWideChar, 1024); \
        WideCharToMultiByte( CP_ACP, 0, tempWideChar, -1, outANSI, 1024, NULL, NULL); \
    }
#endif


#include <kbaselib_nsbegin.h>

#if !defined(KARCH_ENV_WIN32)    
	KBASELIB_DLL void UTF8_to_ANSI( const char* pSrc, char* pDest, int pDestSize, char pSubstitute = '.' );
#endif

    #define KStringStaticSize 64

    /**	Utility class to manipulate strings.
    * \nosubgrouping
    */
    class KBASELIB_DLL KString
    {
    public:
	    /**
	    * \name Constructors and Destructor
	    */
	    //@{
	    //! Create an instance.
	    static KString		*Create();
	    //! Create an instance if not already allocated ( is null )
	    static KString		*Create(KString *pString);
	    //! Destroy an allocated version of the string
	    void				Destroy();
	    //! Destroy the allocated space if empty
	    static KString* DestroyIfEmpty(KString *pString);
	    //! Destroy the allocated space if empty
	    static KString* StringOrEmpty(KString *pString);
	    //@}

    public:

	    /**
	    * \name Constructors and Destructor
	    */
	    //@{

	    //! Default constructor.
	    KString();

	    //! Copy constructor.
	    KString(const KString& pStr);

	    //! String constructor.
	    KString(const char* pStr);

	    //! Character constructor.
	    KString(char pChar, int pNbRepeat = 1);

	    //! String constructor with maximum length.
	    KString(const char* pCharPtr, int pLength);

	    //! Int constructor.
	    KString(const int pValue);

	    //! Float constructor.
	    KString(const float pValue);

	    //! Double constructor.
	    KString(const double pValue);

	    //! Destructor.
	    ~KString();

	    //@}

	    /**
	    * \name Instance Validation.
	    */
	    //@{

	    //! Return \c true if string is valid.
	    bool IsOK() const;

	    //! Invalidate string.
	    KString &Invalidate();

	    //! Get string length like "C" strlen().
	    int GetLen() const;

	    //! Return \c true if string is of length 0.
	    bool IsEmpty() const;

	    //! Discard the content of the object.
	    KString  &Empty();   

	    //@}

	    /**
	    * \name Buffer Access
	    */
	    //@{

	    //! Access by reference.
	    char& operator[](int pIndex);

	    //! Access by copy.
	    char operator[](int pIndex) const;    

	    //! Cast operator.
	    operator const char*() const;

	    //! Uniform non-UNICODE support.
	    char* ToChars() const;

	    //! Non-const buffer access.
	    char* Buffer();     

	    //! Non-const buffer access, same as Buffer().
	    char* GetBuffer();

	    //@}

	    /**
	    * \name Assignement Operators
	    */
	    //@{

	    //! KString assignment operator.
	    const KString& operator=(const KString& pStr);

	    //! Character assignment operator.
	    const KString& operator=(char pChar);

	    //! String assignment operator.
	    const KString& operator=(const char* pStr);

	    //! Int assignment operator.
	    const KString& operator=(int  pValue);

	    //! Float assignment operator.
	    const KString& operator=(float pValue);

	    //! Double assignment operator.
	    const KString& operator=(double pValue);

	    //! String assignment function.
	    const KString& Copy(int pLen, const char* pStr);

	    //@}

	    /**
	    * \name Append and Concatenation
	    */
	    //@{

	    //! Append as "C" strcat().
	    const KString& Append(const char* pStr); 

	    //! Append as "C" strncat().
	    const KString& AppendN(const char* pStr, int pLength); 

	    //! KString append.
	    const KString& operator+=(const KString& pKStr);

	    //! Character append.
	    const KString& operator+=(char pChar);

	    //! String append.
	    const KString& operator+=(const char* pStr);

	    //! Int append.
	    const KString& operator+=(int pValue);

	    //! Float append.
	    const KString& operator+=(float pValue);

	    //! KString concatenation.
	    friend KBASELIB_DLL KString operator+(const KString& pKStr1, const KString& pKStr2);

	    //! Character concatenation.
	    friend KBASELIB_DLL KString operator+(const KString& pKStr, char pChar);

	    //! String concatenation.
	    friend KBASELIB_DLL KString operator+(const KString& pKStr, const char* pStr);

	    //! Int concatenation.
	    friend KBASELIB_DLL KString operator+(const KString& pKStr, int pValue);

	    //! Float concatenation.
	    friend KBASELIB_DLL KString operator+(const KString& pKStr, float pValue);

	    //@}

	    /**
	    * \name String Comparison
	    */
	    //@{
    	
	    //! Compare as "C" strcmp().
	    int Compare(const char * pStr) const;        

	    //! Compare as "C" stricmp().
	    int CompareNoCase( const char * pStr ) const;   

	    //! Equality operator.
	    bool operator==(const KString &pStr) const;

	    //! Inequality operator.
	    bool operator!=(const KString &pStr) const;

	    //! Inferior to operator.
	    bool operator< (const KString &pStr) const;

	    //! Inferior or equal to operator.
	    bool operator<=(const KString &pStr) const;

	    //! Superior or equal to operator.
	    bool operator>=(const KString &pStr) const;

	    //! Superior to operator.
	    bool operator> (const KString &pStr) const;

	    //! Equality operator.
	    bool operator==(const char *pStr) const;

	    //! Inequality operator.
	    bool operator!=(const char *pStr) const;

	    //! Inferior to operator.
	    bool operator< (const char *pStr) const;

	    //! Inferior or equal to operator.
	    bool operator<=(const char *pStr) const;

	    //! Superior or equal to operator.
	    bool operator>=(const char *pStr) const;

	    //! Superior to operator.
	    bool operator> (const char *pStr) const;

	    //@}

	    /**
	    * \name Substring Extraction
	    */
	    //@{

	    //! Extract middle string for a given length.
	    KString Mid(int pFirst, int pCount) const;

	    //! Extract middle string up to the end.
	    KString Mid(int pFirst) const;

	    //! Extract left string.
	    KString Left(int pCount) const;

	    //! Extract right string.
	    KString Right(int pCount) const;

	    //@}

	    /**
	    * \name Padding
	    */
	    //@{

	    //! Padding types.
	    enum PaddingType { eRight, eLeft, eBoth };

	    //! Add padding characters.
	    KString Pad(PaddingType pPadding, int pLen, char pCar = ' ') const;

	    //! Remove padding characters.
	    KString UnPad(PaddingType pPadding) const;

	    //@}

	    /**
	    * \name Conversion
	    */
	    //@{

	    //! Uppercase conversion
	    KString Upper() const;

	    //! Lowercase conversion
	    KString Lower() const;

	    //! Reverse conversion
	    KString Reverse() const;

	    //! Convert to Unix, changes \\r\\n characters for a single \\n
	    KString ConvertToUnix() const;

	    //! Convert to Windows, changes \\n character for both \\r\\n
	    KString ConvertToWindows() const;

	    //@}

	    /**
	    * \name Search
	    */
	    //@{

	    /** Look for a single character match, like "C" strchr().
	    * \return Index or -1 if not found.
	    */
	    int Find(char pChar, int pStartPosition = 0) const; 

	    /** Look for a substring match, like "C" strstr().
	    * \return Starting index or -1 if not found.
	    */
	    int Find(const char* pStrSub, int pStartPosition = 0) const;

	    /** Look for a single character match, like "C" strrchr().
	    * \return Index or -1 if not found.
	    */
	    int ReverseFind(char pChar) const;

	    /** Look for a single character match, like "C" strpbrk().
	    * \return Index or -1 if not found.
	    */
	    int FindOneOf(const char * pStrCharSet, int pStartPosition = 0) const;

	    /** Replace a substring.
	    * \return \c true if substring found and replaced.
	    */
	    bool FindAndReplace(const char* pFind, const char* pReplaceBy, int pStartPosition = 0);

	    //@}

	    /**
	    * \name Token Extraction
	    */
	    //@{

	    //! Get number of tokens.
	    int GetTokenCount(const char* pSpans);

	    //! Get token at given index.
	    KString GetToken(int pTokenIndex, const char* pSpans);

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

    private:

	    // Lengths/sizes in characters. 
	    // Note: an extra character is always allocated.
	    char *mData; // Actual string (zero terminated).
	    int mDataLen; // Does not include terminating 0.
	    int mAllocLen; // Does not include terminating 0.
	    char mStaticData[KStringStaticSize]; // Actual static string (zero terminated) used as long as the string is smaller than KStringStaticSize

	    void Init();
	    void FreeBuffer();
	    void FreeBuffer(char *&pOldData);

	    bool AllocCopy(KString& pDest, int pCopyLen, int pCopyIndex) const;
	    bool AllocBuffer(int pLen);
	    bool AllocBuffer(int pLen, char *&pOldData);

	    bool AssignCopy(int pSrcLen, const char * pSrcData);
	    bool ConcatCopy(int pSrc1Len, const char * pSrc1Data, 
					    int pSrc2Len, const char * pSrc2Data);
	    bool ConcatInPlace(int pSrcLen, const char * pSrcData);

	    bool IsIn(char pChar, const char* pString);

    #endif // #ifndef DOXYGEN_SHOULD_SKIP_THIS

    };

    //! KString concatenation.
    KBASELIB_DLL KString operator+(const KString& pKStr1, const KString& pKStr2);

    //! Character concatenation.
    KBASELIB_DLL KString operator+(const KString& pKStr, char pChar);

    //! String concatenation.
    KBASELIB_DLL KString operator+(const KString& pKStr, const char* pStr);

    //! Int concatenation.
    KBASELIB_DLL KString operator+(const KString& pKStr, int pValue);

    //! Float concatenation.
    KBASELIB_DLL KString operator+(const KString& pKStr, float pValue);


    // Strip the first and last character of a string assuming they are quotation marks.
    inline void StripQuotationMarks(KString& str)
    {
	    str = str.Mid( 1, str.GetLen() - 2 );
    }

    inline void RemoveChar( KString& pString, char lToRemove )
    {
        int lPos = pString.ReverseFind( lToRemove );

        while( lPos >= 0 ) 
        {
            pString = pString.Left( lPos ) + pString.Mid( lPos + 1 );
            lPos = pString.ReverseFind( lToRemove );
        }
    }

#include <kbaselib_nsend.h>

#endif // #define _FBXSDK_KSTRING_H_

