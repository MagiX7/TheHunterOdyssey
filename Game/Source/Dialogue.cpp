#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "DialogueManager.h"
#include "Dialogue.h"

DialogueOption::DialogueOption()
{
	id = 0; nextNodeId = 0;
}

DialogueOption::~DialogueOption()
{
}

Dialogue::Dialogue(int dialogueId) : id(dialogueId)
{
	isDialogueActive = false;
}

Dialogue::~Dialogue()
{
}

void Dialogue::Draw(int& count, Font* font)
{
	// Render npc text like a writing machine
	++count;
	if (count % 2 == 0 && currentNode->stopDialog == false)
	{
		if (currentNode->letterCount < currentNode->text.size())
		{
			textToPrint += currentNode->text.at(currentNode->letterCount);
			++currentNode->letterCount;
		}
		else currentNode->dialogFinished = true;
	}

	app->render->DrawText(font, textToPrint.c_str(), 70, 115, 40, 5, { 0,0,0,255 }, 630);

	
	if (currentNode->dialogFinished)
	{
		// Render options for the player
		eastl::list<DialogueOption*>::iterator optionsIterator = currentNode->options.begin();

		SDL_Rect r = (*optionsIterator)->bounds;
		int offsetY = 0;		
		if (nodes.size() > 1)
		{
			for (; optionsIterator != (*currentNode).options.end(); ++optionsIterator)
			{
				r.y += offsetY;
				app->render->DrawText2(font, (*optionsIterator)->text.c_str(), r, 30, 5, { 20,20,20,255 }, 1210);
				//(*optionsIterator)->bounds.y += offsetY;
				offsetY += 90;
			}
		}
		else
		{
			app->render->DrawText2(font, (*optionsIterator)->text.c_str(), (*optionsIterator)->bounds, 30, 5, { 20,20,20,255 }, 1210);
		}

	}
}

bool Dialogue::CleanUp()
{
	textToPrint.clear();
	eastl::list<NpcNode*>::iterator it = nodes.begin();
	eastl::list<DialogueOption*>::iterator item;

	for (; it != nodes.end(); ++it)
	{
		(*it)->text.clear();
		
		for (item = (*it)->options.begin(); item != (*it)->options.end(); ++item)
		{
			(*item)->text.clear();
		}
		(*it)->options.clear();
	}

	nodes.clear();
	return true;
}

void Dialogue::TriggerDialog(int id)
{
	isDialogueActive = true;
}

void Dialogue::DisableDialog(int id)
{
}

void NpcNode::Reset()
{
	this->dialogFinished = false;
	this->letterCount = 0;
	this->stopDialog = false;
}

DialogueOption* NpcNode::SetCurrentOptionById(int id)
{
	eastl::list<DialogueOption*>::iterator it = options.begin();
	int i = 0;
	while (it != options.end())
	{
		if (id == i)
		{
			currentOption = (*it);
			return (*it);
		}
		++i;
		++it;
	}
	return nullptr;
}