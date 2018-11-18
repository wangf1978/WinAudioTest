#include "stdafx.h"
#include "MMDeviceUtility.h"
#include <devpkey.h>
#include <propkey.h>
#include <propvarutil.h>

FLAG_NAME_PAIR CMMDeviceUtility::Device_state_flag_names[] = {
	DECL_TUPLE_W(DEVICE_STATE_ACTIVE),
	DECL_TUPLE_W(DEVICE_STATE_DISABLED),
	DECL_TUPLE_W(DEVICE_STATE_NOTPRESENT),
	DECL_TUPLE_W(DEVICE_STATE_UNPLUGGED),
};

FLAG_NAME_PAIR CMMDeviceUtility::Data_flow_flag_names[] = {
	DECL_TUPLE_W(eRender),
	DECL_TUPLE_W(eCapture),
	DECL_TUPLE_W(eAll),
};

FLAG_NAME_PAIR CMMDeviceUtility::Role_flag_names[] = {
	DECL_TUPLE_W(eConsole),
	DECL_TUPLE_W(eMultimedia),
	DECL_TUPLE_W(eCommunications),
};

const std::tuple<const PROPERTYKEY, const WCHAR*> CMMDeviceUtility::prop_key_names[] =
{
	DECL_TUPLE_W(PKEY_AudioEndpoint_FormFactor),
	DECL_TUPLE_W(PKEY_AudioEndpoint_ControlPanelPageProvider),
	DECL_TUPLE_W(PKEY_AudioEndpoint_Association),
	DECL_TUPLE_W(PKEY_AudioEndpoint_PhysicalSpeakers),
	DECL_TUPLE_W(PKEY_AudioEndpoint_GUID),
	DECL_TUPLE_W(PKEY_AudioEndpoint_Disable_SysFx),
	DECL_TUPLE_W(PKEY_AudioEndpoint_FullRangeSpeakers),
	DECL_TUPLE_W(PKEY_AudioEndpoint_Supports_EventDriven_Mode),
	DECL_TUPLE_W(PKEY_AudioEndpoint_JackSubType),
	DECL_TUPLE_W(PKEY_AudioEndpoint_Default_VolumeInDb),
	DECL_TUPLE_W(PKEY_AudioEngine_DeviceFormat),
	DECL_TUPLE_W(PKEY_AudioEngine_OEMFormat),
	DECL_TUPLE_W(PKEY_AudioEndpointLogo_IconEffects),
	DECL_TUPLE_W(PKEY_AudioEndpointLogo_IconPath),
	DECL_TUPLE_W(PKEY_AudioEndpointSettings_MenuText),
	DECL_TUPLE_W(PKEY_AudioEndpointSettings_LaunchContract),
	DECL_TUPLE_EX_W(DEVPKEY_NAME),
	DECL_TUPLE_EX_W(DEVPKEY_Device_DeviceDesc),
	DECL_TUPLE_EX_W(DEVPKEY_Device_HardwareIds),
	DECL_TUPLE_EX_W(DEVPKEY_Device_CompatibleIds),
	DECL_TUPLE_EX_W(DEVPKEY_Device_Service),
	DECL_TUPLE_EX_W(DEVPKEY_Device_Class),
	DECL_TUPLE_EX_W(DEVPKEY_Device_ClassGuid),
	DECL_TUPLE_EX_W(DEVPKEY_Device_Driver),
	DECL_TUPLE_EX_W(DEVPKEY_Device_ConfigFlags),
	DECL_TUPLE_EX_W(DEVPKEY_Device_Manufacturer),
	DECL_TUPLE_EX_W(DEVPKEY_Device_FriendlyName),
	DECL_TUPLE_EX_W(DEVPKEY_Device_LocationInfo),
	DECL_TUPLE_EX_W(DEVPKEY_Device_PDOName),
	DECL_TUPLE_EX_W(DEVPKEY_Device_Capabilities),
	DECL_TUPLE_EX_W(DEVPKEY_Device_UINumber),
	DECL_TUPLE_EX_W(DEVPKEY_Device_UpperFilters),
	DECL_TUPLE_EX_W(DEVPKEY_Device_LowerFilters),
	DECL_TUPLE_EX_W(DEVPKEY_Device_BusTypeGuid),
	DECL_TUPLE_EX_W(DEVPKEY_Device_LegacyBusType),
	DECL_TUPLE_EX_W(DEVPKEY_Device_BusNumber),
	DECL_TUPLE_EX_W(DEVPKEY_Device_EnumeratorName),
	DECL_TUPLE_EX_W(DEVPKEY_Device_Security),
	DECL_TUPLE_EX_W(DEVPKEY_Device_SecuritySDS),
	DECL_TUPLE_EX_W(DEVPKEY_Device_DevType),
	DECL_TUPLE_EX_W(DEVPKEY_Device_Exclusive),
	DECL_TUPLE_EX_W(DEVPKEY_Device_Characteristics),
	DECL_TUPLE_EX_W(DEVPKEY_Device_Address),
	DECL_TUPLE_EX_W(DEVPKEY_Device_UINumberDescFormat),
	DECL_TUPLE_EX_W(DEVPKEY_Device_PowerData),
	DECL_TUPLE_EX_W(DEVPKEY_Device_RemovalPolicy),
	DECL_TUPLE_EX_W(DEVPKEY_Device_RemovalPolicyDefault),
	DECL_TUPLE_EX_W(DEVPKEY_Device_RemovalPolicyOverride),
	DECL_TUPLE_EX_W(DEVPKEY_Device_InstallState),
	DECL_TUPLE_EX_W(DEVPKEY_Device_LocationPaths),
	DECL_TUPLE_EX_W(DEVPKEY_Device_BaseContainerId),
	DECL_TUPLE_EX_W(DEVPKEY_Device_InstanceId),
	DECL_TUPLE_EX_W(DEVPKEY_Device_DevNodeStatus),
	DECL_TUPLE_EX_W(DEVPKEY_Device_ProblemCode),
	DECL_TUPLE_EX_W(DEVPKEY_Device_EjectionRelations),
	DECL_TUPLE_EX_W(DEVPKEY_Device_RemovalRelations),
	DECL_TUPLE_EX_W(DEVPKEY_Device_PowerRelations),
	DECL_TUPLE_EX_W(DEVPKEY_Device_BusRelations),
	DECL_TUPLE_EX_W(DEVPKEY_Device_Parent),
	DECL_TUPLE_EX_W(DEVPKEY_Device_Children),
	DECL_TUPLE_EX_W(DEVPKEY_Device_Siblings),
	DECL_TUPLE_EX_W(DEVPKEY_Device_TransportRelations),
	DECL_TUPLE_EX_W(DEVPKEY_Device_ProblemStatus),
	DECL_TUPLE_EX_W(DEVPKEY_Device_Reported),
	DECL_TUPLE_EX_W(DEVPKEY_Device_Legacy),
	DECL_TUPLE_EX_W(DEVPKEY_Device_ContainerId),
	DECL_TUPLE_EX_W(DEVPKEY_Device_InLocalMachineContainer),
	DECL_TUPLE_EX_W(DEVPKEY_Device_Model),
	DECL_TUPLE_EX_W(DEVPKEY_Device_ModelId),
	DECL_TUPLE_EX_W(DEVPKEY_Device_FriendlyNameAttributes),
	DECL_TUPLE_EX_W(DEVPKEY_Device_ManufacturerAttributes),
	DECL_TUPLE_EX_W(DEVPKEY_Device_PresenceNotForDevice),
	DECL_TUPLE_EX_W(DEVPKEY_Device_SignalStrength),
	DECL_TUPLE_EX_W(DEVPKEY_Device_IsAssociateableByUserAction),
	DECL_TUPLE_EX_W(DEVPKEY_Device_ShowInUninstallUI),
	DECL_TUPLE_EX_W(DEVPKEY_Device_Numa_Proximity_Domain),
	DECL_TUPLE_EX_W(DEVPKEY_Device_DHP_Rebalance_Policy),
	DECL_TUPLE_EX_W(DEVPKEY_Device_Numa_Node),
	DECL_TUPLE_EX_W(DEVPKEY_Device_BusReportedDeviceDesc),
	DECL_TUPLE_EX_W(DEVPKEY_Device_IsPresent),
	DECL_TUPLE_EX_W(DEVPKEY_Device_HasProblem),
	DECL_TUPLE_EX_W(DEVPKEY_Device_ConfigurationId),
	DECL_TUPLE_EX_W(DEVPKEY_Device_ReportedDeviceIdsHash),
	DECL_TUPLE_EX_W(DEVPKEY_Device_PhysicalDeviceLocation),
	DECL_TUPLE_EX_W(DEVPKEY_Device_BiosDeviceName),
	DECL_TUPLE_EX_W(DEVPKEY_Device_DriverProblemDesc),
	DECL_TUPLE_EX_W(DEVPKEY_Device_DebuggerSafe),
	DECL_TUPLE_EX_W(DEVPKEY_Device_PostInstallInProgress),
	DECL_TUPLE_EX_W(DEVPKEY_Device_Stack),
	DECL_TUPLE_EX_W(DEVPKEY_Device_ExtendedConfigurationIds),
	DECL_TUPLE_EX_W(DEVPKEY_Device_IsRebootRequired),
	DECL_TUPLE_EX_W(DEVPKEY_Device_FirmwareDate),
	DECL_TUPLE_EX_W(DEVPKEY_Device_FirmwareVersion),
	DECL_TUPLE_EX_W(DEVPKEY_Device_FirmwareRevision),
	DECL_TUPLE_EX_W(DEVPKEY_Device_DependencyProviders),
	DECL_TUPLE_EX_W(DEVPKEY_Device_DependencyDependents),
	DECL_TUPLE_EX_W(DEVPKEY_Device_SoftRestartSupported),
	DECL_TUPLE_EX_W(DEVPKEY_Device_ExtendedAddress),
	DECL_TUPLE_EX_W(DEVPKEY_Device_SessionId),
	DECL_TUPLE_EX_W(DEVPKEY_Device_InstallDate),
	DECL_TUPLE_EX_W(DEVPKEY_Device_FirstInstallDate),
	DECL_TUPLE_EX_W(DEVPKEY_Device_LastArrivalDate),
	DECL_TUPLE_EX_W(DEVPKEY_Device_LastRemovalDate),
	DECL_TUPLE_EX_W(DEVPKEY_Device_DriverDate),
	DECL_TUPLE_EX_W(DEVPKEY_Device_DriverVersion),
	DECL_TUPLE_EX_W(DEVPKEY_Device_DriverDesc),
	DECL_TUPLE_EX_W(DEVPKEY_Device_DriverInfPath),
	DECL_TUPLE_EX_W(DEVPKEY_Device_DriverInfSection),
	DECL_TUPLE_EX_W(DEVPKEY_Device_DriverInfSectionExt),
	DECL_TUPLE_EX_W(DEVPKEY_Device_MatchingDeviceId),
	DECL_TUPLE_EX_W(DEVPKEY_Device_DriverProvider),
	DECL_TUPLE_EX_W(DEVPKEY_Device_DriverPropPageProvider),
	DECL_TUPLE_EX_W(DEVPKEY_Device_DriverCoInstallers),
	DECL_TUPLE_EX_W(DEVPKEY_Device_ResourcePickerTags),
	DECL_TUPLE_EX_W(DEVPKEY_Device_ResourcePickerExceptions),
	DECL_TUPLE_EX_W(DEVPKEY_Device_DriverRank),
	DECL_TUPLE_EX_W(DEVPKEY_Device_DriverLogoLevel),
	DECL_TUPLE_EX_W(DEVPKEY_Device_NoConnectSound),
	DECL_TUPLE_EX_W(DEVPKEY_Device_GenericDriverInstalled),
	DECL_TUPLE_EX_W(DEVPKEY_Device_AdditionalSoftwareRequested),
	DECL_TUPLE_EX_W(DEVPKEY_Device_SafeRemovalRequired),
	DECL_TUPLE_EX_W(DEVPKEY_Device_SafeRemovalRequiredOverride),
	DECL_TUPLE_EX_W(DEVPKEY_DrvPkg_Model),
	DECL_TUPLE_EX_W(DEVPKEY_DrvPkg_VendorWebSite),
	DECL_TUPLE_EX_W(DEVPKEY_DrvPkg_DetailedDescription),
	DECL_TUPLE_EX_W(DEVPKEY_DrvPkg_DocumentationLink),
	DECL_TUPLE_EX_W(DEVPKEY_DrvPkg_Icon),
	DECL_TUPLE_EX_W(DEVPKEY_DrvPkg_BrandingIcon),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceClass_UpperFilters),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceClass_LowerFilters),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceClass_Security),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceClass_SecuritySDS),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceClass_DevType),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceClass_Exclusive),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceClass_Characteristics),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceClass_Name),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceClass_ClassName),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceClass_Icon),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceClass_ClassInstaller),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceClass_PropPageProvider),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceClass_NoInstallClass),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceClass_NoDisplayClass),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceClass_SilentInstall),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceClass_NoUseClass),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceClass_DefaultService),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceClass_IconPath),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceClass_DHPRebalanceOptOut),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceClass_ClassCoInstallers),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceInterface_FriendlyName),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceInterface_Enabled),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceInterface_ClassGuid),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceInterface_ReferenceString),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceInterface_Restricted),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceInterface_UnrestrictedAppCapabilities),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceInterfaceClass_DefaultInterface),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceInterfaceClass_Name),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_Address),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_DiscoveryMethod),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_IsEncrypted),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_IsAuthenticated),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_IsConnected),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_IsPaired),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_Icon),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_Version),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_Last_Seen),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_Last_Connected),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_IsShowInDisconnectedState),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_IsLocalMachine),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_MetadataPath),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_IsMetadataSearchInProgress),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_MetadataChecksum),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_IsNotInterestingForDisplay),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_LaunchDeviceStageOnDeviceConnect),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_LaunchDeviceStageFromExplorer),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_BaselineExperienceId),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_IsDeviceUniquelyIdentifiable),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_AssociationArray),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_DeviceDescription1),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_DeviceDescription2),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_HasProblem),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_IsSharedDevice),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_IsNetworkDevice),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_IsDefaultDevice),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_MetadataCabinet),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_RequiresPairingElevation),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_ExperienceId),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_Category),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_Category_Desc_Singular),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_Category_Desc_Plural),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_Category_Icon),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_CategoryGroup_Desc),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_CategoryGroup_Icon),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_PrimaryCategory),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_UnpairUninstall),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_RequiresUninstallElevation),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_DeviceFunctionSubRank),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_AlwaysShowDeviceAsConnected),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_ConfigFlags),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_PrivilegedPackageFamilyNames),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_CustomPrivilegedPackageFamilyNames),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_IsRebootRequired),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_FriendlyName),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_Manufacturer),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_ModelName),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_ModelNumber),
	DECL_TUPLE_EX_W(DEVPKEY_DeviceContainer_InstallInProgress),
	DECL_TUPLE_EX_W(DEVPKEY_DevQuery_ObjectType),
};

