#include "stdafx.h"
#include <unordered_map>

namespace {
	std::unordered_map<std::string, int> modleHandleTable;
	std::unordered_map<std::string, int> GraphHandleTable;
	std::unordered_map<std::string, int> EffectHandleTable;
}

namespace resource {

//---------------------------------------------
//　モデルデータを読み込み 
//---------------------------------------------
int LoadModel(const char* name)
{
	// すでに読み込んだこのあるデータだったら
	if (modleHandleTable[name] != 0) {
		// そのままそのデータを渡す
		return modleHandleTable[name];
	}
	// 違うかったら
	// 新規データを読み込み
	int	handle = ::MV1LoadModel(name);
	// データを保存
	modleHandleTable[name] = handle;

	return handle;
}
//---------------------------------------------
// グラフィックデータを読み込み
//---------------------------------------------
int LoadGraph(const char* name)
{
	// すでに読み込んだこのあるデータだったら
	if (GraphHandleTable[name] != 0) {
		// そのままそのデータを渡す
		return GraphHandleTable[name];
	}
	// 違うかったら
	// 新規データを読み込み
	int	handle = ::LoadGraph(name);
	// データを保存
	GraphHandleTable[name] = handle;

	return handle;
}
//---------------------------------------------
// エフェクトデータを読み込み
//---------------------------------------------
int LoadEffekseerEffect(const char * name)
{
	// すでに読み込んだこのあるデータだったら
	if (EffectHandleTable[name] != 0) {
		// そのままそのデータを渡す
		return EffectHandleTable[name];
	}
	// 違うかったら
	// 新規データを読み込み
	int	handle = ::LoadEffekseerEffect(name);
	// データを保存
	EffectHandleTable[name] = handle;

	return handle;
}

//---------------------------------------------
// unordered_mapで保存したデータをすべて消去
//---------------------------------------------
void DeleteHandleTable() {
	for (auto &handle: modleHandleTable) {
		::MV1DeleteModel(handle.second);
	}
	modleHandleTable.clear();
	for (auto &handle : GraphHandleTable) {
		::DeleteGraph(handle.second);
	}
	GraphHandleTable.clear();
	for (auto &handle : EffectHandleTable) {
		::DeleteEffekseerEffect(handle.second);
	}
	EffectHandleTable.clear();
}

}
