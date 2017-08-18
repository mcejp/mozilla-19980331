/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */
/*******************************************************************************
 * Source date: 16 Apr 1997 08:37:58 GMT
 * netscape/fonts/cfb module C header file
 * Generated by jmc version 1.8 -- DO NOT EDIT
 ******************************************************************************/

#ifndef _Mcfb_H_
#define _Mcfb_H_

#include "jmc.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * cfb
 ******************************************************************************/

/* The type of the cfb interface. */
struct cfbInterface;

/* The public type of a cfb instance. */
typedef struct cfb {
	const struct cfbInterface*	vtable;
} cfb;

/* The inteface ID of the cfb interface. */
#ifndef JMC_INIT_cfb_ID
extern EXTERN_C_WITHOUT_EXTERN const JMCInterfaceID cfb_ID;
#else
EXTERN_C const JMCInterfaceID cfb_ID = { 0x737a3d23, 0x59012641, 0x5e2b332a, 0x051a4050 };
#endif /* JMC_INIT_cfb_ID */
/*******************************************************************************
 * cfb Operations
 ******************************************************************************/

#define cfb_getInterface(self, a, exception)	\
	(((self)->vtable->getInterface)(self, cfb_getInterface_op, a, exception))

#define cfb_addRef(self, exception)	\
	(((self)->vtable->addRef)(self, cfb_addRef_op, exception))

#define cfb_release(self, exception)	\
	(((self)->vtable->release)(self, cfb_release_op, exception))

#define cfb_hashCode(self, exception)	\
	(((self)->vtable->hashCode)(self, cfb_hashCode_op, exception))

#define cfb_equals(self, a, exception)	\
	(((self)->vtable->equals)(self, cfb_equals_op, a, exception))

#define cfb_clone(self, exception)	\
	(((self)->vtable->clone)(self, cfb_clone_op, exception))

#define cfb_toString(self, exception)	\
	(((self)->vtable->toString)(self, cfb_toString_op, exception))

#define cfb_finalize(self, exception)	\
	(((self)->vtable->finalize)(self, cfb_finalize_op, exception))

#define cfb_LookupFont(self, a, b, c, exception)	\
	(((self)->vtable->LookupFont)(self, cfb_LookupFont_op, a, b, c, exception))

#define cfb_CreateFontFromUrl(self, a, b, c, d, e, f, exception)	\
	(((self)->vtable->CreateFontFromUrl)(self, cfb_CreateFontFromUrl_op, a, b, c, d, e, f, exception))

#define cfb_CreateFontFromFile(self, a, b, c, d, exception)	\
	(((self)->vtable->CreateFontFromFile)(self, cfb_CreateFontFromFile_op, a, b, c, d, exception))

#define cfb_ListFonts(self, a, b, exception)	\
	(((self)->vtable->ListFonts)(self, cfb_ListFonts_op, a, b, exception))

#define cfb_ListSizes(self, a, b, exception)	\
	(((self)->vtable->ListSizes)(self, cfb_ListSizes_op, a, b, exception))

#define cfb_GetBaseFont(self, a, exception)	\
	(((self)->vtable->GetBaseFont)(self, cfb_GetBaseFont_op, a, exception))

/*******************************************************************************
 * cfb Interface
 ******************************************************************************/

struct netscape_jmc_JMCInterfaceID;
struct java_lang_Object;
struct java_lang_String;
struct netscape_fonts_nfrc;
struct netscape_fonts_nffmi;
struct netscape_jmc_ConstCString;
struct netscape_fonts_nff;
struct netscape_fonts_nfdoer;
struct netscape_fonts_MWCntxStar;
struct netscape_fonts_nfrf;

