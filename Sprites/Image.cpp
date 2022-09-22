#include "ImageSprite.h"

namespace LAB1 {

	ImageSprite::ImageSprite(Painter* painter, RECT rect, std::wstring fileName) :Sprite{painter, rect} {
		m_Painter->LoadImageFromFile(fileName);
	}

	void ImageSprite::Draw() {
		m_Painter->DrawImage(m_bitmap, m_spriteRect);
	}

}