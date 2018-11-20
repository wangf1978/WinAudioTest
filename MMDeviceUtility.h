#pragma once

#include <mmdeviceapi.h>
#include <wrl/client.h>
#include <mmreg.h>
#include <ks.h>
#include <ksmedia.h>

using namespace Microsoft::WRL;

using FLAG_NAME_PAIR = const std::tuple<INT, const WCHAR*>;

#define KSAUDIO_SPEAKER_NAME(x)	(\
	DECL_ENUM_ITEMW(x, KSAUDIO_SPEAKER_DIRECTOUT)\
	DECL_ENUM_ITEMW(x, KSAUDIO_SPEAKER_MONO)\
	DECL_ENUM_ITEMW(x, KSAUDIO_SPEAKER_1POINT1)\
	DECL_ENUM_ITEMW(x, KSAUDIO_SPEAKER_STEREO)\
	DECL_ENUM_ITEMW(x, KSAUDIO_SPEAKER_2POINT1)\
	DECL_ENUM_ITEMW(x, KSAUDIO_SPEAKER_3POINT0)\
	DECL_ENUM_ITEMW(x, KSAUDIO_SPEAKER_3POINT1)\
	DECL_ENUM_ITEMW(x, KSAUDIO_SPEAKER_QUAD)\
	DECL_ENUM_ITEMW(x, KSAUDIO_SPEAKER_SURROUND)\
	DECL_ENUM_ITEMW(x, KSAUDIO_SPEAKER_5POINT0)\
	DECL_ENUM_ITEMW(x, KSAUDIO_SPEAKER_5POINT1)\
	DECL_ENUM_ITEMW(x, KSAUDIO_SPEAKER_5POINT0)\
	DECL_ENUM_ITEMW(x, KSAUDIO_SPEAKER_7POINT0)\
	DECL_ENUM_ITEMW(x, KSAUDIO_SPEAKER_7POINT1)\
	DECL_ENUM_ITEMW(x, KSAUDIO_SPEAKER_5POINT1_SURROUND)\
	DECL_ENUM_LAST_ITEMW(x, KSAUDIO_SPEAKER_7POINT1_SURROUND, NULL))))))))))))))))))

#define SUBFORMAT_FLAG_NAME(x)	(\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_WAVEFORMATEX)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_ANALOG)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_PCM)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_IEEE_FLOAT)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_DRM)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_ALAW)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_MULAW)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_ADPCM)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_MPEG)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_RIFF)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_RIFFWAVE)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_MIDI)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_MIDI_BUS)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_RIFFMIDI)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_STANDARD_MPEG1_VIDEO)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_STANDARD_MPEG1_AUDIO)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_STANDARD_MPEG2_VIDEO)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_STANDARD_MPEG2_AUDIO)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_STANDARD_AC3_AUDIO)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_DSS_VIDEO)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_DSS_AUDIO)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_MPEG1Packet)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_MPEG1Payload)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_MPEG1Video)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_MPEG2_VIDEO)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_MPEG2_AUDIO)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_LPCM_AUDIO)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_AC3_AUDIO)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_IEC61937_DOLBY_DIGITAL)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_IEC61937_WMA_PRO)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_IEC61937_DTS)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_IEC61937_MPEG1)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_IEC61937_MPEG2)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_IEC61937_MPEG3)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_IEC61937_AAC)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_IEC61937_ATRAC)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_IEC61937_ONE_BIT_AUDIO)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_IEC61937_DOLBY_DIGITAL_PLUS)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_IEC61937_DOLBY_DIGITAL_PLUS_ATMOS)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_IEC61937_DTS_HD)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_IEC61937_DOLBY_MLP)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_IEC61937_DOLBY_MAT20)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_IEC61937_DOLBY_MAT21)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_IEC61937_DST)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_MPEGLAYER3)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_MPEG_HEAAC)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_WMAUDIO2)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_WMAUDIO3)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_WMAUDIO_LOSSLESS)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_DTS_AUDIO)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_SDDS_AUDIO)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_SUBPICTURE)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_VPVideo)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_VPVBI)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_JPEG)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_IMAGE_RGB32)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_L8)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_L8_IR)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_L8_CUSTOM)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_L16)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_L16_IR)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_D16)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_L16_CUSTOM)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_MJPG_IR)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_MJPG_DEPTH)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_MJPG_CUSTOM)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_RAW8)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_CC)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_NABTS)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_TELETEXT)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_NABTS_FEC)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_OVERLAY)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_Line21_BytePair)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_Line21_GOPPacket)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_PCM)\
	DECL_ENUM_ITEMW(x, KSDATAFORMAT_SUBTYPE_IEEE_FLOAT)\
	DECL_ENUM_LAST_ITEMW(x, KSDATAFORMAT_SUBTYPE_WAVEFORMATEX, L"")))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))

