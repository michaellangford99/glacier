#include "parameter_set.h"

void parameter_set::generate_imgui_editor()
{	
	for (auto& [name, p] : parameters)
	{
		ImGui::Text(p->name.c_str());
		p->generate_imgui_editor();
	}
}