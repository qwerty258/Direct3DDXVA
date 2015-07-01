#include <iostream>
using namespace std;
#define INITGUID
#include <d3d9.h>
#include <dxva2api.h>

int main(int argc, char* argv[])
{
    system("pause");

    IDirect3D9* p_IDirect3D9 = Direct3DCreate9(D3D_SDK_VERSION);
    if(NULL == p_IDirect3D9)
    {
        cout << "Direct3DCreate9 error" << endl;
        exit(0);
    }

    UINT adapter_count = p_IDirect3D9->GetAdapterCount();

    cout << "adapter count: " << adapter_count << endl;

    system("pause");

    cout << "get adapter indentifier:" << endl;

    D3DADAPTER_IDENTIFIER9 d3d_adapter_identifier;

    for(UINT i = 0; i < adapter_count; i++)
    {
        cout << i << ":" << endl;
        if(D3D_OK != p_IDirect3D9->GetAdapterIdentifier(i, 0, &d3d_adapter_identifier))
        {
            cout << i << ":p_IDirect3D9->GetAdapterIdentifier error" << endl;
        }
        printf("Description: %s\n", d3d_adapter_identifier.Description);
        printf("DeviceId: 0x%08X\n", d3d_adapter_identifier.DeviceId);
        printf("DeviceIdentifier: %08X-%04X-%04X-%016X\n", d3d_adapter_identifier.DeviceIdentifier.Data1, d3d_adapter_identifier.DeviceIdentifier.Data2, d3d_adapter_identifier.DeviceIdentifier.Data3, *((__int64*)d3d_adapter_identifier.DeviceIdentifier.Data4));
        cout << "DeviceName: " << d3d_adapter_identifier.DeviceName << endl;
        cout << "Driver: " << d3d_adapter_identifier.Driver << endl;
        printf("DriverVersion: 0x%016X\n", d3d_adapter_identifier.DriverVersion);
        printf("Revision: 0x%08X\n", d3d_adapter_identifier.Revision);
        printf("SubSysId: 0x%08X\n", d3d_adapter_identifier.SubSysId);
        printf("VendorId: 0x%08X\n", d3d_adapter_identifier.VendorId);
        printf("WHQLLevel: 0x%08X\n", d3d_adapter_identifier.WHQLLevel);
        cout << endl;
    }

    cout << "get adapter display mode:" << endl;

    D3DDISPLAYMODE d3d_display_mode;

    for(UINT i = 0; i < adapter_count; i++)
    {
        cout << i << ":" << endl;
        if(D3D_OK != p_IDirect3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3d_display_mode))
        {
            cout << i << ":p_IDirect3D9->GetAdapterDisplayMode error" << endl;
        }
        cout << "Format: " << d3d_display_mode.Format << endl;
        cout << "Height: " << d3d_display_mode.Height << endl;
        cout << "RefreshRate: " << d3d_display_mode.RefreshRate << endl;
        cout << "Width: " << d3d_display_mode.Width << endl;
        cout << endl;
    }

    D3DPRESENT_PARAMETERS d3d_present_parameters;
    memset(&d3d_present_parameters, 0x0, sizeof(D3DPRESENT_PARAMETERS));
    //d3d_present_parameters.Flags = D3DPRESENTFLAG_VIDEO;
    d3d_present_parameters.BackBufferWidth = GetSystemMetrics(SM_CXSCREEN);
    d3d_present_parameters.BackBufferHeight = GetSystemMetrics(SM_CYSCREEN);
    //d3d_present_parameters.BackBufferCount = 0;
    d3d_present_parameters.BackBufferFormat = D3DFMT_UNKNOWN;
    d3d_present_parameters.Windowed = TRUE;
    //d3d_present_parameters.hDeviceWindow = GetDesktopWindow();
    d3d_present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    //d3d_present_parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
    //d3d_present_parameters.EnableAutoDepthStencil = FALSE;
    //d3d_present_parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    IDirect3DDevice9* p_IDirect3DDevice9;


    if(D3D_OK != p_IDirect3D9->CreateDevice(0, D3DDEVTYPE_HAL, GetDesktopWindow(), D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED | D3DCREATE_DISABLE_DRIVER_MANAGEMENT_EX, &d3d_present_parameters, &p_IDirect3DDevice9))
    {
        cout << "CreateDevice Error: " << endl;
        system("pause");
        exit(0);
    }

    //IDirect3DQuery9* p_IDirect3DQuery9;
    //long hResult = D3DERR_NOTAVAILABLE;
    //hResult = p_IDirect3DDevice9->CreateQuery(D3DQUERYTYPE_RESOURCEMANAGER, &p_IDirect3DQuery9);
    //if(D3D_OK != hResult)
    //{
    //    cout << "p_IDirect3DDevice9->CreateQuery error" << endl;
    //    system("pause");
    //    exit(0);
    //}

    //DWORD data_size = p_IDirect3DQuery9->GetDataSize();
    //char* query_buffer = new char[data_size];

    //p_IDirect3DQuery9->GetData(query_buffer, data_size, D3DGETDATA_FLUSH);

    //cout << query_buffer << endl;
    //for(size_t i = 0; i < 10000; i++)
    //{
    //    p_IDirect3DDevice9->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
    //    p_IDirect3DDevice9->BeginScene();
    //    p_IDirect3DDevice9->EndScene();
    //    p_IDirect3DDevice9->Present(NULL, NULL, NULL, NULL);
    //}

    UINT token;
    IDirect3DDeviceManager9* p_IDirect3DDeviceManager9;

    if(S_OK != DXVA2CreateDirect3DDeviceManager9(&token, &p_IDirect3DDeviceManager9))
    {
        cout << "DXVA2CreateDirect3DDeviceManager9 Error: " << endl;
        system("pause");
        exit(0);
    }

    if(S_OK != p_IDirect3DDeviceManager9->ResetDevice(p_IDirect3DDevice9, token))
    {
        cout << "p_IDirect3DDeviceManager9->ResetDevice error" << endl;
        system("pause");
        exit(0);
    }

    IDirectXVideoDecoderService* p_IDirectXVideoDecoderService;

    if(D3D_OK != DXVA2CreateVideoService(p_IDirect3DDevice9, IID_IDirectXVideoDecoderService, (void**)&p_IDirectXVideoDecoderService))
    {
        cout << "DXVA2CreateVideoService Error: " << endl;
        system("pause");
        exit(0);
    }

    UINT count;
    GUID* pGuids;

    if(D3D_OK != p_IDirectXVideoDecoderService->GetDecoderDeviceGuids(&count, &pGuids))
    {
        cout << "GetDecoderDeviceGuids Error: " << endl;
        system("pause");
        exit(0);
    }

    cout << "GUIDS: " << count << endl;

    for(UINT i = 0; i < count; i++)
    {
        if(IsEqualGUID(pGuids[i], DXVA2_ModeH264_MoComp_NoFGT))
        {
            cout << "DXVA2_ModeH264_MoComp_NoFGT supported" << endl;
        }
        if(IsEqualGUID(pGuids[i], DXVA2_ModeH264_MoComp_FGT))
        {
            cout << "DXVA2_ModeH264_MoComp_FGT supported" << endl;
        }
        if(IsEqualGUID(pGuids[i], DXVA2_ModeH264_IDCT_NoFGT))
        {
            cout << "DXVA2_ModeH264_IDCT_NoFGT supported" << endl;
        }
        if(IsEqualGUID(pGuids[i], DXVA2_ModeH264_IDCT_FGT))
        {
            cout << "DXVA2_ModeH264_IDCT_FGT supported" << endl;
        }
        if(IsEqualGUID(pGuids[i], DXVA2_ModeH264_VLD_NoFGT))
        {
            cout << "DXVA2_ModeH264_VLD_NoFGT supported" << endl;
        }
        if(IsEqualGUID(pGuids[i], DXVA2_ModeH264_VLD_FGT))
        {
            cout << "DXVA2_ModeH264_VLD_FGT supported" << endl;
        }
    }
    cout << "-------------------------------" << endl;

    D3DFORMAT* p_D3DFORMAT;

    if(S_OK != p_IDirectXVideoDecoderService->GetDecoderRenderTargets(DXVA2_ModeH264_VLD_NoFGT, &count, &p_D3DFORMAT))
    {
        cout << "p_IDirectXVideoDecoderService->GetDecoderRenderTargets Error: " << endl;
        system("pause");
        exit(0);
    }

    cout << "D3DFORMAT:\n";
    for(size_t i = 0; i < count; i++)
    {
        cout << p_D3DFORMAT[i] << endl;
    }
    cout << "-------------------------------" << endl;

    /* Allocates all surfaces needed for the decoder */
    IDirect3DSurface9* array_IDirect3DSurface9[3];

    if(S_OK != p_IDirectXVideoDecoderService->CreateSurface(1920, 1080, 2, p_D3DFORMAT[0], D3DPOOL_DEFAULT, 0, DXVA2_VideoDecoderRenderTarget, array_IDirect3DSurface9, NULL))
    {
        cout << "p_IDirectXVideoDecoderService->CreateSurface error" << endl;
        exit(0);
    }

    DXVA2_VideoDesc dxva2_video_desc;

    memset(&dxva2_video_desc, 0x0, sizeof(DXVA2_VideoDesc));
    dxva2_video_desc.SampleWidth = 1920;
    dxva2_video_desc.SampleHeight = 1080;
    dxva2_video_desc.InputSampleFreq.Denominator = 0;
    dxva2_video_desc.InputSampleFreq.Numerator = 0;
    dxva2_video_desc.OutputFrameFreq = dxva2_video_desc.InputSampleFreq;
    dxva2_video_desc.SampleFormat.NominalRange = DXVA2_NominalRange_Unknown;
    dxva2_video_desc.SampleFormat.SampleFormat = DXVA2_SampleUnknown;
    dxva2_video_desc.SampleFormat.VideoChromaSubsampling = DXVA2_VideoChromaSubsampling_Unknown;
    dxva2_video_desc.SampleFormat.VideoLighting = DXVA2_VideoLighting_Unknown;
    dxva2_video_desc.SampleFormat.VideoPrimaries = DXVA2_VideoPrimaries_Unknown;
    dxva2_video_desc.SampleFormat.VideoTransferFunction = DXVA2_VideoTransFunc_Unknown;
    dxva2_video_desc.SampleFormat.VideoTransferMatrix = DXVA2_VideoTransferMatrix_Unknown;
    dxva2_video_desc.UABProtectionLevel = FALSE;
    dxva2_video_desc.Reserved = 0;
    dxva2_video_desc.Format = D3DFMT_X8R8G8B8;

    /* List all configurations available for the decoder */
    DXVA2_ConfigPictureDecode* p_DXVA2_ConfigPictureDecode;

    if(S_OK != p_IDirectXVideoDecoderService->GetDecoderConfigurations(DXVA2_ModeH264_VLD_NoFGT, &dxva2_video_desc, NULL, &count, &p_DXVA2_ConfigPictureDecode))
    {
        cout << "p_IDirectXVideoDecoderService->GetDecoderConfigurations error" << endl;
        exit(0);
    }

    /* Select the best decoder configuration */
    DXVA2_ConfigPictureDecode* p_best_DXVA2_ConfigPictureDecode = NULL;
    int best_score = 0;
    int score;
    for(UINT i = 0; i < count; i++)
    {
        cout << "configuration[" << i << "] ConfigBitstreamRaw " << p_DXVA2_ConfigPictureDecode[i].ConfigBitstreamRaw << endl;
        if(1 == p_DXVA2_ConfigPictureDecode[i].ConfigBitstreamRaw)
        {
            score = 1;
        }
        else if(2 == p_DXVA2_ConfigPictureDecode[i].ConfigBitstreamRaw)
        {
            score = 2;
        }
        else
        {
            continue;
        }
        if(IsEqualGUID(p_DXVA2_ConfigPictureDecode[i].guidConfigBitstreamEncryption, DXVA2_NoEncrypt))
        {
            score += 16;
        }
        if(best_score < score)
        {
            p_best_DXVA2_ConfigPictureDecode = &p_DXVA2_ConfigPictureDecode[i];
            best_score = score;
        }
    }

    IDirectXVideoDecoder* p_IDirectXVideoDecoder;

    if(S_OK != p_IDirectXVideoDecoderService->CreateVideoDecoder(DXVA2_ModeH264_VLD_NoFGT, &dxva2_video_desc, p_best_DXVA2_ConfigPictureDecode, array_IDirect3DSurface9, 3, &p_IDirectXVideoDecoder))
    {
        cout << "p_IDirectXVideoDecoderService->CreateVideoDecoder error" << endl;
        exit(0);
    }

    char* buffer = new char[1024];
    memset(buffer, 0x0, 1024);

    //for(size_t i = 0; i < 10000; i++)
    //{
    //    p_IDirectXVideoDecoder->BeginFrame(array_IDirect3DSurface9[1], NULL);
    //    p_IDirectXVideoDecoder->GetBuffer(DXVA2_BitStreamDateBufferType, (void**)&buffer, 0);
    //    p_IDirectXVideoDecoder->ReleaseBuffer(DXVA2_BitStreamDateBufferType);
    //    p_IDirectXVideoDecoder->Execute();
    //    p_IDirectXVideoDecoder->EndFrame();
    //}

    cout << "release interfaces:" << endl;

    CoTaskMemFree(p_DXVA2_ConfigPictureDecode);
    CoTaskMemFree(pGuids);
    CoTaskMemFree(p_D3DFORMAT);

    ULONG release_count;

    cout << "release p_IDirectXVideoDecoder" << endl;
    system("pause");
    do
    {
        release_count = p_IDirectXVideoDecoder->Release();
        cout << release_count << endl;
    } while(release_count > 0);

    cout << "release p_IDirectXVideoDecoderService" << endl;
    system("pause");
    do
    {
        release_count = p_IDirectXVideoDecoderService->Release();
        cout << release_count << endl;
    } while(release_count > 0);

    cout << "release p_IDirect3DDeviceManager9" << endl;
    system("pause");
    do
    {
        release_count = p_IDirect3DDeviceManager9->Release();
        cout << release_count << endl;
    } while(release_count > 0);

    cout << "release p_IDirect3DDevice9" << endl;
    system("pause");
    do
    {
        release_count = p_IDirect3DDevice9->Release();
        cout << release_count << endl;
    } while(release_count > 0);

    cout << "release p_IDirect3D9" << endl;
    system("pause");
    do
    {
        release_count = p_IDirect3D9->Release();
        cout << release_count << endl;
    } while(release_count > 0);

    //delete[] query_buffer;

    system("pause");

    return 0;
}

