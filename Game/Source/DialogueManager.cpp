#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "AssetsManager.h"
#include "SceneGameplay.h"
#include "QuestManager.h"
#include "DialogueManager.h"
#include "Easings.h"

#include "Log.h"

DialogueManager::DialogueManager(QuestManager* quests, SceneGameplay* s)
{
	questManager = quests;
	scene = s;
}

DialogueManager::~DialogueManager()
{
}

bool DialogueManager::Start()
{
	int size = app->assetsManager->MakeLoad("Xml/dialogues.xml");
	pugi::xml_parse_result result = file.load_buffer(app->assetsManager->GetLastBuffer(), size);
	app->assetsManager->DeleteBuffer();
	//pugi::xml_parse_result result = file.load_file("");

	if (result == NULL)
	{
		LOG("Could not load the file, pugi error: ", result.description());
	}
	else
	{
		root = file.child("dialogues");
		font = new Font(app, "Font/font3.xml", app->tex);
		texture = app->tex->Load("Textures/UI/gui_dialogue_textures.png");

		letterCount = 0;
		isDialogueActive = false;
		printText = false;
		current = nullptr;
		alpha = 150;
	}

	//Easing Arrow
	easingArrow = new Easing();
	easingArrow->initialPos = 0;
	easingArrow->currentIteration = 0;
	easingArrow->totalIterations = 60;
	easingArrow->deltaPos = 10;
	easingArrow->easingsActivated = false;

	easingArrow2 = new Easing();
	easingArrow2->initialPos = 0;
	easingArrow2->currentIteration = 0;
	easingArrow2->totalIterations = 60;
	easingArrow2->deltaPos = -10;
	easingArrow2->easingsActivated = false;

	arrowPosition = 0;

	return true;
}

bool DialogueManager::Update(float dt)
{
	bool ret = true;

	alpha += 100 * dt;

	//if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) printText = true;
	
	if (current != nullptr)
	{
		if(easingArrow2->easingsActivated == false) easingArrow->easingsActivated = true;

		easingArrow->initialPos = current->currentNode->currentOption->bounds.x - 30;
		easingArrow2->initialPos = easingArrow->initialPos + easingArrow->deltaPos;

		if (easingArrow->easingsActivated)
		{
			arrowPosition = easingArrow->circularEaseInOut(easingArrow->currentIteration, easingArrow->initialPos, easingArrow->deltaPos, easingArrow->totalIterations);

			if (easingArrow->currentIteration < easingArrow->totalIterations)
			{
				easingArrow->currentIteration++;
			}
			else
			{
				easingArrow->currentIteration = 0;
				easingArrow->easingsActivated = false;
				easingArrow2->easingsActivated = true;
			}
		}
		else if (easingArrow2->easingsActivated)
		{
			arrowPosition = easingArrow2->circularEaseInOut(easingArrow2->currentIteration, easingArrow2->initialPos, easingArrow2->deltaPos, easingArrow2->totalIterations);
	
			if (easingArrow2->currentIteration < easingArrow2->totalIterations)
			{
				easingArrow2->currentIteration++;
			}
			else
			{
				easingArrow2->currentIteration = 0;
				easingArrow2->easingsActivated = false;
				easingArrow->easingsActivated = true;
			}
		}

		if (current->currentNode->dialogFinished == true && current->currentNode->id >= 0)
		{
			if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_UP)
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

			if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_UP)
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
			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_A) == KEY_UP)
			{
				NpcNode* aux = GetNodeById(current->currentNode->currentOption->nextNodeId);
				/*current->currentNode=current->currentNode.*/
				if (current->currentNode->currentOption->missionId != -1) questManager->ActivateQuest(current->currentNode->currentOption->missionId);
				if (current->currentNode->currentOption->menu != -1) scene->ChangeState(GameplayMenuState::SHOP);

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
			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_A) == KEY_UP)
			{
				// Dialog finished
				current->currentNode->dialogFinished = false;
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
		app->render->DrawTexture(texture, 50, 100, &sect, false);

		// Draw the background for the player options
		sect = { 615, 137, 556, 203 };
		app->render->DrawTexture(texture, 670, 200, &sect, false);

		// Draw the text
		if (printText == true && current->currentNode->id >= -1)
		{
			current->Draw(letterCount, font);
		}
		else if (current->currentNode->id == -1)
		{
			printText = false;
		}

		// Draw the arrow to give visual feedback of the current option
		if (current->currentNode->dialogFinished)
		{
			sect = { 622, 352, 16,23 };
			app->render->DrawTexture(texture, arrowPosition, current->currentNode->currentOption->bounds.y, &sect, false);
		}
	}
}

bool DialogueManager::UnLoad()
{
	app->tex->UnLoad(texture);

	font->UnLoad(app->tex);
	RELEASE(font);

	if (current != nullptr)
	{
		current->CleanUp();
		RELEASE(current);
	}

	RELEASE(easingArrow);
	RELEASE(easingArrow2);

	eastl::list<Dialogue*>::iterator aux = dialogues.begin();
	for (aux; aux != dialogues.end(); aux++)
	{
		(*aux)->CleanUp();
		RELEASE((*aux));
		dialogues.remove(*aux);
	}
	
	dialogues.clear();

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
		option->missionId = m.attribute("missionId").as_int();
		option->menu = m.attribute("menu").as_int(-1);
		option->bounds.x = 710;
		option->bounds.y = 215 + i;
		//option->bounds.w = 400;
		int offset = font->GetBaseSize();
		option->bounds.w = option->text.size() * offset;
		option->bounds.h = font->GetBaseSize() + 10;

		tmp->options.push_back(option);
		++tmp->optionsNum;
		i += 90;
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

	current = ret;
	return ret;
}