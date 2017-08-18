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
 * Source date: 26 Feb 1998 01:03:40 GMT
 * netscape/fonts/winfp module C stub file
 * Generated by jmc version 1.8 -- DO NOT EDIT
 ******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "xp_mem.h"

/* Include the implementation-specific header: */
#include "Pwinfp.h"

/* Include other interface headers: */

/*******************************************************************************
 * winfp Methods
 ******************************************************************************/

#ifndef OVERRIDE_winfp_getInterface
JMC_PUBLIC_API(void*)
_winfp_getInterface(struct winfp* self, jint op, const JMCInterfaceID* iid, JMCException* *exc)
{
	if (memcmp(iid, &winfp_ID, sizeof(JMCInterfaceID)) == 0)
		return winfpImpl2winfp(winfp2winfpImpl(self));
	return _winfp_getBackwardCompatibleInterface(self, iid, exc);
}
#endif

#ifndef OVERRIDE_winfp_addRef
JMC_PUBLIC_API(void)
_winfp_addRef(struct winfp* self, jint op, JMCException* *exc)
{
	winfpImplHeader* impl = (winfpImplHeader*)winfp2winfpImpl(self);
	impl->refcount++;
}
#endif

#ifndef OVERRIDE_winfp_release
JMC_PUBLIC_API(void)
_winfp_release(struct winfp* self, jint op, JMCException* *exc)
{
	winfpImplHeader* impl = (winfpImplHeader*)winfp2winfpImpl(self);
	if (--impl->refcount == 0) {
		winfp_finalize(self, exc);
	}
}
#endif

#ifndef OVERRIDE_winfp_hashCode
JMC_PUBLIC_API(jint)
_winfp_hashCode(struct winfp* self, jint op, JMCException* *exc)
{
	return (jint)self;
}
#endif

#ifndef OVERRIDE_winfp_equals
JMC_PUBLIC_API(jbool)
_winfp_equals(struct winfp* self, jint op, void* obj, JMCException* *exc)
{
	return self == obj;
}
#endif

#ifndef OVERRIDE_winfp_clone
JMC_PUBLIC_API(void*)
_winfp_clone(struct winfp* self, jint op, JMCException* *exc)
{
	winfpImpl* impl = winfp2winfpImpl(self);
	winfpImpl* newImpl = (winfpImpl*)malloc(sizeof(winfpImpl));
	if (newImpl == NULL) return NULL;
	memcpy(newImpl, impl, sizeof(winfpImpl));
	((winfpImplHeader*)newImpl)->refcount = 1;
	return newImpl;
}
#endif

#ifndef OVERRIDE_winfp_toString
JMC_PUBLIC_API(const char*)
_winfp_toString(struct winfp* self, jint op, JMCException* *exc)
{
	return NULL;
}
#endif

#ifndef OVERRIDE_winfp_finalize
JMC_PUBLIC_API(void)
_winfp_finalize(struct winfp* self, jint op, JMCException* *exc)
{
	/* Override this method and add your own finalization here. */
	XP_FREEIF(self);
}
#endif

/*******************************************************************************
 * Jump Tables
 ******************************************************************************/

const struct winfpInterface winfpVtable = {
	_winfp_getInterface,
	_winfp_addRef,
	_winfp_release,
	_winfp_hashCode,
	_winfp_equals,
	_winfp_clone,
	_winfp_toString,
	_winfp_finalize,
	_winfp_LookupFont,
	_winfp_CreateFontFromFile,
	_winfp_CreateFontStreamHandler,
	_winfp_EnumerateSizes,
	_winfp_ReleaseFontHandle,
	_winfp_GetMatchInfo,
	_winfp_GetRenderableFont,
	_winfp_Name,
	_winfp_Description,
	_winfp_EnumerateMimeTypes,
	_winfp_ListFonts,
	_winfp_ListSizes,
	_winfp_CacheFontInfo
};

/*******************************************************************************
 * Factory Operations
 ******************************************************************************/

JMC_PUBLIC_API(winfp*)
winfpFactory_Create(JMCException* *exception, struct nffbp* broker)
{
	winfpImplHeader* impl = (winfpImplHeader*)XP_NEW_ZAP(winfpImpl);
	winfp* self;
	if (impl == NULL) {
		JMC_EXCEPTION(exception, JMCEXCEPTION_OUT_OF_MEMORY);
		return NULL;
	}
	self = winfpImpl2winfp(impl);
	impl->vtablewinfp = &winfpVtable;
	impl->refcount = 1;
	_winfp_init(self, exception, broker);
	if (JMC_EXCEPTION_RETURNED(exception)) {
		XP_FREE(impl);
		return NULL;
	}
	return self;
}