FLAG_NAME_PAIR CMMDeviceUtility::EndpointFormFactor_flag_names[] = {
	DECL_TUPLE_W(RemoteNetworkDevice),
	DECL_TUPLE_W(Speakers),
	DECL_TUPLE_W(LineLevel),
	DECL_TUPLE_W(Headphones),
	DECL_TUPLE_W(Microphone),
	DECL_TUPLE_W(Headset),
	DECL_TUPLE_W(Handset),
	DECL_TUPLE_W(UnknownDigitalPassthrough),
	DECL_TUPLE_W(SPDIF),
	DECL_TUPLE_W(DigitalAudioDisplayDevice),
	DECL_TUPLE_W(UnknownFormFactor),
};

FLAG_NAME_PAIR CMMDeviceUtility::WaveFormatTag_flag_names[] = {
	DECL_TUPLE_W(WAVE_FORMAT_UNKNOWN),
	DECL_TUPLE_W(WAVE_FORMAT_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_IEEE_FLOAT),
	DECL_TUPLE_W(WAVE_FORMAT_VSELP),
	DECL_TUPLE_W(WAVE_FORMAT_IBM_CVSD),
	DECL_TUPLE_W(WAVE_FORMAT_ALAW),
	DECL_TUPLE_W(WAVE_FORMAT_MULAW),
	DECL_TUPLE_W(WAVE_FORMAT_DTS),
	DECL_TUPLE_W(WAVE_FORMAT_DRM),
	DECL_TUPLE_W(WAVE_FORMAT_WMAVOICE9),
	DECL_TUPLE_W(WAVE_FORMAT_WMAVOICE10),
	DECL_TUPLE_W(WAVE_FORMAT_OKI_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_DVI_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_IMA_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_MEDIASPACE_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_SIERRA_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_G723_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_DIGISTD),
	DECL_TUPLE_W(WAVE_FORMAT_DIGIFIX),
	DECL_TUPLE_W(WAVE_FORMAT_DIALOGIC_OKI_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_MEDIAVISION_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_CU_CODEC),
	DECL_TUPLE_W(WAVE_FORMAT_HP_DYN_VOICE),
	DECL_TUPLE_W(WAVE_FORMAT_YAMAHA_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_SONARC),
	DECL_TUPLE_W(WAVE_FORMAT_DSPGROUP_TRUESPEECH),
	DECL_TUPLE_W(WAVE_FORMAT_ECHOSC1),
	DECL_TUPLE_W(WAVE_FORMAT_AUDIOFILE_AF36),
	DECL_TUPLE_W(WAVE_FORMAT_APTX),
	DECL_TUPLE_W(WAVE_FORMAT_AUDIOFILE_AF10),
	DECL_TUPLE_W(WAVE_FORMAT_PROSODY_1612),
	DECL_TUPLE_W(WAVE_FORMAT_LRC),
	DECL_TUPLE_W(WAVE_FORMAT_DOLBY_AC2),
	DECL_TUPLE_W(WAVE_FORMAT_GSM610),
	DECL_TUPLE_W(WAVE_FORMAT_MSNAUDIO),
	DECL_TUPLE_W(WAVE_FORMAT_ANTEX_ADPCME),
	DECL_TUPLE_W(WAVE_FORMAT_CONTROL_RES_VQLPC),
	DECL_TUPLE_W(WAVE_FORMAT_DIGIREAL),
	DECL_TUPLE_W(WAVE_FORMAT_DIGIADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_CONTROL_RES_CR10),
	DECL_TUPLE_W(WAVE_FORMAT_NMS_VBXADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_CS_IMAADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_ECHOSC3),
	DECL_TUPLE_W(WAVE_FORMAT_ROCKWELL_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_ROCKWELL_DIGITALK),
	DECL_TUPLE_W(WAVE_FORMAT_XEBEC),
	DECL_TUPLE_W(WAVE_FORMAT_G721_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_G728_CELP),
	DECL_TUPLE_W(WAVE_FORMAT_MSG723),
	DECL_TUPLE_W(WAVE_FORMAT_INTEL_G723_1),
	DECL_TUPLE_W(WAVE_FORMAT_INTEL_G729),
	DECL_TUPLE_W(WAVE_FORMAT_SHARP_G726),
	DECL_TUPLE_W(WAVE_FORMAT_MPEG),
	DECL_TUPLE_W(WAVE_FORMAT_RT24),
	DECL_TUPLE_W(WAVE_FORMAT_PAC),
	DECL_TUPLE_W(WAVE_FORMAT_MPEGLAYER3),
	DECL_TUPLE_W(WAVE_FORMAT_LUCENT_G723),
	DECL_TUPLE_W(WAVE_FORMAT_CIRRUS),
	DECL_TUPLE_W(WAVE_FORMAT_ESPCM),
	DECL_TUPLE_W(WAVE_FORMAT_VOXWARE),
	DECL_TUPLE_W(WAVE_FORMAT_CANOPUS_ATRAC),
	DECL_TUPLE_W(WAVE_FORMAT_G726_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_G722_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_DSAT),
	DECL_TUPLE_W(WAVE_FORMAT_DSAT_DISPLAY),
	DECL_TUPLE_W(WAVE_FORMAT_VOXWARE_BYTE_ALIGNED),
	DECL_TUPLE_W(WAVE_FORMAT_VOXWARE_AC8),
	DECL_TUPLE_W(WAVE_FORMAT_VOXWARE_AC10),
	DECL_TUPLE_W(WAVE_FORMAT_VOXWARE_AC16),
	DECL_TUPLE_W(WAVE_FORMAT_VOXWARE_AC20),
	DECL_TUPLE_W(WAVE_FORMAT_VOXWARE_RT24),
	DECL_TUPLE_W(WAVE_FORMAT_VOXWARE_RT29),
	DECL_TUPLE_W(WAVE_FORMAT_VOXWARE_RT29HW),
	DECL_TUPLE_W(WAVE_FORMAT_VOXWARE_VR12),
	DECL_TUPLE_W(WAVE_FORMAT_VOXWARE_VR18),
	DECL_TUPLE_W(WAVE_FORMAT_VOXWARE_TQ40),
	DECL_TUPLE_W(WAVE_FORMAT_VOXWARE_SC3),
	DECL_TUPLE_W(WAVE_FORMAT_VOXWARE_SC3_1),
	DECL_TUPLE_W(WAVE_FORMAT_SOFTSOUND),
	DECL_TUPLE_W(WAVE_FORMAT_VOXWARE_TQ60),
	DECL_TUPLE_W(WAVE_FORMAT_MSRT24),
	DECL_TUPLE_W(WAVE_FORMAT_G729A),
	DECL_TUPLE_W(WAVE_FORMAT_MVI_MVI2),
	DECL_TUPLE_W(WAVE_FORMAT_DF_G726),
	DECL_TUPLE_W(WAVE_FORMAT_DF_GSM610),
	DECL_TUPLE_W(WAVE_FORMAT_ISIAUDIO),
	DECL_TUPLE_W(WAVE_FORMAT_ONLIVE),
	DECL_TUPLE_W(WAVE_FORMAT_MULTITUDE_FT_SX20),
	DECL_TUPLE_W(WAVE_FORMAT_INFOCOM_ITS_G721_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_CONVEDIA_G729),
	DECL_TUPLE_W(WAVE_FORMAT_CONGRUENCY),
	DECL_TUPLE_W(WAVE_FORMAT_SBC24),
	DECL_TUPLE_W(WAVE_FORMAT_DOLBY_AC3_SPDIF),
	DECL_TUPLE_W(WAVE_FORMAT_MEDIASONIC_G723),
	DECL_TUPLE_W(WAVE_FORMAT_PROSODY_8KBPS),
	DECL_TUPLE_W(WAVE_FORMAT_ZYXEL_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_PHILIPS_LPCBB),
	DECL_TUPLE_W(WAVE_FORMAT_PACKED),
	DECL_TUPLE_W(WAVE_FORMAT_MALDEN_PHONYTALK),
	DECL_TUPLE_W(WAVE_FORMAT_RACAL_RECORDER_GSM),
	DECL_TUPLE_W(WAVE_FORMAT_RACAL_RECORDER_G720_A),
	DECL_TUPLE_W(WAVE_FORMAT_RACAL_RECORDER_G723_1),
	DECL_TUPLE_W(WAVE_FORMAT_RACAL_RECORDER_TETRA_ACELP),
	DECL_TUPLE_W(WAVE_FORMAT_NEC_AAC),
	DECL_TUPLE_W(WAVE_FORMAT_RAW_AAC1),
	DECL_TUPLE_W(WAVE_FORMAT_RHETOREX_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_IRAT),
	DECL_TUPLE_W(WAVE_FORMAT_VIVO_G723),
	DECL_TUPLE_W(WAVE_FORMAT_VIVO_SIREN),
	DECL_TUPLE_W(WAVE_FORMAT_PHILIPS_CELP),
	DECL_TUPLE_W(WAVE_FORMAT_PHILIPS_GRUNDIG),
	DECL_TUPLE_W(WAVE_FORMAT_DIGITAL_G723),
	DECL_TUPLE_W(WAVE_FORMAT_SANYO_LD_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_SIPROLAB_ACEPLNET),
	DECL_TUPLE_W(WAVE_FORMAT_SIPROLAB_ACELP4800),
	DECL_TUPLE_W(WAVE_FORMAT_SIPROLAB_ACELP8V3),
	DECL_TUPLE_W(WAVE_FORMAT_SIPROLAB_G729),
	DECL_TUPLE_W(WAVE_FORMAT_SIPROLAB_G729A),
	DECL_TUPLE_W(WAVE_FORMAT_SIPROLAB_KELVIN),
	DECL_TUPLE_W(WAVE_FORMAT_VOICEAGE_AMR),
	DECL_TUPLE_W(WAVE_FORMAT_G726ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_DICTAPHONE_CELP68),
	DECL_TUPLE_W(WAVE_FORMAT_DICTAPHONE_CELP54),
	DECL_TUPLE_W(WAVE_FORMAT_QUALCOMM_PUREVOICE),
	DECL_TUPLE_W(WAVE_FORMAT_QUALCOMM_HALFRATE),
	DECL_TUPLE_W(WAVE_FORMAT_TUBGSM),
	DECL_TUPLE_W(WAVE_FORMAT_MSAUDIO1),
	DECL_TUPLE_W(WAVE_FORMAT_WMAUDIO2),
	DECL_TUPLE_W(WAVE_FORMAT_WMAUDIO3),
	DECL_TUPLE_W(WAVE_FORMAT_WMAUDIO_LOSSLESS),
	DECL_TUPLE_W(WAVE_FORMAT_WMASPDIF),
	DECL_TUPLE_W(WAVE_FORMAT_UNISYS_NAP_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_UNISYS_NAP_ULAW),
	DECL_TUPLE_W(WAVE_FORMAT_UNISYS_NAP_ALAW),
	DECL_TUPLE_W(WAVE_FORMAT_UNISYS_NAP_16K),
	DECL_TUPLE_W(WAVE_FORMAT_SYCOM_ACM_SYC008),
	DECL_TUPLE_W(WAVE_FORMAT_SYCOM_ACM_SYC701_G726L),
	DECL_TUPLE_W(WAVE_FORMAT_SYCOM_ACM_SYC701_CELP54),
	DECL_TUPLE_W(WAVE_FORMAT_SYCOM_ACM_SYC701_CELP68),
	DECL_TUPLE_W(WAVE_FORMAT_KNOWLEDGE_ADVENTURE_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_FRAUNHOFER_IIS_MPEG2_AAC),
	DECL_TUPLE_W(WAVE_FORMAT_DTS_DS),
	DECL_TUPLE_W(WAVE_FORMAT_CREATIVE_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_CREATIVE_FASTSPEECH8),
	DECL_TUPLE_W(WAVE_FORMAT_CREATIVE_FASTSPEECH10),
	DECL_TUPLE_W(WAVE_FORMAT_UHER_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_ULEAD_DV_AUDIO),
	DECL_TUPLE_W(WAVE_FORMAT_ULEAD_DV_AUDIO_1),
	DECL_TUPLE_W(WAVE_FORMAT_QUARTERDECK),
	DECL_TUPLE_W(WAVE_FORMAT_ILINK_VC),
	DECL_TUPLE_W(WAVE_FORMAT_RAW_SPORT),
	DECL_TUPLE_W(WAVE_FORMAT_ESST_AC3),
	DECL_TUPLE_W(WAVE_FORMAT_GENERIC_PASSTHRU),
	DECL_TUPLE_W(WAVE_FORMAT_IPI_HSX),
	DECL_TUPLE_W(WAVE_FORMAT_IPI_RPELP),
	DECL_TUPLE_W(WAVE_FORMAT_CS2),
	DECL_TUPLE_W(WAVE_FORMAT_SONY_SCX),
	DECL_TUPLE_W(WAVE_FORMAT_SONY_SCY),
	DECL_TUPLE_W(WAVE_FORMAT_SONY_ATRAC3),
	DECL_TUPLE_W(WAVE_FORMAT_SONY_SPC),
	DECL_TUPLE_W(WAVE_FORMAT_TELUM_AUDIO),
	DECL_TUPLE_W(WAVE_FORMAT_TELUM_IA_AUDIO),
	DECL_TUPLE_W(WAVE_FORMAT_NORCOM_VOICE_SYSTEMS_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_FM_TOWNS_SND),
	DECL_TUPLE_W(WAVE_FORMAT_MICRONAS),
	DECL_TUPLE_W(WAVE_FORMAT_MICRONAS_CELP833),
	DECL_TUPLE_W(WAVE_FORMAT_BTV_DIGITAL),
	DECL_TUPLE_W(WAVE_FORMAT_INTEL_MUSIC_CODER),
	DECL_TUPLE_W(WAVE_FORMAT_INDEO_AUDIO),
	DECL_TUPLE_W(WAVE_FORMAT_QDESIGN_MUSIC),
	DECL_TUPLE_W(WAVE_FORMAT_ON2_VP7_AUDIO),
	DECL_TUPLE_W(WAVE_FORMAT_ON2_VP6_AUDIO),
	DECL_TUPLE_W(WAVE_FORMAT_VME_VMPCM),
	DECL_TUPLE_W(WAVE_FORMAT_TPC),
	DECL_TUPLE_W(WAVE_FORMAT_LIGHTWAVE_LOSSLESS),
	DECL_TUPLE_W(WAVE_FORMAT_OLIGSM),
	DECL_TUPLE_W(WAVE_FORMAT_OLIADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_OLICELP),
	DECL_TUPLE_W(WAVE_FORMAT_OLISBC),
	DECL_TUPLE_W(WAVE_FORMAT_OLIOPR),
	DECL_TUPLE_W(WAVE_FORMAT_LH_CODEC),
	DECL_TUPLE_W(WAVE_FORMAT_LH_CODEC_CELP),
	DECL_TUPLE_W(WAVE_FORMAT_LH_CODEC_SBC8),
	DECL_TUPLE_W(WAVE_FORMAT_LH_CODEC_SBC12),
	DECL_TUPLE_W(WAVE_FORMAT_LH_CODEC_SBC16),
	DECL_TUPLE_W(WAVE_FORMAT_NORRIS),
	DECL_TUPLE_W(WAVE_FORMAT_ISIAUDIO_2),
	DECL_TUPLE_W(WAVE_FORMAT_SOUNDSPACE_MUSICOMPRESS),
	DECL_TUPLE_W(WAVE_FORMAT_MPEG_ADTS_AAC),
	DECL_TUPLE_W(WAVE_FORMAT_MPEG_RAW_AAC),
	DECL_TUPLE_W(WAVE_FORMAT_MPEG_LOAS),
	DECL_TUPLE_W(WAVE_FORMAT_NOKIA_MPEG_ADTS_AAC),
	DECL_TUPLE_W(WAVE_FORMAT_NOKIA_MPEG_RAW_AAC),
	DECL_TUPLE_W(WAVE_FORMAT_VODAFONE_MPEG_ADTS_AAC),
	DECL_TUPLE_W(WAVE_FORMAT_VODAFONE_MPEG_RAW_AAC),
	DECL_TUPLE_W(WAVE_FORMAT_MPEG_HEAAC),
	DECL_TUPLE_W(WAVE_FORMAT_VOXWARE_RT24_SPEECH),
	DECL_TUPLE_W(WAVE_FORMAT_SONICFOUNDRY_LOSSLESS),
	DECL_TUPLE_W(WAVE_FORMAT_INNINGS_TELECOM_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_LUCENT_SX8300P),
	DECL_TUPLE_W(WAVE_FORMAT_LUCENT_SX5363S),
	DECL_TUPLE_W(WAVE_FORMAT_CUSEEME),
	DECL_TUPLE_W(WAVE_FORMAT_NTCSOFT_ALF2CM_ACM),
	DECL_TUPLE_W(WAVE_FORMAT_DVM),
	DECL_TUPLE_W(WAVE_FORMAT_DTS2),
	DECL_TUPLE_W(WAVE_FORMAT_MAKEAVIS),
	DECL_TUPLE_W(WAVE_FORMAT_DIVIO_MPEG4_AAC),
	DECL_TUPLE_W(WAVE_FORMAT_NOKIA_ADAPTIVE_MULTIRATE),
	DECL_TUPLE_W(WAVE_FORMAT_DIVIO_G726),
	DECL_TUPLE_W(WAVE_FORMAT_LEAD_SPEECH),
	DECL_TUPLE_W(WAVE_FORMAT_LEAD_VORBIS),
	DECL_TUPLE_W(WAVE_FORMAT_WAVPACK_AUDIO),
	DECL_TUPLE_W(WAVE_FORMAT_ALAC),
	DECL_TUPLE_W(WAVE_FORMAT_OGG_VORBIS_MODE_1),
	DECL_TUPLE_W(WAVE_FORMAT_OGG_VORBIS_MODE_2),
	DECL_TUPLE_W(WAVE_FORMAT_OGG_VORBIS_MODE_3),
	DECL_TUPLE_W(WAVE_FORMAT_OGG_VORBIS_MODE_1_PLUS),
	DECL_TUPLE_W(WAVE_FORMAT_OGG_VORBIS_MODE_2_PLUS),
	DECL_TUPLE_W(WAVE_FORMAT_OGG_VORBIS_MODE_3_PLUS),
	DECL_TUPLE_W(WAVE_FORMAT_3COM_NBX),
	DECL_TUPLE_W(WAVE_FORMAT_OPUS),
	DECL_TUPLE_W(WAVE_FORMAT_FAAD_AAC),
	DECL_TUPLE_W(WAVE_FORMAT_AMR_NB),
	DECL_TUPLE_W(WAVE_FORMAT_AMR_WB),
	DECL_TUPLE_W(WAVE_FORMAT_AMR_WP),
	DECL_TUPLE_W(WAVE_FORMAT_GSM_AMR_CBR),
	DECL_TUPLE_W(WAVE_FORMAT_GSM_AMR_VBR_SID),
	DECL_TUPLE_W(WAVE_FORMAT_COMVERSE_INFOSYS_G723_1),
	DECL_TUPLE_W(WAVE_FORMAT_COMVERSE_INFOSYS_AVQSBC),
	DECL_TUPLE_W(WAVE_FORMAT_COMVERSE_INFOSYS_SBC),
	DECL_TUPLE_W(WAVE_FORMAT_SYMBOL_G729_A),
	DECL_TUPLE_W(WAVE_FORMAT_VOICEAGE_AMR_WB),
	DECL_TUPLE_W(WAVE_FORMAT_INGENIENT_G726),
	DECL_TUPLE_W(WAVE_FORMAT_MPEG4_AAC),
	DECL_TUPLE_W(WAVE_FORMAT_ENCORE_G726),
	DECL_TUPLE_W(WAVE_FORMAT_ZOLL_ASAO),
	DECL_TUPLE_W(WAVE_FORMAT_SPEEX_VOICE),
	DECL_TUPLE_W(WAVE_FORMAT_VIANIX_MASC),
	DECL_TUPLE_W(WAVE_FORMAT_WM9_SPECTRUM_ANALYZER),
	DECL_TUPLE_W(WAVE_FORMAT_WMF_SPECTRUM_ANAYZER),
	DECL_TUPLE_W(WAVE_FORMAT_GSM_610),
	DECL_TUPLE_W(WAVE_FORMAT_GSM_620),
	DECL_TUPLE_W(WAVE_FORMAT_GSM_660),
	DECL_TUPLE_W(WAVE_FORMAT_GSM_690),
	DECL_TUPLE_W(WAVE_FORMAT_GSM_ADAPTIVE_MULTIRATE_WB),
	DECL_TUPLE_W(WAVE_FORMAT_POLYCOM_G722),
	DECL_TUPLE_W(WAVE_FORMAT_POLYCOM_G728),
	DECL_TUPLE_W(WAVE_FORMAT_POLYCOM_G729_A),
	DECL_TUPLE_W(WAVE_FORMAT_POLYCOM_SIREN),
	DECL_TUPLE_W(WAVE_FORMAT_GLOBAL_IP_ILBC),
	DECL_TUPLE_W(WAVE_FORMAT_RADIOTIME_TIME_SHIFT_RADIO),
	DECL_TUPLE_W(WAVE_FORMAT_NICE_ACA),
	DECL_TUPLE_W(WAVE_FORMAT_NICE_ADPCM),
	DECL_TUPLE_W(WAVE_FORMAT_VOCORD_G721),
	DECL_TUPLE_W(WAVE_FORMAT_VOCORD_G726),
	DECL_TUPLE_W(WAVE_FORMAT_VOCORD_G722_1),
	DECL_TUPLE_W(WAVE_FORMAT_VOCORD_G728),
	DECL_TUPLE_W(WAVE_FORMAT_VOCORD_G729),
	DECL_TUPLE_W(WAVE_FORMAT_VOCORD_G729_A),
	DECL_TUPLE_W(WAVE_FORMAT_VOCORD_G723_1),
	DECL_TUPLE_W(WAVE_FORMAT_VOCORD_LBC),
	DECL_TUPLE_W(WAVE_FORMAT_NICE_G728),
	DECL_TUPLE_W(WAVE_FORMAT_FRACE_TELECOM_G729),
	DECL_TUPLE_W(WAVE_FORMAT_CODIAN),
	DECL_TUPLE_W(WAVE_FORMAT_FLAC),
	DECL_TUPLE_W(WAVE_FORMAT_EXTENSIBLE),
	DECL_TUPLE_W(WAVE_FORMAT_DEVELOPMENT),
};


