#pragma once

#include "EASTL/string.h"
#include "EASTL/array.h"
#include "EASTL/list.h"

class DialogueOption
{
public:
	DialogueOption();
	virtual ~DialogueOption();

public:
	eastl::string text;
	int id;
	int nextId;
};

class NpcNode
{
public:
	NpcNode(eastl::string ttext) : text(ttext) {}
	virtual ~NpcNode() {};

public:
	eastl::string text; // npc_text
	eastl::list<DialogueOption*> options; // All the possible options

};

class Dialogue
{
public:
	Dialogue(int dialogueId);
	virtual ~Dialogue();

	void Draw();
	bool CleanUp();

	void TriggerDialog(int id);
	void DisableDialog(int id);

public:
	int id;
	NpcNode* currentNode;
	eastl::list<NpcNode*> nodes;
	bool isDialogueActive;
};