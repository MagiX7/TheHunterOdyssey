#pragma once

#include "Dialogue.h"
#include "Font.h"

#include "PugiXml/src/pugixml.hpp"

class DialogueManager
{
public:
	DialogueManager();
	virtual ~DialogueManager();
	
	bool Start();
	bool Update(float dt);
	void Draw();

	Dialogue* LoadDialogue(int id); // Load the "npc" node

	NpcNode* LoadNode(int id);
	NpcNode* GetNodeById(int id);
	Dialogue* current;
	bool printText;
private:
	pugi::xml_document file;
	pugi::xml_node root;

	
	eastl::list<Dialogue*> dialogues;

	Font* font;
	
	int lastUserInput;
	int letterCount;

	

};