#define IS_WAVEFORMATEX_IEC61937(x)	(\
	(x) == KSDATAFORMAT_SUBTYPE_IEC61937_DOLBY_DIGITAL ||\
	(x) == KSDATAFORMAT_SUBTYPE_IEC61937_WMA_PRO ||\
	(x) == KSDATAFORMAT_SUBTYPE_IEC61937_DTS ||\
	(x) == KSDATAFORMAT_SUBTYPE_IEC61937_MPEG1 ||\
	(x) == KSDATAFORMAT_SUBTYPE_IEC61937_MPEG2 ||\
	(x) == KSDATAFORMAT_SUBTYPE_IEC61937_MPEG3 ||\
	(x) == KSDATAFORMAT_SUBTYPE_IEC61937_AAC ||\
	(x) == KSDATAFORMAT_SUBTYPE_IEC61937_ATRAC ||\
	(x) == KSDATAFORMAT_SUBTYPE_IEC61937_ONE_BIT_AUDIO ||\
	(x) == KSDATAFORMAT_SUBTYPE_IEC61937_DOLBY_DIGITAL_PLUS ||\
	(x) == KSDATAFORMAT_SUBTYPE_IEC61937_DOLBY_DIGITAL_PLUS_ATMOS ||\
	(x) == KSDATAFORMAT_SUBTYPE_IEC61937_DTS_HD ||\
	(x) == KSDATAFORMAT_SUBTYPE_IEC61937_DOLBY_MLP ||\
	(x) == KSDATAFORMAT_SUBTYPE_IEC61937_DOLBY_MAT20 ||\
	(x) == KSDATAFORMAT_SUBTYPE_IEC61937_DOLBY_MAT21 ||\
	(x) == KSDATAFORMAT_SUBTYPE_IEC61937_DST)

class CMMDeviceUtility
{
public:
	CMMDeviceUtility();
	~CMMDeviceUtility();

	static FLAG_NAME_PAIR Device_state_flag_names[4];
	static FLAG_NAME_PAIR Data_flow_flag_names[3];
	static FLAG_NAME_PAIR Role_flag_names[3];
	static const std::tuple<const PROPERTYKEY, const WCHAR*> prop_key_names[207];
	static FLAG_NAME_PAIR EndpointFormFactor_flag_names[EndpointFormFactor_enum_count];
	static FLAG_NAME_PAIR WaveFormatTag_flag_names[267];
	static FLAG_NAME_PAIR Speaker_position_flag_names[18];

	inline static void GetFlagsDesc(INT nFlags, FLAG_NAME_PAIR* flag_names, size_t flag_count, WCHAR* szDesc, int ccDesc)
	{
		bool bFirst = true;
		int ccWritten = 0;
		memset(szDesc, 0, ccDesc);
		for (size_t i = 0; i < flag_count; i++)
		{
			if (nFlags&(std::get<0>(flag_names[i])))
			{
				ccWritten += _stprintf_s(szDesc + ccWritten, ccDesc - ccWritten, L"%s%s", !bFirst ? L" | " : L"", std::get<1>(flag_names[i]));
				bFirst = false;
			}
		}
	}

	inline static const WCHAR* GetEnumerateName(INT nEnumValue, FLAG_NAME_PAIR* enum_names, size_t enum_count)
	{
		for (size_t i = 0; i < enum_count; i++)
			if (nEnumValue == std::get<0>(enum_names[i]))
				return std::get<1>(enum_names[i]);

		return NULL;
	}

