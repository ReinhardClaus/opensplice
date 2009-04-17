/*
 *                         OpenSplice DDS
 *
 *   This software and documentation are Copyright 2006 to 2009 PrismTech 
 *   Limited and its licensees. All rights reserved. See file:
 *
 *                     $OSPL_HOME/LICENSE 
 *
 *   for full copyright notice and license terms. 
 *
 */
/*
 * in_ddsiSubmessageToken.h
 *
 *  Created on: Feb 24, 2009
 *      Author: frehberg
 */

#ifndef IN_DDSISUBMESSAGETOKEN_H_
#define IN_DDSISUBMESSAGETOKEN_H_


#if defined (__cplusplus)
extern "C" {
#endif

typedef in_octet* in_ddsiSubmessageToken;

/** Convert token to pointer */
#define in_ddsiSubmessageTokenGetPtr(_token) (_token)

#if defined (__cplusplus)
}
#endif


#endif /* IN_DDSISUBMESSAGETOKEN_H_ */
