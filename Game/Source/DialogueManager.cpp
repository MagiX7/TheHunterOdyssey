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
	}

	return true;
}

bool DialogueManager::Update(float dt)
{
	//current->Update(dt);


	return true;
}

void DialogueManager::Draw()
{
	// Render npc text
	app->render->DrawText(font, (*current->nodes.begin())->text.c_str(), 0, 0, 100, 5, { 255,255,255 });

	// Render options for the player
	eastl::list<DialogueOption*>::iterator optionsIterator = (*current->nodes.begin())->options.begin();
	if (current->nodes.size() > 1)
	{
		int offsetY = 0;
		for (; optionsIterator != (*current->currentNode).options.end(); ++optionsIterator)
		{
			app->render->DrawText(font, (*optionsIterator)->text.c_str(), 0, 110 + offsetY, 50, 5, { 0,255,0,255 });
			offsetY += 60;
		}
	}
}

void DialogueManager::DeleteDialogue(int id)
{
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
		tmp = new NpcNode(n.child("npc_text").attribute("text").as_string());

		for (pugi::xml_node m = n.child("option"); m; m = m.next_sibling("option"))
		{
			DialogueOption* option = new DialogueOption();
			option->text = m.attribute("text").as_string();
			option->id = m.attribute("id").as_int();
			option->nextId = m.attribute("nextNodeId").as_int();

			tmp->options.push_back(option);
		}

		ret->nodes.push_back(tmp);

	}

	ret->currentNode = (*ret->nodes.begin());

	return ret;
}