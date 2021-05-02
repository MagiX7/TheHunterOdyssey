#pragma once

#include "Armor.h"

class KnightHelmet : public Armor
{
public:
	KnightHelmet(SDL_Rect b, SDL_Texture* atlas) : Armor(b, atlas){}
	virtual ~KnightHelmet() {}
	
	bool Load() override;
	void Draw(bool showColliders) override;
};