FLAG_NAME_PAIR CMMDeviceUtility::Speaker_position_flag_names[] = {
	DECL_TUPLE_W(SPEAKER_FRONT_LEFT),
	DECL_TUPLE_W(SPEAKER_FRONT_RIGHT),
	DECL_TUPLE_W(SPEAKER_FRONT_CENTER),
	DECL_TUPLE_W(SPEAKER_LOW_FREQUENCY),
	DECL_TUPLE_W(SPEAKER_BACK_LEFT),
	DECL_TUPLE_W(SPEAKER_BACK_RIGHT),
	DECL_TUPLE_W(SPEAKER_FRONT_LEFT_OF_CENTER),
	DECL_TUPLE_W(SPEAKER_FRONT_RIGHT_OF_CENTER),
	DECL_TUPLE_W(SPEAKER_BACK_CENTER),
	DECL_TUPLE_W(SPEAKER_SIDE_LEFT),
	DECL_TUPLE_W(SPEAKER_SIDE_RIGHT),
	DECL_TUPLE_W(SPEAKER_TOP_CENTER),
	DECL_TUPLE_W(SPEAKER_TOP_FRONT_LEFT),
	DECL_TUPLE_W(SPEAKER_TOP_FRONT_CENTER),
	DECL_TUPLE_W(SPEAKER_TOP_FRONT_RIGHT),
	DECL_TUPLE_W(SPEAKER_TOP_BACK_LEFT),
	DECL_TUPLE_W(SPEAKER_TOP_BACK_CENTER),
	DECL_TUPLE_W(SPEAKER_TOP_BACK_RIGHT),
};

