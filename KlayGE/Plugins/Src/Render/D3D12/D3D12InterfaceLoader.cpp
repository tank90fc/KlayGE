/**
 * @file D3D12InterfaceLoader.cpp
 * @author Minmin Gong
 *
 * @section DESCRIPTION
 *
 * This source file is part of KlayGE
 * For the latest info, see http://www.klayge.org
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * You may alternatively use this source under the terms of
 * the KlayGE Proprietary License (KPL). You can obtained such a license
 * from http://www.klayge.org/licensing/.
 */

#include <KlayGE/KlayGE.hpp>
#include <KFL/COMPtr.hpp>

#include <KlayGE/D3D12/D3D12InterfaceLoader.hpp>

namespace KlayGE
{
    D3D12InterfaceLoader::D3D12InterfaceLoader()
    {
#ifdef KLAYGE_PLATFORM_WINDOWS_DESKTOP
        // Dynamic loading because these dlls can't be loaded on WinXP
        mod_dxgi_ = ::LoadLibraryEx(TEXT("dxgi.dll"), nullptr, 0);
        if (nullptr == mod_dxgi_)
        {
            ::MessageBoxW(nullptr, L"Can't load dxgi.dll", L"Error", MB_OK);
        }
        mod_d3d12_ = ::LoadLibraryEx(TEXT("D3D12.dll"), nullptr, 0);
        if (nullptr == mod_d3d12_)
        {
            ::MessageBoxW(nullptr, L"Can't load d3d12.dll", L"Error", MB_OK);
        }
        mod_d3dcompiler_ = ::LoadLibraryEx(TEXT("d3dcompiler_47.dll"), nullptr, 0);
        if (nullptr == mod_d3dcompiler_)
        {
            ::MessageBoxW(nullptr, L"Can't load d3dcompiler_47.dll", L"Error", MB_OK);
        }

        if (mod_dxgi_ != nullptr)
        {
            DynamicCreateDXGIFactory1_ = reinterpret_cast<CreateDXGIFactory1Func>(::GetProcAddress(mod_dxgi_, "CreateDXGIFactory1"));
        }
        if (mod_d3d12_ != nullptr)
        {
            DynamicD3D12CreateDevice_ = reinterpret_cast<D3D12CreateDeviceFunc>(::GetProcAddress(mod_d3d12_, "D3D12CreateDevice"));
            DynamicD3D12GetDebugInterface_ = reinterpret_cast<D3D12GetDebugInterfaceFunc>(::GetProcAddress(mod_d3d12_, "D3D12GetDebugInterface"));
            DynamicD3D12SerializeRootSignature_ = reinterpret_cast<D3D12SerializeRootSignatureFunc>(::GetProcAddress(mod_d3d12_, "D3D12SerializeRootSignature"));
        }
        if (mod_d3dcompiler_ != nullptr)
        {
            DynamicD3DCompile_ = reinterpret_cast<D3DCompileFunc>(::GetProcAddress(mod_d3dcompiler_, "D3DCompile"));
            DynamicD3DReflect_ = reinterpret_cast<D3DReflectFunc>(::GetProcAddress(mod_d3dcompiler_, "D3DReflect"));
            DynamicD3DStripShader_ = reinterpret_cast<D3DStripShaderFunc>(::GetProcAddress(mod_d3dcompiler_, "D3DStripShader"));
        }
#else
        DynamicCreateDXGIFactory1_ = ::CreateDXGIFactory1;
        DynamicD3D12CreateDevice_ = ::D3D12CreateDevice;
        DynamicD3D12GetDebugInterface_ = ::D3D12GetDebugInterface;
        DynamicD3D12SerializeRootSignature_ = ::D3D12SerializeRootSignature;
#endif
    }

    D3D12InterfaceLoader::~D3D12InterfaceLoader()
    {
#ifdef KLAYGE_PLATFORM_WINDOWS_DESKTOP
        ::FreeLibrary(mod_d3dcompiler_);
        ::FreeLibrary(mod_d3d12_);
        ::FreeLibrary(mod_dxgi_);
#endif
    }

    D3D12InterfaceLoader& D3D12InterfaceLoader::Instance()
    {
        static D3D12InterfaceLoader ret;
        return ret;
    }

    HRESULT D3D12InterfaceLoader::CreateDXGIFactory1(REFIID riid, void** ppFactory) const
    {
        return DynamicCreateDXGIFactory1_(riid, ppFactory);
    }

    HRESULT D3D12InterfaceLoader::D3D12CreateDevice(IUnknown* pAdapter,
        D3D_FEATURE_LEVEL MinimumFeatureLevel, REFIID riid,
        void** ppDevice)
    {
        return DynamicD3D12CreateDevice_(pAdapter, MinimumFeatureLevel, riid, ppDevice);
    }

    HRESULT D3D12InterfaceLoader::D3D12GetDebugInterface(REFIID riid, void** ppvDebug) const
    {
        return DynamicD3D12GetDebugInterface_(riid, ppvDebug);
    }

    HRESULT D3D12InterfaceLoader::D3D12SerializeRootSignature(D3D12_ROOT_SIGNATURE_DESC const * pRootSignature,
        D3D_ROOT_SIGNATURE_VERSION Version, ID3DBlob** ppBlob, ID3DBlob** ppErrorBlob) const
    {
        return DynamicD3D12SerializeRootSignature_(pRootSignature, Version, ppBlob, ppErrorBlob);
    }

#ifdef KLAYGE_PLATFORM_WINDOWS_DESKTOP
    HRESULT D3D12InterfaceLoader::D3DCompile(LPCVOID pSrcData, SIZE_T SrcDataSize, LPCSTR pSourceName,
            D3D_SHADER_MACRO const * pDefines, ID3DInclude* pInclude, LPCSTR pEntrypoint,
            LPCSTR pTarget, UINT Flags1, UINT Flags2, ID3DBlob** ppCode, ID3DBlob** ppErrorMsgs) const
    {
        return DynamicD3DCompile_(pSrcData, SrcDataSize, pSourceName, pDefines, pInclude, pEntrypoint,
            pTarget, Flags1, Flags2, ppCode, ppErrorMsgs);
    }

    HRESULT D3D12InterfaceLoader::D3DReflect(LPCVOID pSrcData, SIZE_T SrcDataSize, REFIID pInterface,
            void** ppReflector) const
    {
        return DynamicD3DReflect_(pSrcData, SrcDataSize, pInterface, ppReflector);
    }

    HRESULT D3D12InterfaceLoader::D3DStripShader(LPCVOID pShaderBytecode, SIZE_T BytecodeLength,
            UINT uStripFlags, ID3DBlob** ppStrippedBlob) const
    {
        return DynamicD3DStripShader_(pShaderBytecode, BytecodeLength, uStripFlags, ppStrippedBlob);
    }
#endif
}