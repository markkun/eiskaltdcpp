/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STATICLIB
#define STATICLIB
#endif
#include <miniupnpc/miniupnpc.h>
#include <miniupnpc/upnpcommands.h>
#include <miniupnpc/upnperrors.h>

#if (MINIUPNPC_API_VERSION >= 8)

#include "upnpc6.h"
#include <dcpp/Util.h>
#include <dcpp/SettingsManager.h>

static UPNPUrls urls6;
static IGDdatas data6;
const std::string UPnPc6::name = "MiniUPnP6";

using namespace std;
using namespace dcpp;

bool UPnPc6::init()
{
    string mcastif = SETTING(BIND_IFACE6)? Util::getIfaceI4(SETTING(BIND_IFACE_NAME6)) : SETTING(BIND_ADDRESS6);
    UPNPDev *devices = upnpDiscover(5000, mcastif.c_str(), 0, 0, 1, 0);

    if (!devices)
        return false;

    bool ret = UPNP_GetValidIGD(devices, &urls6, &data6, 0, 0);

    freeUPNPDevlist(devices);

    return ret;
}

bool UPnPc6::add(const unsigned short port, const UPnP::Protocol protocol, const string& description)
{
    const string port_ = Util::toString(port);

    return UPNP_AddPortMapping(urls6.controlURL, data6.first.servicetype, port_.c_str(), port_.c_str(),
        (SETTING(BIND_IFACE6)? Util::getIfaceI6(SETTING(BIND_IFACE_NAME6)).c_str() : SETTING(BIND_ADDRESS6).c_str())
        , description.c_str(), protocols[protocol], NULL, 0) == UPNPCOMMAND_SUCCESS;
}

bool UPnPc6::remove(const unsigned short port, const UPnP::Protocol protocol)
{
    return UPNP_DeletePortMapping(urls6.controlURL, data6.first.servicetype, Util::toString(port).c_str(),
        protocols[protocol], NULL) == UPNPCOMMAND_SUCCESS;
}

string UPnPc6::getExternalIP()
{
    char buf[16] = { 0 };
    if (UPNP_GetExternalIPAddress(urls6.controlURL, data6.first.servicetype, buf) == UPNPCOMMAND_SUCCESS)
        return buf;
    return Util::emptyString;
}

#endif
