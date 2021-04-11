#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"

#include "DialogueManager.h"

#include "Log.h"

DialogueManager::DialogueManager()
{
}

DialogueManager::~DialogueManager()
{
}

bool DialogueManager::Start()
{
	pugi::xml_parse_result result = file.load_file("dialogues.xml");

	if (result == NULL)
	{
		LOG("Could not load the file, pugi error: ", result.description());
	}
	else
	{
		root = file.child("dialogues");
		font = new Font("Assets/Font/font3.xml", app->tex);
		texture = app->tex->Load("Assets/Textures/dialogue_background.png");
		//current = LoadDialogue(0);

		letterCount = 0;
		printText = false;
		current = nullptr;
		alpha = 150;
	}

	return true;
}

bool DialogueManager::Update(float dt)
{
	bool ret = true;

	alpha += 100 * dt;

	//if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) printText = true;
	
	if (current != nullptr)
	{
		if (current->currentNode->dialogFinished == true && current->currentNode->id >= 0)
		{
			if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || app->input->pads->down)
			{
				eastl::list<DialogueOption*>::iterator it = current->currentNode->options.begin();
				for (int i = 0; i < current->currentNode->optionsNum; ++i, ++it)
				{
					if ((*it)->id == current->currentNode->currentOption->id + 1)
					{
						current->currentNode->currentOption = (*it);
						break;
					}
				}
			}

			if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || app->input->pads->up)
			{
				eastl::list<DialogueOption*>::iterator it = current->currentNode->options.begin();
				for (int i = 0; i < current->currentNode->optionsNum; ++i, ++it)
				{
					if ((*it)->id == current->currentNode->currentOption->id - 1)
					{
						current->currentNode->currentOption = (*it);
						break;
					}
				}
			}

			// If player presses enter, means he has chosen an option
			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->pads->a)
			{
				NpcNode* aux = GetNodeById(current->currentNode->currentOption->nextNodeId);
				/*current->currentNode=current->currentNode.*/
				RELEASE(current->currentNode);
				current->currentNode = aux;
				current->currentNode->Reset();
				letterCount = 0;
				current->textToPrint.clear();
				current->currentNode->currentOption = (*aux->options.begin());
			}
		}

		if (current->currentNode->id == -1)
		{
			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->pads->a)
			{
				// Dialog finished
				//current->currentNode->dialogFinished = false;
				isDialogueActive = false;
				printText = false;
				ret = false;
			}
		}
	}
	
	return ret;
}

void DialogueManager::Draw()
{
	if (current != nullptr)
	{
		// Draw the background for the npc text
		SDL_Rect sect = { 0,0,612, 479 };
		app->render->DrawTexture(texture, 50 - (app->render->camera.x ), 100 - (app->render->camera.y), &sect);

		// Draw the background for the player options
		sect = { 615, 137, 556, 203 };
		app->render->DrawTexture(texture, 670 - (app->render->camera.x ), 200 - (app->render->camera.y ), &sect);

		if (current->currentNode->dialogFinished)
		{
			if (alpha >= 255)
			{
				alpha = 0;
			}
			//SDL_Rect r = { current->currentNode->currentOption->bounds.x,  current->currentNode->currentOption->bounds.y, 400,50 };
			SDL_Rect r = { current->currentNode->currentOption->bounds.x + (app->render->camera.x * (-1))  , current->currentNode->currentOption->bounds.y + (app->render->camera.y * (-1)) ,
				current->currentNode->currentOption->bounds.w,current->currentNode->currentOption->bounds.h };

			app->render->DrawRectangle(r, 149, 255, 255, alpha);
		}

		if (printText == true && current->currentNode->id >= -1)
		{
			current->Draw(letterCount, font);
		}
		else if (current->currentNode->id == -1)
		{
			printText = false;
		}
	}
}

bool DialogueManager::UnLoad()
{
	font->UnLoad(app->tex);

	RELEASE(font);

	return true;
}

NpcNode* DialogueManager::LoadNode(int id, pugi::xml_node node)
{
	NpcNode* tmp = new NpcNode(node.child("npc_text").attribute("text").as_string());
	tmp->id = node.attribute("id").as_int();
	int i = 0;
	for (pugi::xml_node m = node.child("option"); m; m = m.next_sibling("option"))
	{
		DialogueOption* option = new DialogueOption();
		option->text = m.attribute("text").as_string();
		option->id = m.attribute("id").as_int();
		option->nextNodeId = m.attribute("nextNodeId").as_int();
		option->bounds.x = 680;
		option->bounds.y = 215 + i;
		//option->bounds.w = 400;
		int offset = font->GetBaseSize();
		option->bounds.w = option->text.size() * offset;
		option->bounds.h = 50;

		tmp->options.push_back(option);
		++tmp->optionsNum;
		i += 58;
	}

	return tmp;
}

NpcNode* DialogueManager::GetNodeById(int id)
{
	eastl::list<NpcNode*>::iterator it = current->nodes.begin();
	if (current->nodes.size() > 1)
	{
		while (((*it)->id != id) && (it != current->nodes.end()))
			it = it.next();
	}

	return (*it);
}

Dialogue* DialogueManager::LoadDialogue(int id)
{
	Dialogue* ret = new Dialogue(id);

	// Search the dialogue with the npc we want to load
	pugi::xml_node npcNode = root.child("npc");

	while (npcNode.attribute("id").as_int() != id)
		npcNode = npcNode.next_sibling("npc");
	int a = npcNode.attribute("id").as_int();
	// Load the npc text + all the possible options
	NpcNode* tmp = nullptr;
	
	for (pugi::xml_node n = npcNode.child("node"); n; n = n.next_sibling("node"))
	{
		tmp = LoadNode(n.attribute("id").as_int(),n);
		ret->nodes.push_back(tmp);
	}
	
	eastl::list<NpcNode*>::iterator it = ret->nodes.begin();
	for (; it != ret->nodes.end(); ++it)
	{
		eastl::list<DialogueOption*>::iterator itOptions = (*it)->options.begin();
		for (; itOptions != (*it)->options.end(); ++itOptions)
		{
			for (eastl::list<NpcNode*>::iterator it2 = ret->nodes.begin(); it2 != ret->nodes.end(); ++it2)
			{
				if ((*itOptions)->nextNodeId == (*it2)->id)
					(*itOptions)->nextNode = (*it2);
			}
		}
	}

	// Set current option to later draw the box (visual feedback)
	//(*ret->nodes.begin())->currentOption = new DialogueOption();
	(*ret->nodes.begin())->currentOption = (*(*ret->nodes.begin())->options.begin());

	ret->currentNode = (*ret->nodes.begin());

	isDialogueActive = true;

	return ret;
}