#pragma once

#include "EASTL/string.h"
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
	NpcNode(eastl::string ttext) : text(ttext), stopDialog(false), dialogFinished(false), letterCount(0) {}
	virtual ~NpcNode() {};

	void Reset();
	DialogueOption* SetCurrentOptionById(int id);

public:
	int id;
	int letterCount;
	bool stopDialog;
	bool dialogFinished;

	int optionsNum = 0;
	eastl::string text; // npc_text
	eastl::list<DialogueOption*> options; // All the possible options
	DialogueOption* currentOption;
};

class Dialogue
{
public:
	Dialogue(int dialogueId);
	virtual ~Dialogue();

	void Draw(int& count, Font* font);
	bool CleanUp();

	void TriggerDialog(int id);
	void DisableDialog(int id);

public:
	int id;
	bool isDialogueActive;
	eastl::string textToPrint;

	NpcNode* currentNode;
	eastl::list<NpcNode*> nodes;

	int talkingFx1;
	int talkingFx2;
	int talkingFx3;
	int talkingFx4;
	bool firstTime;
};