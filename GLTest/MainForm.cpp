#include "pch.h"
#include "MainForm.h"

MainForm::MainForm()
{
	InitializeComponent();
	this->game = gcnew Game();
	// Have to hijack a panel as the designer doesn't support controls from a mixed assembly
	glCanvas = gcnew GLCanvas(this->game);
	glCanvas->Location = glCanvasPlaceholder->Location;
	glCanvas->Size = glCanvasPlaceholder->Size;
	glCanvas->Anchor = glCanvasPlaceholder->Anchor;
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

void MainForm::btnSomething_Click(Object^ sender, EventArgs^ e) {
	MessageBox::Show("test");
}