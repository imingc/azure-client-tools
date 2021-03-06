// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "stdafx.h"
#include "PluginJsonConstants.h"
#include "..\..\AzureDeviceManagementCommon\DMConstants.h"
#include "..\..\AzureDeviceManagementCommon\Plugins\PluginConstants.h"
#include "DeviceInfoHandler.h"

#define CSPDeviceId "./DevInfo/DevId"
#define CSPDeviceManufacturer "./DevInfo/Man"

#define CSPDeviceModel "./DevInfo/Mod"
#define CSPDeviceDmV "./DevInfo/DmV"
#define CSPDeviceLang "./DevInfo/Lang"
#define CSPDeviceType "./DevDetail/DevTyp"

#define CSPDeviceOEM "./DevDetail/OEM"
#define CSPDeviceHwV "./DevDetail/HwV"
#define CSPDeviceFwV "./DevDetail/FwV"
#define CSPDeviceSwV "./DevDetail/SwV"

#define CSPDeviceOSPlatform "./DevDetail/Ext/Microsoft/OSPlatform"
#define CSPDeviceProcessorType "./DevDetail/Ext/Microsoft/ProcessorType"
#define CSPDeviceRadioSwV "./DevDetail/Ext/Microsoft/RadioSwV"
#define CSPDeviceResolution "./DevDetail/Ext/Microsoft/Resolution"

#define CSPDeviceProcessorArchitecture "./DevDetail/Ext/Microsoft/ProcessorArchitecture"
#define CSPDeviceName "./DevDetail/Ext/Microsoft/DeviceName"

#define CSPDeviceOSEdition "./Vendor/MSFT/DeviceStatus/OS/Edition"

#define CSPDeviceTotalRAM "./DevDetail/Ext/Microsoft/TotalRAM"
#define CSPSecureBootState "./Vendor/MSFT/DeviceStatus/SecureBootState"
#define CSPBatteryStatus "./Vendor/MSFT/DeviceStatus/Battery/Status"
#define CSPBatteryRemaining "./Vendor/MSFT/DeviceStatus/Battery/EstimatedChargeRemaining"
#define CSPBatteryRuntime "./Vendor/MSFT/DeviceStatus/Battery/EstimatedRuntime"

using namespace DMCommon;
using namespace DMUtils;
using namespace std;

namespace Microsoft { namespace Azure { namespace DeviceManagement { namespace DeviceInfoPlugin {

    DeviceInfoHandler::DeviceInfoHandler() :
        BaseHandler(DeviceInfoHandlerId, ReportedSchema(JsonDeviceSchemasTypeRaw, JsonDeviceSchemasTagDM, 1, 1))
    {
    }

    void DeviceInfoHandler::Start(
        const Json::Value& handlerConfig,
        bool& active)
    {
        TRACELINE(LoggingLevel::Verbose, __FUNCTION__);

        SetConfig(handlerConfig);

        // Text file logging...
        Json::Value logFilesPath = handlerConfig[JsonTextLogFilesPath];
        if (!logFilesPath.isNull() && logFilesPath.isString())
        {
            wstring wideLogFileName = MultibyteToWide(logFilesPath.asString().c_str());
            wstring wideLogFileNamePrefix = MultibyteToWide(DeviceInfoHandlerId);
            gLogger.SetLogFilePath(wideLogFileName.c_str(), wideLogFileNamePrefix.c_str());
            gLogger.EnableConsole(true);

            TRACELINE(LoggingLevel::Verbose, "Logging configured.");
        }

        active = true;
    }

    void DeviceInfoHandler::OnConnectionStatusChanged(
        DMCommon::ConnectionStatus status)
    {
        TRACELINE(LoggingLevel::Verbose, __FUNCTION__);
        if (status == ConnectionStatus::eOffline)
        {
            TRACELINE(LoggingLevel::Verbose, "Connection Status: Offline.");
        }
        else
        {
            TRACELINE(LoggingLevel::Verbose, "Connection Status: Online.");

            Json::Value handlerConfig = GetConfig();

            // Report on connect...
            Json::Value reportOnConnect = handlerConfig[JsonHandlerConfigReportOnConnect];
            if (!reportOnConnect.isNull() && reportOnConnect.isBool())
            {
                if (reportOnConnect.asBool())
                {
                    TRACELINE(LoggingLevel::Verbose, "Report on connect is on...");

                    Json::Value reportedObject(Json::objectValue);
                    std::shared_ptr<ReportedErrorList> errorList = make_shared<ReportedErrorList>();
                    BuildReported(reportedObject, errorList);
                    FinalizeAndReport(reportedObject, errorList);
                }
                else
                {
                    TRACELINE(LoggingLevel::Verbose, "Report on connect is off...");
                }
            }
        }
    }

    void DeviceInfoHandler::ReadStringProperty(
        const std::string& cspNodePath,
        const std::string& operationId,
        Json::Value& reportedObject,
        std::shared_ptr<DMCommon::ReportedErrorList> errorList)
    {
        wstring wideCspNodePath = Utils::MultibyteToWide(cspNodePath.c_str());

        RunOperation(operationId, errorList,
            [&]()
        {
            // Read back
            string readValue = _mdmProxy.RunGetString(cspNodePath);
            reportedObject[operationId] = Json::Value(readValue);
        });
    }

