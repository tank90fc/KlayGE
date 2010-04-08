// D3D11Texture.hpp
// KlayGE D3D11纹理类 头文件
// Ver 3.8.0
// 版权所有(C) 龚敏敏, 2009
// Homepage: http://klayge.sourceforge.net
//
// 3.8.0
// 初次建立 (2009.1.30)
//
// 修改记录
/////////////////////////////////////////////////////////////////////////////////

#ifndef _D3D11TEXTURE_HPP
#define _D3D11TEXTURE_HPP

#pragma once

#include <boost/smart_ptr.hpp>

#include <KlayGE/D3D11/D3D11MinGWDefs.hpp>
#include <d3d11.h>
#include <d3dx11.h>

#include <KlayGE/Texture.hpp>
#include <KlayGE/D3D11/D3D11Typedefs.hpp>
#include <KlayGE/D3D11/D3D11RenderView.hpp>

namespace KlayGE
{
	class D3D11Texture : public Texture
	{
	public:
		explicit D3D11Texture(TextureType type, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint);
		virtual ~D3D11Texture();

		std::wstring const & Name() const;

		virtual void CopyToTexture1D(Texture& target, int level,
			uint32_t dst_width, uint32_t dst_xOffset, uint32_t src_width, uint32_t src_xOffset);
		virtual void CopyToTexture2D(Texture& target, int level,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_xOffset, uint32_t dst_yOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_xOffset, uint32_t src_yOffset);
		virtual void CopyToTexture3D(Texture& target, int level,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_depth,
			uint32_t dst_xOffset, uint32_t dst_yOffset, uint32_t dst_zOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_depth,
			uint32_t src_xOffset, uint32_t src_yOffset, uint32_t src_zOffset);
		virtual void CopyToTextureCube(Texture& target, CubeFaces face, int level,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_xOffset, uint32_t dst_yOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_xOffset, uint32_t src_yOffset);

		ID3D11ShaderResourceViewPtr const & D3DShaderResourceView() const
		{
			return d3d_sr_view_;
		}
		ID3D11UnorderedAccessViewPtr const & D3DUnorderedAccessView() const
		{
			return d3d_ua_view_;
		}

	protected:
		void GetD3DFlags(D3D11_USAGE& usage, UINT& bind_flags, UINT& cpu_access_flags, UINT& misc_flags);

	private:
		virtual void Map1D(int level, TextureMapAccess tma,
			uint32_t width, uint32_t x_offset,
			void*& data);
		virtual void Map2D(int level, TextureMapAccess tma,
			uint32_t width, uint32_t height, uint32_t x_offset, uint32_t y_offset,
			void*& data, uint32_t& row_pitch);
		virtual void Map3D(int level, TextureMapAccess tma,
			uint32_t width, uint32_t height, uint32_t depth,
			uint32_t x_offset, uint32_t y_offset, uint32_t z_offset,
			void*& data, uint32_t& row_pitch, uint32_t& slice_pitch);
		virtual void MapCube(CubeFaces face, int level, TextureMapAccess tma,
			uint32_t width, uint32_t height, uint32_t x_offset, uint32_t y_offset,
			void*& data, uint32_t& row_pitch);

		virtual void Unmap1D(int level);
		virtual void Unmap2D(int level);
		virtual void Unmap3D(int level);
		virtual void UnmapCube(CubeFaces face, int level);

	protected:
		ID3D11DevicePtr				d3d_device_;
		ID3D11DeviceContextPtr		d3d_imm_ctx_;
		ID3D11ShaderResourceViewPtr d3d_sr_view_;
		ID3D11UnorderedAccessViewPtr d3d_ua_view_;
	};

	typedef boost::shared_ptr<D3D11Texture> D3D11TexturePtr;


	class D3D11Texture1D : public D3D11Texture
	{
	public:
		D3D11Texture1D(uint32_t width, uint32_t numMipMaps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint, ElementInitData* init_data);

		uint32_t Width(int level) const;
		uint32_t Height(int level) const;
		uint32_t Depth(int level) const;

		void CopyToTexture(Texture& target);
		void CopyToTexture1D(Texture& target, int level,
			uint32_t dst_width, uint32_t dst_xOffset, uint32_t src_width, uint32_t src_xOffset);

		void BuildMipSubLevels();

		ID3D11Texture1DPtr const & D3DTexture() const
		{
			return d3dTexture1D_;
		}

	private:
		void Map1D(int level, TextureMapAccess tma,
			uint32_t x_offset, uint32_t width, void*& data);
		void Unmap1D(int level);

