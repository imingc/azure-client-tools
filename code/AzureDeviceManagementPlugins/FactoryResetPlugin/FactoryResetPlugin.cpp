// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "stdafx.h"
#include "..\..\AzureDeviceManagementCommon\Plugins\PluginConstants.h"
#include "..\..\AzureDeviceManagementCommon\Plugins\PluginJsonConstants.h"
#include "..\AzureDeviceManagementPluginCommon\PluginStub.h"
#include "PluginJsonConstants.h"
#include "FactoryResetCmdHandler.h"

using namespace std;
using namespace DMUtils;
using namespace DMCommon;

using namespace Microsoft::Azure::DeviceManagement::Plugin::Common;
using namespace Microsoft::Azure::DeviceManagement::FactoryResetPlugin;

PluginStub gPluginStub;
std::shared_ptr<IRawHandler> gFactoryResetCmdHandler;

std::shared_ptr<IRawHandler> CreateFactoryResetCmdHandler()
{
    if (gFactoryResetCmdHandler == nullptr)
    {
        gFactoryResetCmdHandler = make_shared<FactoryResetCmdHandler>();
    }
    return gFactoryResetCmdHandler;
}

int __stdcall PluginCreate()
{
    gPluginStub.Initialize();
    gPluginStub.RegisterRawHandler(FactoryResetCmdHandlerId, CreateFactoryResetCmdHandler);
    return PLUGIN_ERR_SUCCESS;
}

#include "..\AzureDeviceManagementPluginCommon\PluginExports.h"
