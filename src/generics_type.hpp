#pragma once

namespace mmdl
{
	// サイズ指定可能なコンテナ
	template<typename T, typename U>
	concept resizable_container = requires(T t, U u)
	{
		t.resize(u);
		t[u];
	};
}