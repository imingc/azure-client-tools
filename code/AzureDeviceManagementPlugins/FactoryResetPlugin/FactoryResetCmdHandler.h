// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

namespace Microsoft { namespace Azure { namespace DeviceManagement { namespace FactoryResetPlugin {

    class FactoryResetCmdHandler : public DMCommon::BaseHandler
    {
    public:
        FactoryResetCmdHandler();

        // IRawHandler
        void Start(
            const Json::Value& config,
            bool& active);

        void OnConnectionStatusChanged(
            DMCommon::ConnectionStatus status);

        DMCommon::InvokeResult Invoke(
            const Json::Value& desiredConfig) noexcept;
    };

}}}}