		void UpdateParams();

	private:
		D3D11_TEXTURE1D_DESC desc_;
		ID3D11Texture1DPtr d3dTexture1D_;

		std::vector<uint32_t> widthes_;
	};

	class D3D11Texture2D : public D3D11Texture
	{
	public:
		D3D11Texture2D(uint32_t width, uint32_t height, uint32_t numMipMaps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint, ElementInitData* init_data);

		uint32_t Width(int level) const;
		uint32_t Height(int level) const;
		uint32_t Depth(int level) const;

		void CopyToTexture(Texture& target);
		void CopyToTexture2D(Texture& target, int level,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_xOffset, uint32_t dst_yOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_xOffset, uint32_t src_yOffset);
		void CopyToTextureCube(Texture& target, CubeFaces face, int level,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_xOffset, uint32_t dst_yOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_xOffset, uint32_t src_yOffset);

		void BuildMipSubLevels();

		ID3D11Texture2DPtr const & D3DTexture() const
		{
			return d3dTexture2D_;
		}

	private:
		void Map2D(int level, TextureMapAccess tma,
			uint32_t x_offset, uint32_t y_offset, uint32_t width, uint32_t height,
			void*& data, uint32_t& row_pitch);
		void Unmap2D(int level);

		void UpdateParams();

	private:
		D3D11_TEXTURE2D_DESC desc_;
		ID3D11Texture2DPtr d3dTexture2D_;

		std::vector<uint32_t>	widthes_;
		std::vector<uint32_t>	heights_;
	};

	class D3D11Texture3D : public D3D11Texture
	{
	public:
		D3D11Texture3D(uint32_t width, uint32_t height, uint32_t depth, uint32_t numMipMaps, uint32_t array_size, ElementFormat format, uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint, ElementInitData* init_data);

		uint32_t Width(int level) const;
		uint32_t Height(int level) const;
		uint32_t Depth(int level) const;

		void CopyToTexture(Texture& target);
		void CopyToTexture3D(Texture& target, int level,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_depth,
			uint32_t dst_xOffset, uint32_t dst_yOffset, uint32_t dst_zOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_depth,
			uint32_t src_xOffset, uint32_t src_yOffset, uint32_t src_zOffset);

		void BuildMipSubLevels();

		ID3D11Texture3DPtr const & D3DTexture() const
		{
			return d3dTexture3D_;
		}

	private:
		void Map3D(int level, TextureMapAccess tma,
			uint32_t x_offset, uint32_t y_offset, uint32_t z_offset,
			uint32_t width, uint32_t height, uint32_t depth,
			void*& data, uint32_t& row_pitch, uint32_t& slice_pitch);
		void Unmap3D(int level);

		void UpdateParams();

	private:
		D3D11_TEXTURE3D_DESC desc_;
		ID3D11Texture3DPtr d3dTexture3D_;

		std::vector<uint32_t>	widthes_;
		std::vector<uint32_t>	heights_;
		std::vector<uint32_t>	depthes_;
	};

	class D3D11TextureCube : public D3D11Texture
	{
	public:
		D3D11TextureCube(uint32_t size, uint32_t numMipMaps, uint32_t array_size, ElementFormat format,
			uint32_t sample_count, uint32_t sample_quality, uint32_t access_hint, ElementInitData* init_data);

		uint32_t Width(int level) const;
		uint32_t Height(int level) const;
		uint32_t Depth(int level) const;

		void CopyToTexture(Texture& target);
		void CopyToTextureCube(Texture& target, CubeFaces face, int level,
			uint32_t dst_width, uint32_t dst_height, uint32_t dst_xOffset, uint32_t dst_yOffset,
			uint32_t src_width, uint32_t src_height, uint32_t src_xOffset, uint32_t src_yOffset);

		void BuildMipSubLevels();

		ID3D11TextureCubePtr const & D3DTexture() const
		{
			return d3dTextureCube_;
		}

	private:
		void MapCube(CubeFaces face, int level, TextureMapAccess tma,
			uint32_t x_offset, uint32_t y_offset, uint32_t width, uint32_t height,
			void*& data, uint32_t& row_pitch);
		void UnmapCube(CubeFaces face, int level);

		void UpdateParams();

	private:
		D3D11_TEXTURE2D_DESC desc_;
		ID3D11TextureCubePtr d3dTextureCube_;

		std::vector<uint32_t>	widthes_;
	};
}

#endif			// _D3D11TEXTURE_HPP
