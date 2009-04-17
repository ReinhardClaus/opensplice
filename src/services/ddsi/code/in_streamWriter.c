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
 * in_streamWriter.c
 *
 *  Created on: Mar 3, 2009
 *      Author: frehberg
 */

/* **** interface headers **** */
#include "in__streamWriter.h"

/* **** implementation headers **** */

/* **** private functions **** */

/* **** public functions **** */


/** */
os_boolean
in_streamWriterInit(
        in_streamWriter _this,
        in_objectKind kind,
        in_objectDeinitFunc deinit,
        in_streamWriterPublicVTable staticPublicVTable)
{
	in_objectInit(OS_SUPER(_this),
			kind,
			deinit);
	/* reference the static vtable */
	_this->publicVTable  = staticPublicVTable;

	return OS_TRUE;
}


/** */
void
in_streamWriterDeinit(
		in_streamWriter _this)
{
	in_objectDeinit(OS_SUPER(_this));
}

/** */
void
in_streamWriterFree(
		in_streamWriter _this)
{
	in_objectFree(OS_SUPER(_this));
}


/** must be invoked if destination (locatorList changes,
 * in this sense the object is stateless */
void
in_streamWriterFlush(
		in_streamWriter _this,
		Coll_List* locatorList)
{
	if (_this->publicVTable->flush) {
		_this->publicVTable->flush(_this, locatorList);
	}
}

void
in_streamWriterFlushSingle(
        in_streamWriter _this,
        in_locator locator)
{
    if (_this->publicVTable->flushSingle) {
        _this->publicVTable->flushSingle(_this, locator);
    }
}
/** */
in_result
in_streamWriterAppendData(
		in_streamWriter _this,
		v_message message,
		in_connectivityWriterFacade facade,
		os_boolean recipientExpectsInlineQos,
		Coll_List *locatorList)
{
	in_result result = IN_RESULT_ERROR;

	if (_this->publicVTable->appendData) {
		result =
			_this->publicVTable->appendData(_this,
				message,
				facade,
				recipientExpectsInlineQos,
				locatorList);
	}
	return result;
}

/** */
in_result
in_streamWriterAppendParticipantData(
		in_streamWriter _this,
		in_connectivityParticipantFacade facade,
		Coll_List *locatorList)
{
	in_result result = IN_RESULT_ERROR;

	if (_this->publicVTable->appendParticipantData) {
		result =
			_this->publicVTable->appendParticipantData(_this,
				facade,
				locatorList);
	}
	return result;
}

/** */
in_result
in_streamWriterAppendReaderData(
		in_streamWriter _this,
		in_connectivityReaderFacade facade,
		Coll_List *locatorList)
{
	in_result result = IN_RESULT_ERROR;

	if (_this->publicVTable->appendReaderData) {
		result =
			_this->publicVTable->appendReaderData(_this,
				facade,
				locatorList);
	}
	return result;
}

/** */
in_result
in_streamWriterAppendWriterData(
		in_streamWriter _this,
		in_connectivityWriterFacade facade,
		Coll_List *locatorList)
{
	in_result result = IN_RESULT_ERROR;

	if (_this->publicVTable->appendWriterData) {
		result =
			_this->publicVTable->appendWriterData(_this,
				facade,
				locatorList);
	}
	return result;
}

/** ingore "Final" flag for now
 *
 * If sending the first AckNack to writer, the streamWriter
 * must be flushed first to deliver previous data items to multiple
 * recipients. When all AckNacks have been appended for that destination, invoke
 * in_streamReaderFlush1(in_locator locator) */
in_result
in_streamWriterAppendAckNack(
        in_streamWriter _this,
        in_ddsiGuid readerGuid,
        in_ddsiGuid writerGuid,
        /* single sequence number, but will be transformed
         * to sequencenumber-set on the wire */
        in_ddsiSequenceNumber writerSN,
        in_locator destination)
{
    in_result result = IN_RESULT_ERROR;

      if (_this->publicVTable->appendAckNack) {
          result =
              _this->publicVTable->appendAckNack(_this,
                      readerGuid,
                      writerGuid,
                      writerSN,
                      destination);
      }
      return result;
}

/** \return corresponding default multicast locator for data (also SDP traffic) */
in_locator
in_streamWriterGetDataMulticastLocator(in_streamWriter _this)
{
    in_locator result = NULL;
    if (_this->publicVTable->getDataMulticastLocator) {
        result = _this->publicVTable->getDataMulticastLocator(_this);
    }
    return result;
}

/** \return corresponding default uniicast locator for data (also SDP traffic) */
in_locator
in_streamWriterGetDataUnicastLocator(in_streamWriter _this)
{
    in_locator result = NULL;
    if (_this->publicVTable->getDataUnicastLocator) {
        result = _this->publicVTable->getDataUnicastLocator(_this);
    }
    return result;

}


/** \return corresponding default unicast locator for control messages */
in_locator
in_streamWriterGetCtrlUnicastLocator(in_streamWriter _this)
{
    in_locator result = NULL;
    if (_this->publicVTable->getCtrlUnicastLocator) {
        result = _this->publicVTable->getCtrlUnicastLocator(_this);
    }
    return result;

}


