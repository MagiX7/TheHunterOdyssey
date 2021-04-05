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

	NpcNode* LoadNode(int id, pugi::xml_node node);
	NpcNode* GetNodeById(int id);

public:
	Dialogue* current;
	bool printText;
	bool isDialogueActive;

private:
	// Pugi stuff to load dialogues
	pugi::xml_document file;
	pugi::xml_node root;
	
	eastl::list<Dialogue*> dialogues;

	Font* font;
	
	int lastUserInput;
	int letterCount;
};