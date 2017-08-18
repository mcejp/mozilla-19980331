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
 * Source date: 9 Apr 1997 21:45:12 GMT
 * netscape/fonts/nfrf public interface
 * Generated by jmc version 1.8 -- DO NOT EDIT
 ******************************************************************************/

#ifndef _Mnfrf_H_
#define _Mnfrf_H_

#include "jmc.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * nfrf
 ******************************************************************************/

/* The type of the nfrf interface. */
struct nfrfInterface;

/* The public type of a nfrf instance. */
typedef struct nfrf {
	const struct nfrfInterface*	vtable;
} nfrf;

/* The inteface ID of the nfrf interface. */
#ifndef JMC_INIT_nfrf_ID
extern EXTERN_C_WITHOUT_EXTERN const JMCInterfaceID nfrf_ID;
#else
EXTERN_C const JMCInterfaceID nfrf_ID = { 0x472d303c, 0x3c0b096b, 0x7609161a, 0x402b5c49 };
#endif /* JMC_INIT_nfrf_ID */
/*******************************************************************************
 * nfrf Operations
 ******************************************************************************/

#define nfrf_getInterface(self, a, exception)	\
	(((self)->vtable->getInterface)(self, nfrf_getInterface_op, a, exception))

#define nfrf_addRef(self, exception)	\
	(((self)->vtable->addRef)(self, nfrf_addRef_op, exception))

#define nfrf_release(self, exception)	\
	(((self)->vtable->release)(self, nfrf_release_op, exception))

#define nfrf_hashCode(self, exception)	\
	(((self)->vtable->hashCode)(self, nfrf_hashCode_op, exception))

#define nfrf_equals(self, a, exception)	\
	(((self)->vtable->equals)(self, nfrf_equals_op, a, exception))

#define nfrf_clone(self, exception)	\
	(((self)->vtable->clone)(self, nfrf_clone_op, exception))

#define nfrf_toString(self, exception)	\
	(((self)->vtable->toString)(self, nfrf_toString_op, exception))

#define nfrf_finalize(self, exception)	\
	(((self)->vtable->finalize)(self, nfrf_finalize_op, exception))

#define nfrf_GetMatchInfo(self, exception)	\
	(((self)->vtable->GetMatchInfo)(self, nfrf_GetMatchInfo_op, exception))

#define nfrf_GetPointSize(self, exception)	\
	(((self)->vtable->GetPointSize)(self, nfrf_GetPointSize_op, exception))

#define nfrf_GetMaxWidth(self, exception)	\
	(((self)->vtable->GetMaxWidth)(self, nfrf_GetMaxWidth_op, exception))

#define nfrf_GetFontAscent(self, exception)	\
	(((self)->vtable->GetFontAscent)(self, nfrf_GetFontAscent_op, exception))

#define nfrf_GetFontDescent(self, exception)	\
	(((self)->vtable->GetFontDescent)(self, nfrf_GetFontDescent_op, exception))

#define nfrf_GetMaxLeftBearing(self, exception)	\
	(((self)->vtable->GetMaxLeftBearing)(self, nfrf_GetMaxLeftBearing_op, exception))

#define nfrf_GetMaxRightBearing(self, exception)	\
	(((self)->vtable->GetMaxRightBearing)(self, nfrf_GetMaxRightBearing_op, exception))

#define nfrf_SetTransformMatrix(self, a, a_length, exception)	\
	(((self)->vtable->SetTransformMatrix)(self, nfrf_SetTransformMatrix_op, a, a_length, exception))

#define nfrf_GetTransformMatrix(self, exception)	\
	(((self)->vtable->GetTransformMatrix)(self, nfrf_GetTransformMatrix_op, exception))

#define nfrf_MeasureText(self, a, b, c, c_length, d, d_length, exception)	\
	(((self)->vtable->MeasureText)(self, nfrf_MeasureText_op, a, b, c, c_length, d, d_length, exception))

#define nfrf_MeasureBoundingBox(self, a, b, c, c_length, d, exception)	\
	(((self)->vtable->MeasureBoundingBox)(self, nfrf_MeasureBoundingBox_op, a, b, c, c_length, d, exception))

#define nfrf_DrawText(self, a, b, c, d, e, e_length, exception)	\
	(((self)->vtable->DrawText)(self, nfrf_DrawText_op, a, b, c, d, e, e_length, exception))

