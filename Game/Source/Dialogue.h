#pragma once

#include "EASTL/string.h"
#include "EASTL/array.h"
#include "EASTL/list.h"

#include "SDL/include/SDL_rect.h"

class NpcNode;

class DialogueOption
{
public:
	DialogueOption();
	virtual ~DialogueOption();

public:
	eastl::string text;
	NpcNode* nextNode;

	int id;
	int nextNodeId;

	SDL_Rect bounds;
};

class NpcNode
{
public:
	NpcNode(eastl::string ttext) : text(ttext) {}
	virtual ~NpcNode() {};

public:
	int id;
	int letterCount;
	bool stopDialog;

	eastl::string text; // npc_text
	eastl::list<DialogueOption*> options; // All the possible options
	DialogueOption* currentOption;

};

class Dialogue
{
public:
	Dialogue(int dialogueId);
	virtual ~Dialogue();

	void Draw(int& count);
	bool CleanUp();

	void TriggerDialog(int id);
	void DisableDialog(int id);

public:
	int id;
	NpcNode* currentNode;
	eastl::list<NpcNode*> nodes;
	bool isDialogueActive;
};