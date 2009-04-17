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
/* Abstraction layer includes */
#include "os_heap.h"

/* collection includes */
#include "Coll_List.h"
#include "Coll_Compare.h"

/* DDSi includes */
#include "in_connectivityReaderFacade.h"
#include "in_connectivityEntityFacade.h"


static os_boolean
in_connectivityReaderFacadeInit(
    in_connectivityReaderFacade _this,
    struct v_subscriptionInfo *info);

static void
in_connectivityReaderFacadeDeinit(
    in_object obj);

OS_STRUCT(in_connectivityReaderFacade)
{
    OS_EXTENDS(in_connectivityEntityFacade);
    /* A set containing all matched peers, in_connectivityPeerWriter objects */
    struct v_subscriptionInfo info;
    Coll_Set matchedWriters;
/*    in_connectivityReaderIdentifier id;*/
};


in_connectivityReaderFacade
in_connectivityReaderFacadeNew(
    struct v_subscriptionInfo *info)
{
    os_boolean success;
    in_connectivityReaderFacade _this;

    _this = in_connectivityReaderFacade(os_malloc(sizeof(OS_STRUCT(in_connectivityReaderFacade))));

    if(_this)
    {
        success = in_connectivityReaderFacadeInit(_this,info);

        if(!success)
        {
            os_free(_this);
            _this = NULL;
        }
    }

    return _this;
}

os_boolean
in_connectivityReaderFacadeInit(
    in_connectivityReaderFacade _this,
    struct v_subscriptionInfo *info)
{
    os_boolean success;
    assert(_this);

    success = in_connectivityEntityFacadeInit(
        OS_SUPER(_this),
        IN_OBJECT_KIND_READER_FACADE,
        in_connectivityReaderFacadeDeinit);

    if(success)
    {
        _this->info = *info;
        _this->info.type_name = os_strdup(info->type_name);
        _this->info.topic_name = os_strdup(info->topic_name);
        Coll_Set_init(&_this->matchedWriters, pointerIsLessThen, TRUE);
    }
    return success;
}

static void
in_connectivityReaderFacadeDeinit(
    in_object obj)
{
    in_connectivityPeerWriter writer;
    Coll_Iter* iterator;
    in_connectivityReaderFacade _this;

    assert(in_connectivityReaderFacadeIsValid(obj));
    _this = in_connectivityReaderFacade(obj);

    iterator = Coll_Set_getFirstElement(&_this->matchedWriters);
    while(iterator)
    {
        writer = in_connectivityPeerWriter(Coll_Iter_getObject(iterator));
        in_connectivityPeerWriterFree(writer);
        iterator = Coll_Iter_getNext(iterator);
        Coll_Set_remove(&_this->matchedWriters, writer);
        os_free(_this->info.type_name);
        os_free(_this->info.topic_name);
    }
    in_connectivityEntityFacadeDeinit(obj);
}

in_result
in_connectivityReaderFacadeAddMatchedPeer(
    in_connectivityReaderFacade _this,
    in_connectivityPeerWriter peer)
{
    in_result result;
    os_uint32 collResult;

    assert(in_connectivityReaderFacadeIsValid(_this));
    assert(in_connectivityPeerWriterIsValid(peer));

    collResult = Coll_Set_add(&_this->matchedWriters, in_connectivityPeerWriterKeep(peer));
    IN_COLLRESULT_TO_RESULT(collResult, result);

    if(result != IN_RESULT_OK)
    {
        /* undo the keep! */
        in_connectivityPeerWriterFree(peer);
    }
    return result;
}

in_result
in_connectivityReaderFacadeRemoveMatchedPeer(
    in_connectivityReaderFacade _this,
    in_connectivityPeerWriter peer)
{
    in_result result = IN_RESULT_OK;
    in_connectivityPeerWriter removedWriter;

    assert(in_connectivityReaderFacadeIsValid(_this));
    assert(in_connectivityPeerWriterIsValid(peer));

    removedWriter = Coll_Set_remove(&_this->matchedWriters, peer);

    if(!removedWriter)
    {
        result = IN_RESULT_NOT_FOUND;
    } else
    {
        /* undo the keep of the collection! */
        in_connectivityPeerWriterFree(peer);
    }

    return result;
}

struct v_subscriptionInfo *
in_connectivityReaderFacadeGetInfo(
    in_connectivityReaderFacade _this)
{
    assert(in_connectivityReaderFacadeIsValid(_this));

    return &_this->info;
}



Coll_Set*
in_connectivityReaderFacadeGetMatchedWriters(
    in_connectivityReaderFacade _this)
{
    assert(in_connectivityReaderFacadeIsValid(_this));

    return &_this->matchedWriters;
}