    void DeviceInfoHandler::BuildReported(
        Json::Value& reportedObject,
        std::shared_ptr<DMCommon::ReportedErrorList> errorList)
    {
        ReadStringProperty(CSPDeviceId, JsonDeviceId, reportedObject, errorList);
        ReadStringProperty(CSPDeviceManufacturer, JsonDeviceManufacturer, reportedObject, errorList);

        ReadStringProperty(CSPDeviceModel, JsonDeviceModel, reportedObject, errorList);
        ReadStringProperty(CSPDeviceDmV, JsonDeviceDmV, reportedObject, errorList);
        ReadStringProperty(CSPDeviceLang, JsonDeviceLang, reportedObject, errorList);
        ReadStringProperty(CSPDeviceType, JsonDeviceType, reportedObject, errorList);

        ReadStringProperty(CSPDeviceOEM, JsonDeviceOEM, reportedObject, errorList);
        ReadStringProperty(CSPDeviceHwV, JsonDeviceHwV, reportedObject, errorList);
        ReadStringProperty(CSPDeviceFwV, JsonDeviceFwV, reportedObject, errorList);
        ReadStringProperty(CSPDeviceSwV, JsonDeviceSwV, reportedObject, errorList);

        ReadStringProperty(CSPDeviceOSPlatform, JsonDeviceOSPlatform, reportedObject, errorList);
        ReadStringProperty(CSPDeviceProcessorType, JsonDeviceProcessorType, reportedObject, errorList);
        ReadStringProperty(CSPDeviceRadioSwV, JsonDeviceRadioSwV, reportedObject, errorList);
        ReadStringProperty(CSPDeviceResolution, JsonDeviceResolution, reportedObject, errorList);

        ReadStringProperty(CSPDeviceProcessorArchitecture, JsonDeviceProcessorArchitecture, reportedObject, errorList);
        ReadStringProperty(CSPDeviceName, JsonDeviceName, reportedObject, errorList);
        ReadStringProperty(CSPDeviceOSEdition, JsonDeviceOSEdition, reportedObject, errorList);

        ReadNumberProperty<unsigned int>(CSPDeviceTotalRAM, JsonDeviceTotalRAM, reportedObject, errorList);
        ReadNumberProperty<unsigned int>(CSPSecureBootState, JsonSecureBootState, reportedObject, errorList);
        ReadNumberProperty<unsigned int>(CSPBatteryStatus, JsonBatteryStatus, reportedObject, errorList);
        ReadNumberProperty<char>(CSPBatteryRemaining, JsonBatteryRemaining, reportedObject, errorList);
        ReadNumberProperty<int>(CSPBatteryRuntime, JsonBatteryRuntime, reportedObject, errorList);
    }

    void DeviceInfoHandler::EmptyReported(
        Json::Value& reportedObject)
    {
        Json::Value nullValue;
        reportedObject[JsonDeviceId] = nullValue;
        reportedObject[JsonDeviceManufacturer] = nullValue;

        reportedObject[JsonDeviceModel] = nullValue;
        reportedObject[JsonDeviceDmV] = nullValue;
        reportedObject[JsonDeviceLang] = nullValue;
        reportedObject[JsonDeviceType] = nullValue;

        reportedObject[JsonDeviceOEM] = nullValue;
        reportedObject[JsonDeviceHwV] = nullValue;
        reportedObject[JsonDeviceFwV] = nullValue;
        reportedObject[JsonDeviceSwV] = nullValue;

        reportedObject[JsonDeviceOSPlatform] = nullValue;
        reportedObject[JsonDeviceProcessorType] = nullValue;
        reportedObject[JsonDeviceRadioSwV] = nullValue;
        reportedObject[JsonDeviceResolution] = nullValue;

        reportedObject[JsonDeviceProcessorArchitecture] = nullValue;
        reportedObject[JsonDeviceName] = nullValue;
        reportedObject[JsonDeviceOSEdition] = nullValue;

        reportedObject[JsonDeviceTotalRAM] = nullValue;
        reportedObject[JsonSecureBootState] = nullValue;
        reportedObject[JsonBatteryStatus] = nullValue;
        reportedObject[JsonBatteryRemaining] = nullValue;
        reportedObject[JsonBatteryRuntime] = nullValue;
    }

    InvokeResult DeviceInfoHandler::Invoke(
        const Json::Value& groupDesiredConfigJson) noexcept
    {
        TRACELINE(LoggingLevel::Verbose, __FUNCTION__);

        // Returned objects (if InvokeContext::eDirectMethod, it is returned to the cloud direct method caller).
        InvokeResult invokeResult(InvokeContext::eDesiredState);

        // Twin reported objects
        Json::Value reportedObject(Json::objectValue);
        std::shared_ptr<ReportedErrorList> errorList = make_shared<ReportedErrorList>();

        RunOperation(GetId(), errorList,
            [&]()
        {
            // Make sure this is not a transient state
            if (IsRefreshing(groupDesiredConfigJson))
            {
                return;
            }

            // Process Meta Data
            _metaData->FromJsonParentObject(groupDesiredConfigJson);

            // Apply new state
            // -> None defined.

            // Is configured?
            _isConfigured = true;

            // Report current state
            if (_metaData->GetReportingMode() == JsonReportingModeAlways)
            {
                BuildReported(reportedObject, errorList);
            }
            else
            {
                EmptyReported(reportedObject);
            }
        });

        FinalizeAndReport(reportedObject, errorList);

        return invokeResult;
    }

}}}}