struct cfbInterface {
	void*	(*getInterface)(struct cfb* self, jint op, const JMCInterfaceID* a, JMCException* *exception);
	void	(*addRef)(struct cfb* self, jint op, JMCException* *exception);
	void	(*release)(struct cfb* self, jint op, JMCException* *exception);
	jint	(*hashCode)(struct cfb* self, jint op, JMCException* *exception);
	jbool	(*equals)(struct cfb* self, jint op, void* a, JMCException* *exception);
	void*	(*clone)(struct cfb* self, jint op, JMCException* *exception);
	const char*	(*toString)(struct cfb* self, jint op, JMCException* *exception);
	void	(*finalize)(struct cfb* self, jint op, JMCException* *exception);
	struct nff*	(*LookupFont)(struct cfb* self, jint op, struct nfrc* a, struct nffmi* b, const char* c, JMCException* *exception);
	struct nff*	(*CreateFontFromUrl)(struct cfb* self, jint op, struct nfrc* a, const char* b, const char* c, jint d, struct nfdoer* e, MWContext * f, JMCException* *exception);
	struct nff*	(*CreateFontFromFile)(struct cfb* self, jint op, struct nfrc* a, const char* b, const char* c, const char* d, JMCException* *exception);
	void*	(*ListFonts)(struct cfb* self, jint op, struct nfrc* a, struct nffmi* b, JMCException* *exception);
	void*	(*ListSizes)(struct cfb* self, jint op, struct nfrc* a, struct nffmi* b, JMCException* *exception);
	struct nff*	(*GetBaseFont)(struct cfb* self, jint op, struct nfrf* a, JMCException* *exception);
};

/*******************************************************************************
 * cfb Operation IDs
 ******************************************************************************/

typedef enum cfbOperations {
	cfb_getInterface_op,
	cfb_addRef_op,
	cfb_release_op,
	cfb_hashCode_op,
	cfb_equals_op,
	cfb_clone_op,
	cfb_toString_op,
	cfb_finalize_op,
	cfb_LookupFont_op,
	cfb_CreateFontFromUrl_op,
	cfb_CreateFontFromFile_op,
	cfb_ListFonts_op,
	cfb_ListSizes_op,
	cfb_GetBaseFont_op
} cfbOperations;

/*******************************************************************************
 * Writing your C implementation: "cfb.h"
 * *****************************************************************************
 * You must create a header file named "cfb.h" that implements
 * the struct cfbImpl, including the struct cfbImplHeader
 * as it's first field:
 * 
 * 		#include "Mcfb.h" // generated header
 * 
 * 		struct cfbImpl {
 * 			cfbImplHeader	header;
 * 			<your instance data>
 * 		};
 * 
 * This header file will get included by the generated module implementation.
 ******************************************************************************/

/* Forward reference to the user-defined instance struct: */
typedef struct cfbImpl	cfbImpl;

struct nffbp;
struct nffbu;

/* This struct must be included as the first field of your instance struct: */
typedef struct cfbImplHeader {
	const struct nffbuInterface*	vtablenffbu;
	const struct nffbpInterface*	vtablenffbp;
	const struct cfbInterface*	vtablecfb;
	jint		refcount;
} cfbImplHeader;

/*******************************************************************************
 * Instance Casting Macros
 * These macros get your back to the top of your instance, cfb,
 * given a pointer to one of its interfaces.
 ******************************************************************************/

#undef  cfbImpl2nffbc
#define cfbImpl2nffbc(cfbImplPtr) \
	((nffbc*)((char*)(cfbImplPtr) + offsetof(cfbImplHeader, vtablecfb)))

#undef  nffbc2cfbImpl
#define nffbc2cfbImpl(nffbcPtr) \
	((cfbImpl*)((char*)(nffbcPtr) - offsetof(cfbImplHeader, vtablecfb)))

#undef  cfbImpl2cfb
#define cfbImpl2cfb(cfbImplPtr) \
	((cfb*)((char*)(cfbImplPtr) + offsetof(cfbImplHeader, vtablecfb)))

#undef  cfb2cfbImpl
#define cfb2cfbImpl(cfbPtr) \
	((cfbImpl*)((char*)(cfbPtr) - offsetof(cfbImplHeader, vtablecfb)))

#undef  cfbImpl2Object
#define cfbImpl2Object_1(cfbImplPtr) \
	((Object*)((char*)(cfbImplPtr) + offsetof(cfbImplHeader, vtablenffbp)))

#undef  Object2cfbImpl
#define Object2cfbImpl_1(ObjectPtr) \
	((cfbImpl*)((char*)(ObjectPtr) - offsetof(cfbImplHeader, vtablenffbp)))

#undef  cfbImpl2nffbp
#define cfbImpl2nffbp(cfbImplPtr) \
	((nffbp*)((char*)(cfbImplPtr) + offsetof(cfbImplHeader, vtablenffbp)))