	inline static int GetBufDesc(unsigned char* buf, int buf_size, int nLeadingSpace, WCHAR* szBufDesc, int ccBufDesc)
	{
		int nWritten = 0;
		int nLeftChars = buf_size;
		int nLines = nLeftChars / 16 + (nLeftChars % 16 == 0 ? 0 : 1);
		int nRequiredCC = nLines*nLeadingSpace + buf_size + nLines * 16 * 3 /* "%02X " plus "%c" */ + 4 /* "- " plus "\n" plus "\0" */;

		if (ccBufDesc == 0 || szBufDesc == nullptr)
			return nRequiredCC;
		
		if (ccBufDesc < nRequiredCC)
			return RET_CODE_BUFFER_TOO_SMALL;

		for (int i = 0; i < nLines; i++)
		{
			for (int j = 0; j < nLeadingSpace; j++)
				szBufDesc[nWritten + j] = L' ';

			nWritten += nLeadingSpace;
			for (int j = 0; j < (nLeftChars < 16 ? nLeftChars : 16); j++)
				nWritten += swprintf_s(szBufDesc + nWritten, ccBufDesc - nWritten, L"%02X ", buf[i * 16 + j]);

			for (int j = 0; j < 16 - (nLeftChars < 16 ? nLeftChars : 16); j++)
				nWritten += swprintf_s(szBufDesc + nWritten, ccBufDesc - nWritten, L"   ");

			nWritten += swprintf_s(szBufDesc + nWritten, ccBufDesc - nWritten, L"- ");

			for (int j = 0; j < (nLeftChars < 16 ? nLeftChars : 16); j++)
				nWritten += swprintf_s(szBufDesc + nWritten, ccBufDesc - nWritten, L"%c",
					iswprint(buf[i * 16 + j]) ? buf[i * 16 + j] : L'.');

			nWritten += swprintf_s(szBufDesc + nWritten, ccBufDesc - nWritten, L"\n");

			nLeftChars -= nLeftChars < 16 ? 0 : 16;
		}

		*(szBufDesc + nWritten) = L'\0';
		nWritten++;

		return nWritten;
	}

