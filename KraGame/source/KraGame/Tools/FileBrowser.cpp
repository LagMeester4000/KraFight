#include "KraGame/Tools/FileBrowser.h"
#include "KraGame/Detail/StringParse.h"
#include <imgui.h>
#include <cstring>
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;

game::FileBrowser::FileBrowser()
{
}

void game::FileBrowser::Open()
{
	strcpy(BrowserPath, Rootfolder.c_str());
	UpdateElements();
	strcpy(ToDraw, "");
}

bool game::FileBrowser::Draw()
{
	bool Ret = false;

	if (ImGui::Begin("File browser"))
	{
		// Current folder path
		if (ImGui::InputText("Path", BrowserPath, 64, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			UpdateElements();
		}

		// Back button
		ImGui::SameLine();
		if (ImGui::Button("Back"))
		{
			std::string BP = BrowserPath;
			size_t Len = BP.length();
			size_t LastFSlash = str::FindLastInstanceOf('/', BP);
			if (LastFSlash == Len)
				LastFSlash = 0;
			size_t LastBSlash = str::FindLastInstanceOf('\\', BP);
			if (LastBSlash == Len)
				LastBSlash = 0;

			size_t LastSlash = LastBSlash > LastFSlash ? LastBSlash : LastFSlash;
			BrowserPath[LastSlash] = 0;
			UpdateElements();

			if (Save)
				UpdateToDraw();
		}

		// Files and folders
		ImGui::Separator();
		ImGui::BeginChild("FileWindow", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);
		for (size_t I = 0; I < Elements.size(); ++I)
		{
			auto& It = Elements[I];
			
			// Select file
			std::string FileOrFolder = It.Folder ? "> " + It.Name : It.Name;
			if (ImGui::Selectable(FileOrFolder.c_str(), SelectedFile == (size_t)I))
			{
				SelectedFile = (int)I;
				if (Save)
					UpdateToDraw();
			}

			// Check if item is double clicked
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{
				if (!It.Folder)
				{
					if (!Save)
						Complete(It);
				}
				else
				{
					std::string PAsStr = BrowserPath;
					filesystem::path Path = PAsStr;
					Path.append(It.Name);
					PAsStr = Path.string();
					strcpy(BrowserPath, PAsStr.c_str());
					UpdateElements();
				}
			}

		}
		ImGui::EndChild();

		// Show file, choose, cancel
		{
			int Flags = Save ? ImGuiInputTextFlags_EnterReturnsTrue : ImGuiInputTextFlags_ReadOnly;
			ImGui::InputText("##Export", ToDraw, 64, Flags);

			std::string Str;
			if (!ImGui::IsItemActive() && !Save)
			{
				if (SelectedFile != -1 && SelectedFile < Elements.size())
				{
					if (!Elements[SelectedFile].Folder)
					{
						Str = BrowserPath;
						if (Str.size() > 0)
						{
							char C = Str[Str.size() - 1];
							if (C != '/' && C != '\\')
								Str += '/';
						}
						Str += Elements[SelectedFile].Name;
					}
				}
				strcpy(ToDraw, Str.c_str());
			}
			
			ImGui::SameLine();
			if (ImGui::Button("Choose"))
			{
				if (!Save)
					Result = Str;
				else
					Result = ToDraw;
				Ret = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				Result = "";
				Ret = true;
			}
		}
	}
	ImGui::End();

	return Ret;
}

void game::FileBrowser::UpdateElements()
{
	Elements.clear();
	SelectedFile = -1;

	std::string BP = BrowserPath;
	BP = "./" + BP;
	filesystem::path Path(BP);
	for (auto& It : filesystem::directory_iterator(Path))
	{
		Element Push;
		Push.Folder = filesystem::is_directory(It);
		if (Push.Folder)
		{
			Push.Name = It.path().filename().string();
			Elements.push_back(Push);
			continue;
		}

		std::string Extension = It.path().extension().string();
		for (auto& Exts : PermittedExtensions)
		{
			if (Extension == Exts)
			{
				Push.Name = It.path().filename().string();
				Elements.push_back(Push);
				break;
			}
		}
	}
	

}

void game::FileBrowser::UpdateToDraw()
{
	std::string Str = BrowserPath; 
	
	// Add "/" after path
	if (Str.size() > 0)
	{
		char C = Str[Str.size() - 1];
		if (C != '/' && C != '\\')
			Str += '/';
	}

	// Add selected folder/file
	if (SelectedFile != -1 && SelectedFile < Elements.size())
	{
		Str += Elements[SelectedFile].Name;
		
		if (Elements[SelectedFile].Folder)
		{
			Str += "/";
		}
	}

	strcpy(ToDraw, Str.c_str());
}

void game::FileBrowser::Complete(Element & El)
{
}
