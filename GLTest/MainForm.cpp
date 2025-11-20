#include "pch.h"
#include "MainForm.h"

MainForm::MainForm()
{
	InitializeComponent();
	this->game = gcnew RubyDung();
	// Have to hijack a panel as the designer doesn't support controls from a mixed assembly
	glCanvas = gcnew GLCanvas(this->game);
	glCanvas->Location = glCanvasPlaceholder->Location;
	glCanvas->Size = glCanvasPlaceholder->Size;
	glCanvas->Anchor = glCanvasPlaceholder->Anchor;
	glCanvas->Dock = glCanvasPlaceholder->Dock;
	this->Controls->Add(glCanvas);
	this->Controls->Remove(glCanvasPlaceholder);
}

MainForm::~MainForm()
{
	if (components)
	{
		delete components;
	}
}

void MainForm::aboutToolStripMenuItem_Click(Object^ sender, EventArgs^ e) {
	ShellAboutA(CastPtr<HWND>(this->Handle), 
		"RubyDung#RubyDung C++/CLI Edition", 
		"RubyDung ported to C++/CLI by vlOd\nOriginal written by Markus Persson", 
		CastPtr<HICON>(SystemIcons::Information->Handle));
}

void MainForm::controlsToolStripMenuItem_Click(Object^ sender, EventArgs^ e) {
	MessageBox::Show(
		"LMB: Place block\n"
		"RMB: Break block\n"
		"Mouse Move: Look around\n"
		"W/A/S/D: Walk\n"
		"Space: Jump\n"
		"R: Reset position\n"
		"Enter: Save level\n"
		"Esc: Toggle cursor lock",
		"Controls",
		MessageBoxButtons::OK,
		MessageBoxIcon::Information);
}