CMMDeviceUtility::CMMDeviceUtility()
{
}


CMMDeviceUtility::~CMMDeviceUtility()
{
}

int CMMDeviceUtility::GetDeviceTitleName(IMMDevice* pDevice, WCHAR* wszTitleName, int ccTitleName)
{
	if (pDevice == nullptr)
		return -1;

	LPWSTR strID = nullptr;
	if (FAILED(pDevice->GetId(&strID)))
		return -1;

	int ccWritten = swprintf_s(wszTitleName, ccTitleName, L"%s", strID);
	CoTaskMemFree(strID);

	ComPtr<IPropertyStore> spPropStore;
	if (SUCCEEDED(pDevice->OpenPropertyStore(STGM_READ, &spPropStore)))
	{
		PROPVARIANT propvar;
		PropVariantInit(&propvar);
		if (SUCCEEDED(spPropStore->GetValue(*((const PROPERTYKEY*)&DEVPKEY_DeviceInterface_FriendlyName), &propvar)))
		{
			ccWritten += swprintf_s(wszTitleName + ccWritten, ccTitleName - ccWritten, L"/");
			ccWritten += swprintf_s(wszTitleName + ccWritten, ccTitleName - ccWritten, L"%s", propvar.pwszVal);
		}
		PropVariantClear(&propvar);
	}

	return 0;
}

