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
		current = LoadDialogue(0);

		letterCount = 0;
		printText = false;

	}

	return true;
}

bool DialogueManager::Update(float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) printText = true;

	if (current->currentNode->dialogFinished == true && current->currentNode->id >= 0)
	{
		if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			lastUserInput = 1;
			current->currentNode->currentOption = (*(*current->nodes.begin())->options.begin());
		}
		else if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
			lastUserInput = 2;
			current->currentNode->currentOption = (*(*current->nodes.begin())->options.begin().next());
		}
		else if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		{
			lastUserInput = 3;
			current->currentNode->currentOption = (*(*current->nodes.begin())->options.begin().next().next());
		}

		// If player presses enter, means he has chosen an option
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			NpcNode* aux = GetNodeById(current->currentNode->currentOption->nextNodeId);
			RELEASE(current->currentNode);
			current->currentNode = LoadNode(aux->id);
			current->currentNode->Reset();
			letterCount = 0;
			current->textToPrint.clear();
			current->currentNode->currentOption = (*aux->options.begin());
		}
	}
	if (current->currentNode->id == -1)
	{
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			// Dialog finished
			printText = false;
			ret = false;
		}
	}

	return ret;
}

void DialogueManager::Draw()
{
	if (printText == true && current->currentNode->id >= -1)
	{
		current->Draw(letterCount, font);
	}
}

NpcNode* DialogueManager::LoadNode(int id)
{
	pugi::xml_node n = root.child("npc").first_child();
	
	// Find the node
	while (n.attribute("id").as_int() != id)
		n = n.next_sibling("node");

	NpcNode* tmp = new NpcNode(n.child("npc_text").attribute("text").as_string());
	tmp->id = n.attribute("id").as_int();
	for (pugi::xml_node m = n.child("option"); m; m = m.next_sibling("option"))
	{
		DialogueOption* option = new DialogueOption();
		option->text = m.attribute("text").as_string();
		option->id = m.attribute("id").as_int();
		option->nextNodeId = m.attribute("nextNodeId").as_int();
		option->bounds.x = 0;
		option->bounds.y = 110;
		option->bounds.w = 200;
		option->bounds.h = 50;

		tmp->options.push_back(option);
	}

	return tmp;
}

NpcNode* DialogueManager::GetNodeById(int id)
{
	eastl::list<NpcNode*>::iterator it = current->nodes.begin();
	while (((*it)->id != id) && (it != current->nodes.end()))
		it = it.next();

	return (*it);
}

Dialogue* DialogueManager::LoadDialogue(int id)
{
	Dialogue* ret = new Dialogue(id);

	// Search the dialogue with the npc we want to load
	pugi::xml_node npcNode = root.child("npc");
	while (npcNode.attribute("id").as_int() != id)
		npcNode = npcNode.next_sibling("npc");

	// Load the npc text + all the possible options
	NpcNode* tmp = nullptr;
	
	for (pugi::xml_node n = npcNode.child("node"); n; n = n.next_sibling("node"))
	{
		tmp = LoadNode(n.attribute("id").as_int());
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
	(*ret->nodes.begin())->currentOption = new DialogueOption();
	(*ret->nodes.begin())->currentOption = (*(*ret->nodes.begin())->options.begin());

	ret->currentNode = (*ret->nodes.begin());

	return ret;
}