#pragma once
#include <string>
#include <vector>

namespace game {
	// File browser UI using ImGui
	class FileBrowser {
	public:
		FileBrowser();

		// Resets some internal values in the file browser
		void Open();

		// Draw the file browser
		// Returns true when file is selected or window is cosed
		bool Draw();

	public:
		std::vector<std::string> PermittedExtensions;
		std::string Rootfolder = "";
		bool WindowOpen = false;
		// If this is a window for saving a file
		bool Save = false;

		// Resulting value 
		std::string Result = "";

	private:
		void UpdateElements();
		void UpdateToDraw();

		struct Element {
			bool Folder = false;
			std::string Name = "";
		};

		// The window will close and the file selection is complete
		void Complete(Element& El);

	private:
		char BrowserPath[64] = "";
		std::vector<Element> Elements;
		int SelectedFile = -1;
		char ToDraw[64] = "";
	};
}