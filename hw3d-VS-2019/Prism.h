#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include "ChiliMath.h"

class Prism
{
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated(int longDiv)
	{
		namespace dx = DirectX;
		assert(longDiv >= 3);

		const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const auto offset = dx::XMVectorSet(0.0f, 0.0f, 2.0f, 0.0f);
		const float longitudeAngle = 2.0f * PI / longDiv;

		// Near center
		std::vector<V> vertices;
		vertices.emplace_back();
		vertices.back().pos = { 0.0f, 0.0f, -1.0f };
		const auto iCenterNear = (unsigned short)(vertices.size() - 1);

		// Far center
		vertices.emplace_back();
		vertices.back().pos = { 0.0f, 0.0f, 1.0f };
		const auto iCenterFar = (unsigned short)(vertices.size() - 1);

		// Base vertices
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			// Near base
			{
				vertices.emplace_back();
				auto v = dx::XMVector3Transform(
					base
					, dx::XMMatrixRotationZ(longitudeAngle * iLong)
				);
				dx::XMStoreFloat3(&vertices.back().pos, v);
			}

			// Far base
			{
				vertices.emplace_back();
				auto v = dx::XMVector3Transform(
					base
					, dx::XMMatrixRotationZ(longitudeAngle * iLong)
				);
				v = dx::XMVectorAdd(v, offset);
				dx::XMStoreFloat3(&vertices.back().pos, v);
			}
		}

		// Side indices
		std::vector< unsigned short> indices;
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			const auto i = iLong * 2;
			const auto mod = longDiv * 2;
			indices.push_back(i + 2);
			indices.push_back((i + 2) % mod + 2);
			indices.push_back(i + 1 + 2);
			indices.push_back((i + 2) % mod + 2);
			indices.push_back((i + 3) % mod + 2);
			indices.push_back(i + 1 + 2);
		}

		// Base indices
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			const auto i = iLong * 2;
			const auto mod = longDiv * 2;
			indices.push_back(i + 2);
			indices.push_back(iCenterNear);
			indices.push_back((i + 2) % mod + 2);
			indices.push_back(iCenterFar);
			indices.push_back(i + 1 + 2);
			indices.push_back((i + 3) % mod + 2);
		}

		return { std::move(vertices), std::move(indices) };
	}
	template<class V>
	static IndexedTriangleList<V> MakeTesselatedIndependentCapNormals(int longDiv)
	{
		namespace dx = DirectX;
		assert(longDiv >= 3);

		const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const auto offset = dx::XMVectorSet(0.0f, 0.0f, 2.0f, 0.0f);
		const float longitudeAngle = 2.0f * PI / longDiv;

		std::vector<V> vertices;

		// Near center
		const auto iCenterNear = (unsigned short)vertices.size();
		vertices.emplace_back();
		vertices.back().pos = { 0.0f, 0.0f, -1.0f };
		vertices.back().n = { 0.0f, 0.0f, -1.0f };
		// near base vertices
		const auto iBaseNear = (unsigned short)vertices.size();
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				base
				, dx::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			dx::XMStoreFloat3(&vertices.back().pos, v);
			vertices.back().n = { 0.0f, 0.0f, -1.0f };
		}
		// Far center
		const auto iCenterFar = (unsigned short)vertices.size();
		vertices.emplace_back();
		vertices.back().pos = { 0.0f, 0.0f, 1.0f };
		vertices.back().n = { 0.0f, 0.0f, 1.0f };
		// Far base vertices
		const auto iBaseFar = (unsigned short)vertices.size();
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(
				base
				, dx::XMMatrixRotationZ(longitudeAngle * iLong)
			);
			v = dx::XMVectorAdd(v, offset);
			dx::XMStoreFloat3(&vertices.back().pos, v);
			vertices.back().n = { 0.0f, 0.0f, 1.0f };
		}
		// Fusilage vertices

		const auto iFusilage = (unsigned short)vertices.size();
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			// Near base
			{
				vertices.emplace_back();
				auto v = dx::XMVector3Transform(
					base
					, dx::XMMatrixRotationZ(longitudeAngle * iLong)
				);
				dx::XMStoreFloat3(&vertices.back().pos, v);
				vertices.back().n = { vertices.back().pos.x, vertices.back().pos.y, 0.0f };
			}
			// Far base
			{
				vertices.emplace_back();
				auto v = dx::XMVector3Transform(
					base
					, dx::XMMatrixRotationZ(longitudeAngle * iLong)
				);
				v = dx::XMVectorAdd(v, offset);
				dx::XMStoreFloat3(&vertices.back().pos, v);
				vertices.back().n = { vertices.back().pos.x, vertices.back().pos.y, 0.0f };
			}
		}

		std::vector<unsigned short> indices;

		// Near base indices
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			const auto i = iLong;
			const auto mod = longDiv;
			// Near
			indices.push_back(i + iBaseNear);
			indices.push_back(iCenterNear);
			indices.push_back((i + 1) % mod + iBaseNear);
		}
		// Far base indices
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			const auto i = iLong;
			const auto mod = longDiv;
			// Far
			indices.push_back(iCenterFar);
			indices.push_back(i + iBaseFar);
			indices.push_back((i + 1) % mod + iBaseFar);
		}
		// Fusilage indices
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			const auto i = iLong * 2;
			const auto mod = longDiv * 2;
			indices.push_back(i + iFusilage);
			indices.push_back((i + 2) % mod + iFusilage);
			indices.push_back(i + 1 + iFusilage);
			indices.push_back((i + 2) % mod + iFusilage);
			indices.push_back((i + 3) % mod + iFusilage);
			indices.push_back(i + 1 + iFusilage);
		}

		return { std::move(vertices), std::move(indices) };
	}
	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>(24);
	}
};