	inline static int GetWaveFormatExDesc(unsigned char* buf, int buf_size, int nLeadingSpace, WCHAR* szBufDesc, int ccBufDesc, WCHAR* szLeading=L"")
	{
		HRESULT hr = S_OK;
		WCHAR wszTmp[2048];
		WCHAR wszIndent[1024];
		int ccWritten = 0;

		if (nLeadingSpace >= _countof(wszIndent))
		{
			printf("[MMDeviceUtil] nLeadingSpace: %zu should NOT exceed %zu.\n", nLeadingSpace, _countof(wszIndent) - 1);
			return RET_CODE_INVALID_PARAMETER;
		}

		wmemset(wszIndent, L' ', nLeadingSpace);
		wszIndent[nLeadingSpace] = L'\0';

		if (buf_size < sizeof(WAVEFORMATEX))
			return -1;

		WAVEFORMATEX* pFmt = (WAVEFORMATEX*)buf;
		ccWritten  = swprintf_s(wszTmp, _countof(wszTmp), L"%s%s%20s: %s(0X%04X)\n", wszIndent, szLeading, L"wFormatTag",
			GetEnumerateName(pFmt->wFormatTag, WaveFormatTag_flag_names, _countof(WaveFormatTag_flag_names)), pFmt->wFormatTag);
		ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %d\n", wszIndent, szLeading, L"nChannels", pFmt->nChannels);
		ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %d\n", wszIndent, szLeading, L"nSamplesPerSec", pFmt->nSamplesPerSec);
		ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %d\n", wszIndent, szLeading, L"nAvgBytesPerSec", pFmt->nAvgBytesPerSec);
		ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %d\n", wszIndent, szLeading, L"nBlockAlign", pFmt->nBlockAlign);
		ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %d\n", wszIndent, szLeading, L"wBitsPerSample", pFmt->wBitsPerSample);
		ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %d\n", wszIndent, szLeading, L"cbSize", pFmt->cbSize);

		switch (pFmt->wFormatTag)
		{
		case WAVE_FORMAT_EXTENSIBLE:
			if (buf_size >= sizeof(WAVEFORMATEXTENSIBLE))
			{
				WAVEFORMATEXTENSIBLE* pFmtExt = (WAVEFORMATEXTENSIBLE*)buf;
				ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %d\n", wszIndent, szLeading, L"wValidBitsPerSample", pFmtExt->Samples.wValidBitsPerSample);
				if (pFmtExt->dwChannelMask == SPEAKER_ALL)
					ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: any possible permutation of speaker configurations\n", wszIndent, szLeading, L"dwChannelMask");
				else if(pFmtExt->dwChannelMask == SPEAKER_RESERVED)
					ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: Bit mask locations reserved for future use\n", wszIndent, szLeading, L"dwChannelMask");
				else if(KSAUDIO_SPEAKER_NAME(pFmtExt->dwChannelMask) != NULL)
					ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %s\n", wszIndent, szLeading, L"dwChannelMask", KSAUDIO_SPEAKER_NAME(pFmtExt->dwChannelMask));
				else
				{
					WCHAR wszChMask[512];
					GetFlagsDesc(pFmtExt->dwChannelMask, Speaker_position_flag_names, _countof(Speaker_position_flag_names), wszChMask, _countof(wszChMask));
					ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %s(0X%X)\n", wszIndent, szLeading, L"dwChannelMask", wszChMask, pFmtExt->dwChannelMask);
				}
				ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %s\n", wszIndent, szLeading, L"SubFormat", SUBFORMAT_FLAG_NAME(pFmtExt->SubFormat));

				if (buf_size >= sizeof(WAVEFORMATEXTENSIBLE_IEC61937) && IS_WAVEFORMATEX_IEC61937(pFmtExt->SubFormat))
				{
					WAVEFORMATEXTENSIBLE_IEC61937* pFmtIEC = (WAVEFORMATEXTENSIBLE_IEC61937*)buf;
					ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %u\n", wszIndent, szLeading, L"dwEncodedSamplesPerSec", pFmtIEC->dwEncodedSamplesPerSec);
					ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %u\n", wszIndent, szLeading, L"dwEncodedChannelCount", pFmtIEC->dwEncodedChannelCount);
					ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %u\n", wszIndent, szLeading, L"dwAverageBytesPerSec", pFmtIEC->dwAverageBytesPerSec);
				}
			}
			break;
		case WAVE_FORMAT_MPEG:
			if (buf_size >= sizeof(MPEG1WAVEFORMAT))
			{
				MPEG1WAVEFORMAT* pMPEG1Fmt = (MPEG1WAVEFORMAT*)buf;
				ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %u\n", wszIndent, szLeading, L"fwHeadLayer", pMPEG1Fmt->fwHeadLayer);
				ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %u\n", wszIndent, szLeading, L"dwHeadBitrate", pMPEG1Fmt->dwHeadBitrate);
				ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %u\n", wszIndent, szLeading, L"fwHeadMode", pMPEG1Fmt->fwHeadMode);
				ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %u\n", wszIndent, szLeading, L"fwHeadModeExt", pMPEG1Fmt->fwHeadModeExt);
				ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %u\n", wszIndent, szLeading, L"wHeadEmphasis", pMPEG1Fmt->wHeadEmphasis);
				ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %u\n", wszIndent, szLeading, L"fwHeadFlags", pMPEG1Fmt->fwHeadFlags);
				ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %u\n", wszIndent, szLeading, L"dwPTSLow", pMPEG1Fmt->dwPTSLow);
				ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %u\n", wszIndent, szLeading, L"dwPTSHigh", pMPEG1Fmt->dwPTSHigh);
			}
			break;
		case WAVE_FORMAT_MPEGLAYER3:
			if (buf_size >= sizeof(MPEGLAYER3WAVEFORMAT))
			{
				MPEGLAYER3WAVEFORMAT* pMP3Fmt = (MPEGLAYER3WAVEFORMAT*)buf;
				ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %u\n", wszIndent, szLeading, L"wID", pMP3Fmt->wID);
				ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %u\n", wszIndent, szLeading, L"fdwFlags", pMP3Fmt->fdwFlags);
				ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %u\n", wszIndent, szLeading, L"nBlockSize", pMP3Fmt->nBlockSize);
				ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %u\n", wszIndent, szLeading, L"nFramesPerBlock", pMP3Fmt->nFramesPerBlock);
				ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s%s%20s: %u\n", wszIndent, szLeading, L"nCodecDelay", pMP3Fmt->nCodecDelay);
			}
			break;
		}

		if (ccBufDesc > 0 && szBufDesc != nullptr)
		{
			wcsncpy_s(szBufDesc, ccBufDesc, wszTmp, AMP_MIN(ccBufDesc - 1, ccWritten));
		}

		return ccWritten + 1;
	}

	static int GetDeviceTitleName(IMMDevice* pDevice, WCHAR* wszTitleName, int ccTitleName);
	static int GetDeviceFriendlyName(IMMDevice* pDevice, WCHAR* wszDeviceName, int ccDeviceName);

	static int GetDevicePropDesc(IPropertyStore* pPropStore, PROPERTYKEY propkey, WCHAR* szPropDesc, int ccPropDesc, size_t indent = 0);
};

