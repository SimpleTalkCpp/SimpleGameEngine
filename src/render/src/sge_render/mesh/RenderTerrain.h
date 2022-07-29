#pragma once

#include "RenderMesh.h"
#include "../command/RenderRequest.h"

namespace sge {

#define RenderTerrain3_ZoneMask_ENUM_LIST(E) \
	E(None, = 0) \
	E(North, = 1 << 0)  \
	E(East,  = 1 << 1)  \
	E(South, = 1 << 2)  \
	E(West,  = 1 << 3)  \
//----
SGE_ENUM_CLASS(RenderTerrain3_ZoneMask, u8);
SGE_ENUM_ALL_OPERATOR(RenderTerrain3_ZoneMask)

class RenderTerrain : public NonCopyable {
	using This = RenderTerrain;

public:
	using Terrain			= This;
	using ZoneMask			= RenderTerrain3_ZoneMask;
	using Vertex			= Vertex_Pos2f;
	using VertexIndex		= u16;

	void createFromHeightMapFile(const Vec3f& terrainPos, const Vec2f& terrainSize, float terrainHeight, int maxLod, StrView heightMapFilename);
	void createFromHeightMap	(const Vec3f& terrainPos, const Vec2f& terrainSize, float terrainHeight, int maxLod, const Image& heightMap);
	void destroy();

	void render(RenderRequest& req);

	class PatchIndices {
	public:
		void create(Terrain* terrain, int level, ZoneMask zoneMask);
		RenderGpuBuffer* indexBuffer() const { return constCast(_indexBuffer.ptr()); }
		size_t indexCount() const { return _indexCount; }
		RenderDataType indexType() const { return RenderDataTypeUtil::get<VertexIndex>(); }

	private:
		void _addToIndices(Vector<VertexIndex>& indices, Span<Vec2i> sector, int verticesPerRow, Vec2i direction, bool flipXY);

		SPtr<RenderGpuBuffer> _indexBuffer;
		size_t _indexCount = 0;
	};

	class PatchLevelIndices {
	public:
		static const int s_patchMeshCount = 16;

		void create(Terrain* terrain, int level);

		PatchIndices* patchIndices(ZoneMask zoneMask) {
			return &_patchIndices[enumInt(zoneMask)];
		}

	private:
		Terrain*	_terrain = nullptr;
		int			_level = 0;
		Vector<PatchIndices, s_patchMeshCount>	_patchIndices;
	};

	class Patch {
	public:
		void create(Terrain* terrain, const Vec2i& index, Shader* shader);
		int displayLevel() const { return _displayLevel; }

		void render(RenderRequest& req);

		Vec3f worldCenterPos();
		void setDisplayLevelByViewPos(const Vec3f& viewPos);

	private:
		inline	bool _adjacentPatchHasHigherLod(int x, int y) const;

		int					_displayLevel = 0;
		Vec2i				_index {0, 0};
		RenderTerrain*		_terrain = nullptr;
		SPtr<Material>		_material;
	};

			int		patchCellsPerRow	() const { return 1 << (_maxLod - 1); }
			int		patchVerticesPerRow	() const { return patchCellsPerRow() + 1; }
			int		maxLod				() const { return _maxLod; }

	const	Vec3f&	terrainPos			() const { return _terrainPos; }
	const	Vec2f&	terrainSize			() const { return _terrainSize; }
			float	terrainHeight		() const { return _terrainHeight; }

			Vec2i	patchCount			() const { return _patchCount; }
			Vec2f	patchSize			() const { return _terrainSize / Vec2f::s_cast(_patchCount); }
			Vec2f	cellSize			() const { return _terrainSize / Vec2f::s_cast(_heightmapResolution - 1); }

			PatchIndices*	patchIndices(int level, ZoneMask zoneMask);
			Span<PatchLevelIndices>	patchLevelIndices() { return _patchLevelIndices; }

			Patch* patch(int x, int y);

	const VertexLayout*		vertexLayout() const	{ return _vertexLayout; }
	size_t					vertexCount() const		{ return _vertexCount; }

	RenderGpuBuffer* vertexBuffer() { return _vertexBuffer; }
	Texture2D*		heightMapTexture() { return _heightMapTexture; }

private:
	Vec3f	_terrainPos  {0, 0, 0};
	Vec2f	_terrainSize {0, 0};
	float	_terrainHeight = 0;
	Vec2i	_heightmapResolution {0, 0};
	int		_maxLod = 1;
	Vec2i	_patchCount {0, 0};
	Vector<Patch>			_patches;
	Vector<PatchLevelIndices>	_patchLevelIndices;

	const VertexLayout*		_vertexLayout = nullptr;
	size_t					_vertexCount = 0;
	SPtr<RenderGpuBuffer>	_vertexBuffer;

	SPtr<Texture2D>			_heightMapTexture;
};

SGE_INLINE RenderTerrain::PatchIndices* RenderTerrain::patchIndices(int level, ZoneMask zoneMask) {
	if (level < 0 || level >= _patchLevelIndices.size()) {
		SGE_ASSERT(false);
		return nullptr;
	}

	auto& lv = _patchLevelIndices[level];
	return lv.patchIndices(zoneMask);
}

SGE_INLINE RenderTerrain::Patch* RenderTerrain::patch(int x, int y) {
	if (x < 0 || y < 0 || x >= _patchCount.x || y >= _patchCount.y)
		return nullptr;
	return &_patches[y * _patchCount.x + x];
}

SGE_INLINE Vec3f RenderTerrain::Patch::worldCenterPos() {
	auto s = _terrain->patchSize();
	auto pos = (Vec2f::s_cast(_index) + 0.5f) * s;
	auto o = _terrain->terrainPos() + Vec3f(pos.x, 0, pos.y);
	return o;
}

}