/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
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

#ifndef __XP_SEC_h_
#define __XP_SEC_h_

#include "xp_core.h"

/*
** Security related XP functions. Useful stuff that makes app writing
** easier.
*/

XP_BEGIN_PROTOS

struct CERTCertificateStr;

/*
** Return a pretty string that describes the status of the security
** connection using the argument information.
** 	"level" the level of security (HIGH, LOW, or OFF)
** 	"cipher" the name of the cipher
** 	"keySize" the total size of the cipher key
** 	"secretKeySize" the portion of the cipher key that is secret
** The above values can be easily determined by calling
** SSL_SecurityStatus().
*/
extern char *XP_PrettySecurityStatus(int level, char *cipher, int keySize,
				     int secretKeySize);

/*
** Return a dynamically allocated string which describes what security
** version the security library supports. The returned string describes
** in totality the crypto capabilities of the library.
*/
extern char *XP_SecurityCapabilities(void);

/*
** Return a short staticly allocated string (NOT MALLOC'D) which
** describes what security version the security library supports:
** U.S. or international.
** 	"longForm" is non-zero means the return value should be
**	  in the long format, otherwise in the short format.
*/
extern char *XP_SecurityVersion(int longForm);

XP_END_PROTOS

#endif /* __XP_SEC_h_ */