#define nfrf_PrepareDraw(self, a, exception)	\
	(((self)->vtable->PrepareDraw)(self, nfrf_PrepareDraw_op, a, exception))

#define nfrf_EndDraw(self, a, exception)	\
	(((self)->vtable->EndDraw)(self, nfrf_EndDraw_op, a, exception))

/*******************************************************************************
 * nfrf Interface
 ******************************************************************************/

struct netscape_jmc_JMCInterfaceID;
struct java_lang_Object;
struct java_lang_String;
struct netscape_fonts_nffmi;
struct netscape_fonts_nfrc;
struct netscape_fonts_BoundingBoxStar;

struct nfrfInterface {
	void*	(*getInterface)(struct nfrf* self, jint op, const JMCInterfaceID* a, JMCException* *exception);
	void	(*addRef)(struct nfrf* self, jint op, JMCException* *exception);
	void	(*release)(struct nfrf* self, jint op, JMCException* *exception);
	jint	(*hashCode)(struct nfrf* self, jint op, JMCException* *exception);
	jbool	(*equals)(struct nfrf* self, jint op, void* a, JMCException* *exception);
	void*	(*clone)(struct nfrf* self, jint op, JMCException* *exception);
	const char*	(*toString)(struct nfrf* self, jint op, JMCException* *exception);
	void	(*finalize)(struct nfrf* self, jint op, JMCException* *exception);
	struct nffmi*	(*GetMatchInfo)(struct nfrf* self, jint op, JMCException* *exception);
	jdouble	(*GetPointSize)(struct nfrf* self, jint op, JMCException* *exception);
	jint	(*GetMaxWidth)(struct nfrf* self, jint op, JMCException* *exception);
	jint	(*GetFontAscent)(struct nfrf* self, jint op, JMCException* *exception);
	jint	(*GetFontDescent)(struct nfrf* self, jint op, JMCException* *exception);
	jint	(*GetMaxLeftBearing)(struct nfrf* self, jint op, JMCException* *exception);
	jint	(*GetMaxRightBearing)(struct nfrf* self, jint op, JMCException* *exception);
	void	(*SetTransformMatrix)(struct nfrf* self, jint op, void** a, jsize a_length, JMCException* *exception);
	void*	(*GetTransformMatrix)(struct nfrf* self, jint op, JMCException* *exception);
	jint	(*MeasureText)(struct nfrf* self, jint op, struct nfrc* a, jint b, jbyte* c, jsize c_length, jint* d, jsize d_length, JMCException* *exception);
	jint	(*MeasureBoundingBox)(struct nfrf* self, jint op, struct nfrc* a, jint b, jbyte* c, jsize c_length, struct nf_bounding_box * d, JMCException* *exception);
	jint	(*DrawText)(struct nfrf* self, jint op, struct nfrc* a, jint b, jint c, jint d, jbyte* e, jsize e_length, JMCException* *exception);
	jint	(*PrepareDraw)(struct nfrf* self, jint op, struct nfrc* a, JMCException* *exception);
	jint	(*EndDraw)(struct nfrf* self, jint op, struct nfrc* a, JMCException* *exception);
};

/*******************************************************************************
 * nfrf Operation IDs
 ******************************************************************************/

typedef enum nfrfOperations {
	nfrf_getInterface_op,
	nfrf_addRef_op,
	nfrf_release_op,
	nfrf_hashCode_op,
	nfrf_equals_op,
	nfrf_clone_op,
	nfrf_toString_op,
	nfrf_finalize_op,
	nfrf_GetMatchInfo_op,
	nfrf_GetPointSize_op,
	nfrf_GetMaxWidth_op,
	nfrf_GetFontAscent_op,
	nfrf_GetFontDescent_op,
	nfrf_GetMaxLeftBearing_op,
	nfrf_GetMaxRightBearing_op,
	nfrf_SetTransformMatrix_op,
	nfrf_GetTransformMatrix_op,
	nfrf_MeasureText_op,
	nfrf_MeasureBoundingBox_op,
	nfrf_DrawText_op,
	nfrf_PrepareDraw_op,
	nfrf_EndDraw_op
} nfrfOperations;

/******************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* _Mnfrf_H_ */
