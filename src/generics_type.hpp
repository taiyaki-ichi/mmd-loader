#pragma once

namespace mmdl
{
	// �T�C�Y�w��\�ȃR���e�i
	template<typename T, typename U>
	concept resizable_container = requires(T t, U u)
	{
		t.resize(u);
		t[u];
	};
}