#undef  nffbp2cfbImpl
#define nffbp2cfbImpl(nffbpPtr) \
	((cfbImpl*)((char*)(nffbpPtr) - offsetof(cfbImplHeader, vtablenffbp)))

#undef  cfbImpl2Object
#define cfbImpl2Object_2(cfbImplPtr) \
	((Object*)((char*)(cfbImplPtr) + offsetof(cfbImplHeader, vtablenffbu)))

#undef  Object2cfbImpl
#define Object2cfbImpl_2(ObjectPtr) \
	((cfbImpl*)((char*)(ObjectPtr) - offsetof(cfbImplHeader, vtablenffbu)))

#undef  cfbImpl2nffbu
#define cfbImpl2nffbu(cfbImplPtr) \
	((nffbu*)((char*)(cfbImplPtr) + offsetof(cfbImplHeader, vtablenffbu)))

#undef  nffbu2cfbImpl
#define nffbu2cfbImpl(nffbuPtr) \
	((cfbImpl*)((char*)(nffbuPtr) - offsetof(cfbImplHeader, vtablenffbu)))

/*******************************************************************************
 * Operations you must implement
 ******************************************************************************/

struct netscape_fonts_nffp;
struct netscape_fonts_nfFontObserverCallback;
struct netscape_fonts_MWContextStar;

extern JMC_PUBLIC_API(void*)
_cfb_getBackwardCompatibleInterface(struct cfb* self, const JMCInterfaceID* iid,
	JMCException* *exception);

extern JMC_PUBLIC_API(void)
_cfb_init(struct cfb* self, JMCException* *exception);

extern JMC_PUBLIC_API(void*)
_cfb_getInterface(struct cfb* self, jint op, const JMCInterfaceID* a, JMCException* *exception);

