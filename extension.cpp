/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod Sample Extension
 * Copyright (C) 2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#include "extension.h"

/**
 * @file extension.cpp
 * @brief Implement extension code here.
 */

enum NumberType
{
    NumberType_Int8,
    NumberType_Int16,
    NumberType_Int32,
    NumberType_Int64,
};

Sample g_Sample;		/**< Global singleton for extension's main interface */

SMEXT_LINK(&g_Sample);

extern sp_nativeinfo_t g_natives[];

void Sample::SDK_OnAllLoaded()
{
    sharesys->RegisterLibrary(myself, "port64");
    sharesys->AddNatives(myself, g_natives);
}

static cell_t Port64_FromPseudoAddress(IPluginContext* pContext, const cell_t* params)
{
    uintptr_t* buffer;
    pContext->LocalToPhysAddr(params[2], reinterpret_cast<cell_t**>(&buffer));
#if defined(__i386__)
    *buffer = params[1];
#else
    *buffer = (uintptr_t)smutils->FromPseudoAddress(params[1]);
#endif
    return 0;
}

static cell_t Port64_ToPseudoAddress(IPluginContext* pContext, const cell_t* params)
{
    uintptr_t* buffer;
    pContext->LocalToPhysAddr(params[1], reinterpret_cast<cell_t**>(&buffer));
#if defined(__i386__)
    return *buffer;
#else
    return (cell_t)smutils->ToPseudoAddress((void*)*buffer);
#endif
}

static cell_t Port64_Add(IPluginContext* pContext, const cell_t* params)
{
    uintptr_t *a,*b,*c;
    pContext->LocalToPhysAddr(params[1], reinterpret_cast<cell_t**>(&a));
    pContext->LocalToPhysAddr(params[2], reinterpret_cast<cell_t**>(&b));
    pContext->LocalToPhysAddr(params[3], reinterpret_cast<cell_t**>(&c));
    *c = *a + *b;
    return 0;
}

static cell_t Port64_Sub(IPluginContext* pContext, const cell_t* params)
{
    uintptr_t *a,*b,*c;
    pContext->LocalToPhysAddr(params[1], reinterpret_cast<cell_t**>(&a));
    pContext->LocalToPhysAddr(params[2], reinterpret_cast<cell_t**>(&b));
    pContext->LocalToPhysAddr(params[3], reinterpret_cast<cell_t**>(&c));
    *c = *a - *b;
    return 0;
}

static cell_t Port64_Mul(IPluginContext* pContext, const cell_t* params)
{
    uintptr_t *a,*b,*c;
    pContext->LocalToPhysAddr(params[1], reinterpret_cast<cell_t**>(&a));
    pContext->LocalToPhysAddr(params[2], reinterpret_cast<cell_t**>(&b));
    pContext->LocalToPhysAddr(params[3], reinterpret_cast<cell_t**>(&c));
    *c = *a * *b;
    return 0;
}

static cell_t Port64_Div(IPluginContext* pContext, const cell_t* params)
{
    uintptr_t *a,*b,*c;
    pContext->LocalToPhysAddr(params[1], reinterpret_cast<cell_t**>(&a));
    pContext->LocalToPhysAddr(params[2], reinterpret_cast<cell_t**>(&b));
    pContext->LocalToPhysAddr(params[3], reinterpret_cast<cell_t**>(&c));
    *c = *a / *b;
    return 0;
}

// address, offset, type, output
static cell_t Port64_LoadFromAddress(IPluginContext* pContext, const cell_t* params)
{
    uintptr_t* addr;
    uintptr_t* output;

    pContext->LocalToPhysAddr(params[4], reinterpret_cast<cell_t**>(&output));    
    pContext->LocalToPhysAddr(params[1], reinterpret_cast<cell_t**>(&addr));
    addr = (uintptr_t*)(*addr + params[2]);

    switch(params[3])
    {
        case NumberType_Int8:
            *output = *(int8_t*)addr;
            break;
        case NumberType_Int16:
            *output = *(int16_t*)addr;
            break;
        case NumberType_Int32:
            *output = *(int32_t*)addr;
            break;
        case NumberType_Int64:
            *output = *(int64_t*)addr;
            break;
        default:
            pContext->ReportError("invalid NumberType %i", params[3]);
            return 1;
    }

    return 0;
}

// address, offset, type, input
static cell_t Port64_StoreToAddress(IPluginContext* pContext, const cell_t* params)
{
    uintptr_t* addr;
    uintptr_t* input;

    pContext->LocalToPhysAddr(params[4], reinterpret_cast<cell_t**>(&input));    
    pContext->LocalToPhysAddr(params[1], reinterpret_cast<cell_t**>(&addr));
    addr = (uintptr_t*)(*addr + params[2]);

    switch(params[3])
    {
        case NumberType_Int8:
            *(int8_t*)addr = *(int8_t*)input;
            break;
        case NumberType_Int16:
            *(int16_t*)addr = *(int16_t*)input;
            break;
        case NumberType_Int32:
            *(int32_t*)addr = *(int32_t*)input;
            break;
        case NumberType_Int64:
            *(int64_t*)addr = *(int64_t*)input;
            break;
        default:
            pContext->ReportError("invalid NumberType %i", params[3]);
            return 1;
    }

    return 0;
}

static cell_t Port64_PointerBytes(IPluginContext* pContext, const cell_t* params)
{
    return sizeof(void*);
}

static cell_t Port64_GetEntityAddress(IPluginContext* pContext, const cell_t* params)
{
    uintptr_t* addr;
    pContext->LocalToPhysAddr(params[2], reinterpret_cast<cell_t**>(&addr));

    CBaseEntity* pEntity = gamehelpers->ReferenceToEntity(params[1]);
    if(!pEntity)
    {
        *addr = 0;
        return 0;
    }

    *addr = (uintptr_t)pEntity;
    return 0;
}

sp_nativeinfo_t g_natives[] = {
    { "Port64_PointerBytes",        Port64_PointerBytes},
    { "Port64_FromPseudoAddress",   Port64_FromPseudoAddress },
    { "Port64_ToPseudoAddress",     Port64_ToPseudoAddress },
    { "Port64_Add",                 Port64_Add },
    { "Port64_Sub",                 Port64_Sub },
    { "Port64_Mul",                 Port64_Mul },
    { "Port64_Div",                 Port64_Div },
    { "Port64_LoadFromAddress",     Port64_LoadFromAddress },
    { "Port64_StoreToAddress",      Port64_StoreToAddress },
    { "Port64_GetEntityAddress",    Port64_GetEntityAddress },
    { nullptr,                      nullptr }
};