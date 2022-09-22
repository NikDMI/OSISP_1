#pragma once
#include "../Sprite.h"
#include <string>

namespace LAB1 {
	class ImageSprite final : public Sprite {
	private:
		BITMAP_HANDLE m_bitmap;
	public:
		ImageSprite(Painter* painter, RECT rect, std::wstring fileName);
		void Draw() override;
	};
}
