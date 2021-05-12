#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "DialogueManager.h"
#include "Dialogue.h"
#include "Audio.h"

#include <time.h>

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

	talkingFx1 = app->audio->LoadFx("Assets/Audio/Fx/Gameplay/talking_1.ogg");
	talkingFx2 = app->audio->LoadFx("Assets/Audio/Fx/Gameplay/talking_2.ogg");
	talkingFx3 = app->audio->LoadFx("Assets/Audio/Fx/Gameplay/talking_3.ogg");
	talkingFx4 = app->audio->LoadFx("Assets/Audio/Fx/Gameplay/talking_4.ogg");
	firstTime = true;
	srand(time(NULL));

	channel = app->audio->SetChannel();
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

			if (firstTime || count % 40 == 0)
			{
				firstTime = false;
				int whichAudio = (rand() % 4) + 1;
				if (whichAudio == 1) app->audio->PlayFx(channel, talkingFx1);
				else if (whichAudio == 2) app->audio->PlayFx(channel, talkingFx2);
				else if (whichAudio == 3) app->audio->PlayFx(channel, talkingFx3);
				else if (whichAudio == 4) app->audio->PlayFx(channel, talkingFx4);
			}
		}
		else
		{
			firstTime = true;
			currentNode->dialogFinished = true;
		}
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
				app->render->DrawText(font, (*optionsIterator)->text.c_str(), r, 30, 5, { 20,20,20,255 }, 1210);
				//(*optionsIterator)->bounds.y += offsetY;
				offsetY += 90;
			}
		}
		else
		{
			app->render->DrawText(font, (*optionsIterator)->text.c_str(), (*optionsIterator)->bounds, 30, 5, { 20,20,20,255 }, 1210);
		}

	}
}

bool Dialogue::CleanUp()
{
	app->audio->UnLoadFx(talkingFx1);
	app->audio->UnLoadFx(talkingFx2);
	app->audio->UnLoadFx(talkingFx3);
	app->audio->UnLoadFx(talkingFx4);

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