int CMMDeviceUtility::GetDevicePropDesc(IPropertyStore* pPropStore, PROPERTYKEY propkey, WCHAR* szPropDesc, int ccPropDesc, size_t indent)
{
	HRESULT hr = S_OK;
	PROPVARIANT propval;
	WCHAR wszTmp[2048];
	WCHAR wszIndent[1024];
	int ccWritten = 0;

	if (indent >= _countof(wszIndent))
	{
		printf("[MMDeviceUtil] indent: %zu should NOT exceed %zu.\n", indent, _countof(wszIndent) - 1);
		return RET_CODE_INVALID_PARAMETER;
	}

	wmemset(wszIndent, L' ', indent);
	wszIndent[indent] = L'\0';

	const WCHAR* wszPKeyname = nullptr;
	for (size_t i = 0; i < _countof(CMMDeviceUtility::prop_key_names); i++)
	{
		if (std::get<0>(CMMDeviceUtility::prop_key_names[i]) == propkey)
		{
			wszPKeyname = std::get<1>(CMMDeviceUtility::prop_key_names[i]);
			break;
		}
	}

	if (wszPKeyname != nullptr)
		ccWritten = swprintf_s(wszTmp, _countof(wszTmp), L"%s%41s: ", wszIndent, wszPKeyname);
	else
		ccWritten = swprintf_s(wszTmp, _countof(wszTmp), L"%s{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}/%02u: ",
			wszIndent, propkey.fmtid.Data1, propkey.fmtid.Data2, propkey.fmtid.Data3,
			propkey.fmtid.Data4[0], propkey.fmtid.Data4[1], propkey.fmtid.Data4[2], propkey.fmtid.Data4[3],
			propkey.fmtid.Data4[4], propkey.fmtid.Data4[5], propkey.fmtid.Data4[6], propkey.fmtid.Data4[7], propkey.pid);

	if (ccPropDesc <= ccWritten)
		return RET_CODE_BUFFER_TOO_SMALL;

	BLOB blob = { 0, 0 };
	PropVariantInit(&propval);
	if (FAILED(hr = pPropStore->GetValue(propkey, &propval)))
	{
		ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"N/A (hr: 0X%X}\n", hr);

		if (ccPropDesc <= ccWritten)
			return RET_CODE_BUFFER_TOO_SMALL;
		else
			wcscpy_s(szPropDesc, ccPropDesc, wszTmp);
	}
	else
	{
		switch (propval.vt)
		{
		case VT_I2:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%d", propval.iVal);
			break;
		case VT_I4:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%d", propval.lVal);
			break;
		case VT_R4:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%f", propval.fltVal);
			break;
		case VT_R8:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%f", propval.dblVal);
			break;
		case VT_DATE:
		{
			SYSTEMTIME system_time;
			if (::VariantTimeToSystemTime(propval.date, &system_time))
				ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%u/%u/%u", system_time.wMonth, system_time.wDay, system_time.wYear);
		}
		break;
		case VT_FILETIME:
		{
			SYSTEMTIME system_time;
			if (::FileTimeToSystemTime(&propval.filetime, &system_time))
				ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%u/%u/%u", system_time.wMonth, system_time.wDay, system_time.wYear);
		}
		break;
		case VT_BOOL:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s", propval.boolVal ? L"TRUE" : L"FALSE");
			break;
		case VT_I1:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%d", propval.cVal);
			break;
		case VT_UI1:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%u", propval.bVal);
			break;
		case VT_UI2:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%u", propval.uiVal);
			break;
		case VT_UI4:
			if (propkey == PKEY_AudioEndpoint_FormFactor)
				ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s", GetEnumerateName(propval.ulVal, EndpointFormFactor_flag_names, _countof(EndpointFormFactor_flag_names)));
			else
				ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%u", propval.ulVal);
			break;
		case VT_I8:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%lld", propval.hVal.QuadPart);
			break;
		case VT_UI8:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%llu", propval.uhVal.QuadPart);
			break;
		case VT_INT:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%d", propval.intVal);
			break;
		case VT_UINT:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%u", propval.uintVal);
			break;
		case VT_VOID:
			break;
		case VT_HRESULT:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"0X%X", propval.lVal);
			break;
		case VT_PTR:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%p", propval.pintVal);
			break;
		case VT_INT_PTR:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%p", propval.pintVal);
			break;
		case VT_UINT_PTR:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%p", propval.puintVal);
			break;
		case VT_CLSID:
			if (propval.puuid != nullptr)
			{
				ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
					propval.puuid->Data1, propval.puuid->Data2, propval.puuid->Data3,
					propval.puuid->Data4[0], propval.puuid->Data4[1], propval.puuid->Data4[2], propval.puuid->Data4[3],
					propval.puuid->Data4[4], propval.puuid->Data4[5], propval.puuid->Data4[6], propval.puuid->Data4[7]);
			}
			break;
		case VT_BLOB:
			blob = propval.blob;
			break;
		case VT_STREAM:
		case VT_STORAGE:
		case VT_STREAMED_OBJECT:
		case VT_STORED_OBJECT:
		case VT_BLOB_OBJECT:
		case VT_CF:
		case VT_EMPTY:
		case VT_NULL:
		case VT_DECIMAL:
		case VT_VARIANT:
		case VT_UNKNOWN:
		case VT_DISPATCH:
		case VT_ERROR:
		case VT_BSTR:
		case VT_CY:
		case VT_SAFEARRAY:
		case VT_CARRAY:
		case VT_USERDEFINED:
		case VT_LPSTR:
		case VT_LPWSTR:
		case VT_RECORD:
		case VT_VERSIONED_STREAM:
		case VT_BSTR_BLOB:
		case VT_VECTOR:
		case VT_ARRAY:
		case VT_BYREF:
		case VT_RESERVED:
		case VT_ILLEGAL:
		default:
			if (SUCCEEDED(PropVariantToString(propval, wszTmp + ccWritten, _countof(wszTmp) - ccWritten)))
				ccWritten += wcslen(wszTmp + ccWritten);
		}

		ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"\n");

		if (ccPropDesc <= ccWritten)
			return RET_CODE_BUFFER_TOO_SMALL;
		else
			wcscpy_s(szPropDesc, ccPropDesc, wszTmp);

		if (blob.cbSize > 0 && blob.pBlobData != nullptr)
		{
			if (propkey == PKEY_AudioEngine_DeviceFormat || propkey == PKEY_AudioEngine_OEMFormat)
			{
				if (ccPropDesc == 0 || szPropDesc == nullptr)
					ccWritten += GetWaveFormatExDesc(blob.pBlobData, blob.cbSize, indent + 43, NULL, 0, L"|");
				else
					ccWritten += GetWaveFormatExDesc(blob.pBlobData, blob.cbSize, indent + 43, szPropDesc + ccWritten, ccPropDesc - ccWritten, L"|");
			}
			else
			{
				if (ccPropDesc == 0 || szPropDesc == nullptr)
					ccWritten += GetBufDesc(blob.pBlobData, blob.cbSize, indent + 43, NULL, 0);
				else
					ccWritten += GetBufDesc(blob.pBlobData, blob.cbSize, indent + 43, szPropDesc + ccWritten, ccPropDesc - ccWritten);
			}

			if (ccPropDesc <= ccWritten)
				return RET_CODE_BUFFER_TOO_SMALL;
		}
	}

	PropVariantClear(&propval);

	return ccWritten + 1;
}