extern JMC_PUBLIC_API(void)
_cfb_addRef(struct cfb* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(void)
_cfb_release(struct cfb* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_hashCode(struct cfb* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(jbool)
_cfb_equals(struct cfb* self, jint op, void* a, JMCException* *exception);

extern JMC_PUBLIC_API(void*)
_cfb_clone(struct cfb* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(const char*)
_cfb_toString(struct cfb* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(void)
_cfb_finalize(struct cfb* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(struct nff*)
_cfb_LookupFont(struct cfb* self, jint op, struct nfrc* a, struct nffmi* b, const char* c, JMCException* *exception);

extern JMC_PUBLIC_API(struct nff*)
_cfb_CreateFontFromUrl(struct cfb* self, jint op, struct nfrc* a, const char* b, const char* c, jint d, struct nfdoer* e, MWContext * f, JMCException* *exception);

extern JMC_PUBLIC_API(struct nff*)
_cfb_CreateFontFromFile(struct cfb* self, jint op, struct nfrc* a, const char* b, const char* c, const char* d, JMCException* *exception);

extern JMC_PUBLIC_API(void*)
_cfb_ListFonts(struct cfb* self, jint op, struct nfrc* a, struct nffmi* b, JMCException* *exception);

extern JMC_PUBLIC_API(void*)
_cfb_ListSizes(struct cfb* self, jint op, struct nfrc* a, struct nffmi* b, JMCException* *exception);

extern JMC_PUBLIC_API(struct nff*)
_cfb_GetBaseFont(struct cfb* self, jint op, struct nfrf* a, JMCException* *exception);

extern JMC_PUBLIC_API(void*)
_cfb_nffbp_getInterface(struct nffbp* self, jint op, const JMCInterfaceID* a, JMCException* *exception);

extern JMC_PUBLIC_API(void)
_cfb_nffbp_addRef(struct nffbp* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(void)
_cfb_nffbp_release(struct nffbp* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_nffbp_hashCode(struct nffbp* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(jbool)
_cfb_nffbp_equals(struct nffbp* self, jint op, void* a, JMCException* *exception);

extern JMC_PUBLIC_API(void*)
_cfb_nffbp_clone(struct nffbp* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(const char*)
_cfb_nffbp_toString(struct nffbp* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(void)
_cfb_nffbp_finalize(struct nffbp* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_nffbp_RegisterFontDisplayer(struct nffbp* self, jint op, struct nffp* a, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_nffbp_CreateFontDisplayerFromDLM(struct nffbp* self, jint op, const char* a, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_nffbp_ScanForFontDisplayers(struct nffbp* self, jint op, const char* a, JMCException* *exception);

extern JMC_PUBLIC_API(void)
_cfb_nffbp_RfDone(struct nffbp* self, jint op, struct nfrf* a, JMCException* *exception);

extern JMC_PUBLIC_API(void*)
_cfb_nffbu_getInterface(struct nffbu* self, jint op, const JMCInterfaceID* a, JMCException* *exception);

extern JMC_PUBLIC_API(void)
_cfb_nffbu_addRef(struct nffbu* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(void)
_cfb_nffbu_release(struct nffbu* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_nffbu_hashCode(struct nffbu* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(jbool)
_cfb_nffbu_equals(struct nffbu* self, jint op, void* a, JMCException* *exception);

extern JMC_PUBLIC_API(void*)
_cfb_nffbu_clone(struct nffbu* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(const char*)
_cfb_nffbu_toString(struct nffbu* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(void)
_cfb_nffbu_finalize(struct nffbu* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(struct nffmi*)
_cfb_nffbu_CreateFontMatchInfo(struct nffbu* self, jint op, const char* a, const char* b, const char* c, jint d, jint e, jint f, jint g, jint h, jint i, jint j, JMCException* *exception);

extern JMC_PUBLIC_API(struct nfrc*)
_cfb_nffbu_CreateRenderingContext(struct nffbu* self, jint op, jint a, jint b, void* * c, jsize c_length, JMCException* *exception);

extern JMC_PUBLIC_API(struct nfdoer*)
_cfb_nffbu_CreateFontObserver(struct nffbu* self, jint op, nfFontObserverCallback a, void* b, JMCException* *exception);

extern JMC_PUBLIC_API(void*)
_cfb_nffbu_malloc(struct nffbu* self, jint op, jint a, JMCException* *exception);

extern JMC_PUBLIC_API(void)
_cfb_nffbu_free(struct nffbu* self, jint op, void* a, JMCException* *exception);

extern JMC_PUBLIC_API(void*)
_cfb_nffbu_realloc(struct nffbu* self, jint op, void* a, jint b, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_nffbu_IsWebfontsEnabled(struct nffbu* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_nffbu_EnableWebfonts(struct nffbu* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_nffbu_DisableWebfonts(struct nffbu* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(void*)
_cfb_nffbu_ListFontDisplayers(struct nffbu* self, jint op, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_nffbu_IsFontDisplayerEnabled(struct nffbu* self, jint op, const char* a, JMCException* *exception);

extern JMC_PUBLIC_API(void*)
_cfb_nffbu_ListFontDisplayersForMimetype(struct nffbu* self, jint op, const char* a, JMCException* *exception);

extern JMC_PUBLIC_API(const char*)
_cfb_nffbu_FontDisplayerForMimetype(struct nffbu* self, jint op, const char* a, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_nffbu_EnableFontDisplayer(struct nffbu* self, jint op, const char* a, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_nffbu_DisableFontDisplayer(struct nffbu* self, jint op, const char* a, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_nffbu_EnableMimetype(struct nffbu* self, jint op, const char* a, const char* b, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_nffbu_DisableMimetype(struct nffbu* self, jint op, const char* a, const char* b, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_nffbu_LoadCatalog(struct nffbu* self, jint op, const char* a, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_nffbu_SaveCatalog(struct nffbu* self, jint op, const char* a, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_nffbu_LoadWebfont(struct nffbu* self, jint op, MWContext * a, const char* b, jint c, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_nffbu_ReleaseWebfonts(struct nffbu* self, jint op, MWContext * a, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_nffbu_WebfontsNeedReload(struct nffbu* self, jint op, MWContext * a, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_nffbu_LookupFailed(struct nffbu* self, jint op, MWContext * a, struct nfrc* b, struct nffmi* c, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_nffbu_ToUnicode(struct nffbu* self, jint op, const char* a, jbyte * b, jsize b_length, jshort * c, jsize c_length, JMCException* *exception);

extern JMC_PUBLIC_API(jint)
_cfb_nffbu_UnicodeLen(struct nffbu* self, jint op, const char* a, jbyte * b, jsize b_length, JMCException* *exception);

/*******************************************************************************
 * Factory Operations
 ******************************************************************************/

JMC_PUBLIC_API(cfb*)
cfbFactory_Create(JMCException* *exception);

/******************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* _Mcfb_H_ */
