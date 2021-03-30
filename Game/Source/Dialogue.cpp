#include "App.h"
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

void Dialogue::Draw(int& count)
{

}

bool Dialogue::CleanUp()
{

	return true;
}

void Dialogue::TriggerDialog(int id)
{
	isDialogueActive = true;
}

void Dialogue::DisableDialog(int id)
{
}